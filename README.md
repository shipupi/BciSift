# BciSift

## Installation

### Requirements

- (Linux) Extra libraries I had to install in order to install OpenCV:
```sh
sudo apt install libgtk2.0-dev pkg-config libcanberra-gtk-module libcanberra-gtk3-module 
```

- OpenCv. Installation instructions in : https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html



I've run thesse commands using linux, Cmake however is supposed to be portable (at least in windows and linux), but I have not tried it on another platform yet.

For MAC installation, check the original repository on https://github.com/faturita/BciSift 


### Building

- Make sure the OpenCV_DIR variable is exported
```sh
export OpenCV_DIR=<path_of_opencv/build>
```
- Build makefile with Cmake, and then run it
```sh
cmake .
make
```

