# ===================================================================================
#  aruco CMake configuration file
#
#             ** File generated automatically, do not modify **
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#
#    FIND_PACKAGE(aruco REQUIRED )
#    TARGET_LINK_LIBRARIES(MY_TARGET_NAME )
#
#    This file will define the following variables:
#      - aruco_LIBS          : The list of libraries to links against.
#      - aruco_LIB_DIR       : The directory where lib files are. Calling LINK_DIRECTORIES
#                                with this path is NOT needed.
#      - aruco_VERSION       : The  version of this PROJECT_NAME build. Example: "1.2.0"
#      - aruco_VERSION_MAJOR : Major version part of VERSION. Example: "1"
#      - aruco_VERSION_MINOR : Minor version part of VERSION. Example: "2"
#      - aruco_VERSION_PATCH : Patch version part of VERSION. Example: "0"
#
# ===================================================================================
INCLUDE_DIRECTORIES("C:/Program Files/aruco/include")
SET(aruco_INCLUDE_DIRS "C:/Program Files/aruco/include")

LINK_DIRECTORIES("C:/Program Files/aruco/lib")
SET(aruco_LIB_DIR "C:/Program Files/aruco/lib")

SET(aruco_LIBS opencv_videostab;opencv_videoio;opencv_video;opencv_superres;opencv_stitching;opencv_shape;opencv_photo;opencv_objdetect;opencv_ml;opencv_imgproc;opencv_imgcodecs;opencv_highgui;opencv_flann;opencv_features2d;opencv_cudev;opencv_cudawarping;opencv_cudastereo;opencv_cudaoptflow;opencv_cudaobjdetect;opencv_cudalegacy;opencv_cudaimgproc;opencv_cudafilters;opencv_cudafeatures2d;opencv_cudacodec;opencv_cudabgsegm;opencv_cudaarithm;opencv_core;opencv_calib3d aruco130)

SET(aruco_FOUND 1)
SET(aruco_VERSION        1.3.0)
SET(aruco_VERSION_MAJOR  1)
SET(aruco_VERSION_MINOR  3)
SET(aruco_VERSION_PATCH  0)
