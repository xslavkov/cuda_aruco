# Install script for directory: C:/Users/Mato/Fixer/cuda_aruco/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/aruco")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "main" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/Debug/aruco130.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/Release/aruco130.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/MinSizeRel/aruco130.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/RelWithDebInfo/aruco130.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "main" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/Debug/aruco130.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/Release/aruco130.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/MinSizeRel/aruco130.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Mato/Fixer/cuda_aruco/build/bin/RelWithDebInfo/aruco130.dll")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "main" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aruco" TYPE FILE FILES
    "C:/Users/Mato/Fixer/cuda_aruco/src/ar_omp.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/aruco.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/arucofidmarkers.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/board.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/boarddetector.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/cameraparameters.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/chromaticmask.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/cudaImage.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/cuda_aruco.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/cudautils.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/cvdrawingutils.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/exports.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/highlyreliablemarkers.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/marker.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/markerdetector.h"
    "C:/Users/Mato/Fixer/cuda_aruco/src/subpixelcorner.h"
    )
endif()

