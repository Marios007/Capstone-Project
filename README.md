# CPPND: Capstone - Object dectection with Yolo and openCV

## Description

This project can detect objects in pictures. The implementation is using [OpenCV](https://github.com/opencv/opencv/master) and [yolov3-tiny](https://pjreddie.com/darknet/yolo/).
The programm opens a file an perform the object detection. As a result a frame is drawn around the object and also the name and confidence level is shown. The program can be used with command line



## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo from .
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it exmplae: `make && ./openCVdetector --input=/home/mario/Code/CppND-Capstone/Capstone-Project/files/a.jpg`