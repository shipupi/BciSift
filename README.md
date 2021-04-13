# BciSift

## Installation

### Requirements

- (Linux) Extra libraries I had to install in order to install OpenCV:
```sh
sudo apt install libgtk2.0-dev pkg-config libcanberra-gtk-module libcanberra-gtk3-module 
```

- OpenCv. Installation instructions in : https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html ( Install with opencv_contrib)

- KFR. https://github.com/kfrlib/kfr.git. After downloading, link the directory with ln -s so that kfr is a subdirectory of the root folder

- VLFeat. Clone from https://github.com/faturita/vlfeat and add the path to cmake/FindVLFeat.cmake

- Tensorflow:
    1. Install Bazel: https://docs.bazel.build/versions/4.0.0/install-ubuntu.html
    2. 

I've run thesse commands using linux, Cmake however is supposed to be portable (at least in windows and linux), but I have not tried it on another platform yet.

For MAC installation, check the original repository on https://github.com/faturita/BciSift 


### Building

- You may need to export  OpenCV_DIR variable. However this is probably due to an incorrect opencv installation
```sh
export OpenCV_DIR=<path_of_opencv/build>
```
- Build makefile with Cmake, and then run it
```sh
cmake -DCMAKE_CXX_COMPILER=clang++
make
```

