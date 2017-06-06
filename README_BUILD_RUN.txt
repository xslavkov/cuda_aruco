- git clone
- cmake - target build priecinok
- prepnut build na release
- build all - spadne to
- nakopyrovat vo vs z projektu cuda_aruco Object Files (priecinok aj so subormi) do projektu aruco (druhy z hora !!!)
- build all
- nakopyrovat v OS c:\Intel\INDE\cuda_aruco\build\src\CMakeFiles\cuda_aruco.dir\Release\*.obj do c:\Intel\INDE\cuda_aruco\build\src\CMakeFiles\cuda_aruco.dir\Debug\
- build all

pre uspesny run pridat buffer MDetector.createCudaBuffers(TheInputImage.size().width, TheInputImage.size().height);
na riadok 194 v aruco_hrm_test.cpp

do build/bin/release nakopyrovat:
opencv_calib3d310.dll
opencv_core310.dll
opencv_features2d310.dll
opencv_flann310.dll
opencv_highgui310.dll
opencv_imgcodecs310.dll
opencv_imgproc310.dll
opencv_ml310.dll
opencv_videoio310.dll