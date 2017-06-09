/*****************************************************************************************
Copyright 2011 Rafael Muñoz Salinas. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Rafael Muñoz Salinas.
********************************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include "aruco.h"
#include "cvdrawingutils.h"
#include <opencv2/highgui/highgui.hpp>
#include "highlyreliablemarkers.h"
using namespace cv;
using namespace aruco;

string TheInputImagePath;
string TheLogFilePath;
string TheLogFileTotalPath;
string TheIntrinsicFile;
string TheDictionaryFile;
ofstream logFile;
ofstream logFileTotal;
float TheMarkerSize = -1;
int loopCount;
int ThePyrDownLevel;
bool useCuda;
MarkerDetector MDetector;
vector< Marker > TheMarkers;
Mat TheInputImage, TheInputImageCopy;
CameraParameters TheCameraParameters;
void cvTackBarEvents(int pos, void *);
bool readCameraParameters(string TheIntrinsicFile, CameraParameters &CP, Size size);

pair< double, double > AvrgTime(0, 0); // determines the average time required for detection
double ThresParam1 = 21;
double ThresParam2 = 7;
int waitTime = 0;
bool lockedCorners = false;
int findParam(std::string param, int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        if (string(argv[i]) == param)
            return i;

    return -1;
}

string getParamString(string param, int argc, char **argv, string defvalue = "") {
    int idx = -1;
    for (int i = 0; i < argc && idx == -1; i++)
        if (string(argv[i]) == param)
            idx = i;
    if (idx == -1)
        return defvalue;
    else
        return argv[idx + 1];
}
float getParamVal(string param, int argc, char **argv, float defvalue = -1) {
    int idx = -1;
    for (int i = 0; i < argc && idx == -1; i++)
        if (string(argv[i]) == param)
            idx = i;
    if (idx == -1)
        return defvalue;
    else
        return atof(argv[idx + 1]);
}

/************************************
 *
 ************************************/
bool readArguments(int argc, char **argv) {
    bool mustExit = false;
    TheInputImagePath = getParamString("-in", argc, argv);
    TheDictionaryFile = getParamString("-d", argc, argv);
	TheLogFilePath = getParamString("-o", argc, argv);
	TheLogFileTotalPath = getParamString("-o2", argc, argv);
    TheIntrinsicFile = getParamString("-i", argc, argv);
	loopCount = getParamVal("-l", argc, argv, 1);
    TheMarkerSize = getParamVal("-s", argc, argv, -1);
	useCuda = getParamVal("-cuda", argc, argv, 1) == 1;
    if (findParam("-lockedcorners", argc, argv) != -1)
        lockedCorners = true;
    if (TheInputImagePath.empty()) {
        mustExit = true;
        cerr << "-in required" << endl;
    }
    if (TheDictionaryFile.empty()) {
        mustExit = true;
        cerr << "-d required" << endl;
    }
	if (TheLogFilePath.empty() || TheLogFileTotalPath.empty()) {
		mustExit = true;
		cerr << "-o or -o2 required" << endl;
	}

    if (findParam("-h", argc, argv) != -1)
        mustExit = true;

    if (mustExit) {
        cerr << "Invalid number of arguments" << endl;
        cerr << "Usage: -in  (image) -d (dictionary.yml) -o (logfile.csv) -o2 (logfileTotal.csv) [-cuda 0/1] [-l loopCount] [-i intrinsics.yml] [-s size] [-lockedcorners]\n \
	in.avi/live: open videofile or connect to a camera using the OpenCV library \n \
	dictionary.yml: input marker dictionary used for detection \n \
	intrinsics.yml: input camera parameters (in OpenCV format) to allow camera pose estimation \n \
    size: markers size in meters to allow camera pose estimation." << endl;
        return false;
    }



    return true;
}
double getDetectionTime(double startTick) {
	return (((double)getTickCount() - startTick) / getTickFrequency()) * 1000;
}
/************************************
 *
 ************************************/
int main(int argc, char **argv) {
    try {
        if (readArguments(argc, argv) == false) {
            return 0;
        }
		TheInputImage = imread(TheInputImagePath);
		if (!TheLogFilePath.empty()) {
			logFile.open(TheLogFilePath);
			logFile << "Time detection (ms);Detected markers" << endl;
		}
		if (!TheLogFileTotalPath.empty()) {
			logFileTotal.open(TheLogFileTotalPath);
			logFileTotal << "Total detection time (ms)" << endl;
		}

        // check video is open
		if (TheInputImage.empty()) {
			cerr << "Could not open image: " << TheInputImagePath << endl;
            return -1;
        }

        Dictionary D;
        if (D.fromFile(TheDictionaryFile) == false) {
            cerr << "Could not open dictionary" << endl;
            return -1;
        };

        if (D.size() == 0) {
            cerr << "Invalid dictionary" << endl;
            return -1;
        };

        HighlyReliableMarkers::loadDictionary(D);

        // read camera parameters if passed
        if (TheIntrinsicFile != "") {
            TheCameraParameters.readFromXMLFile(TheIntrinsicFile);
            TheCameraParameters.resize(TheInputImage.size());
        }
        // Configure other parameters
		if (ThePyrDownLevel > 0) {
			MDetector.pyrDown(ThePyrDownLevel);
		}

        MDetector.enableLockedCornersMethod(lockedCorners);
		MDetector.enableCuda(useCuda);
        MDetector.setMakerDetectorFunction(aruco::HighlyReliableMarkers::detect);
        MDetector.setThresholdParams(21, 7);
        MDetector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
        MDetector.setWarpSize((D[0].n() + 2) * 8);
        MDetector.setMinMaxSize(0.005, 0.5);
        MDetector.getThresholdParams(ThresParam1, ThresParam2);
		//if (useCuda) {
			MDetector.createCudaBuffers(TheInputImage.size().width, TheInputImage.size().height);
		//}

        // 	    cv::cvtColor(TheInputImage,TheInputImage,CV_BayerBG2BGR);

		double tick;
		double tickTotal = (double)getTickCount(); // for checking the speed
		for (int j = 0; j < loopCount; j++) {
			tick = (double)getTickCount(); // for checking the speed
			// Detection of markers in the image passed
			MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
			// chekc the speed by calculating the mean speed of all iterations

			if (!TheLogFilePath.empty()) {
				logFile << getDetectionTime(tick) << ";";
				for (unsigned int i = 0; i < TheMarkers.size(); i++) {
					//TheMarkers[i].draw(TheInputImage, Scalar(0, 0, 255), 1);
					if (i != 0) {
						logFile << ",";
					}
					logFile << to_string(TheMarkers[i].id);
				}
				logFile << endl;
			}
		}
		if (!TheLogFileTotalPath.empty()) {
			logFileTotal << getDetectionTime(tickTotal) << endl;
		}

		//Mat small;
		//Size outputSize(640, 400);
		//resize(TheInputImage, small, outputSize);
		//imshow("Window", small);
  //      cv::waitKey(0); // wait for key to be pressed

		if (!TheLogFilePath.empty()) {
			logFile.close();
		}
		if (!TheLogFileTotalPath.empty()) {
			logFileTotal.close();
		}

    } catch (std::exception &ex)

    {
        cout << "Exception :" << ex.what() << endl;
    }
}