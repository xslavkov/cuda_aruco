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
string TheIntrinsicFile;
string TheDictionaryFile;
ofstream logFile;
float TheMarkerSize = -1;
int ThePyrDownLevel;
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
    TheIntrinsicFile = getParamString("-i", argc, argv);
    TheMarkerSize = getParamVal("-s", argc, argv, -1);
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
	if (TheLogFilePath.empty()) {
		mustExit = true;
		cerr << "-o required" << endl;
	}

    if (findParam("-h", argc, argv) != -1)
        mustExit = true;

    if (mustExit) {
        cerr << "Invalid number of arguments" << endl;
        cerr << "Usage: -in  (image) -d (dictionary.yml) -o (logfile.csv) [-i intrinsics.yml] [-s size] [-lockedcorners]\n \
	in.avi/live: open videofile or connect to a camera using the OpenCV library \n \
	dictionary.yml: input marker dictionary used for detection \n \
	intrinsics.yml: input camera parameters (in OpenCV format) to allow camera pose estimation \n \
    size: markers size in meters to allow camera pose estimation." << endl;
        return false;
    }



    return true;
}
double getDetectionTime(double startTick) {
	return ((double)getTickCount() - startTick) / getTickFrequency();
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
		logFile.open(TheLogFilePath);
		logFile << "Time detection (ms);Detected markers" << endl;

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
        MDetector.setMakerDetectorFunction(aruco::HighlyReliableMarkers::detect);
        MDetector.setThresholdParams(21, 7);
        MDetector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
        MDetector.setWarpSize((D[0].n() + 2) * 8);
        MDetector.setMinMaxSize(0.005, 0.5);
        MDetector.getThresholdParams(ThresParam1, ThresParam2);
		MDetector.createCudaBuffers(TheInputImage.size().width, TheInputImage.size().height);

        // 	    cv::cvtColor(TheInputImage,TheInputImage,CV_BayerBG2BGR);

        double tick = (double) getTickCount(); // for checking the speed
        // Detection of markers in the image passed
        MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);
        // chekc the speed by calculating the mean speed of all iterations
		logFile << getDetectionTime(tick) << ";";

		for (unsigned int i = 0; i < TheMarkers.size(); i++) {
			if (i != 0) {
				logFile << ",";
			}
			logFile << to_string(TheMarkers[i].id);
		}
		logFile << endl;

        //cv::waitKey(0); // wait for key to be pressed

		logFile.close();

    } catch (std::exception &ex)

    {
        cout << "Exception :" << ex.what() << endl;
    }
}