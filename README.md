# BciSift 


## Mac Installation

OpenCV is tricky to install on Mac, this is the best and easier way that I found so far:

First, install brew.  Then do this

```
 brew tap homebrew/science
 brew install opencv
```
 
## Add into the .pro file

```
 LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_objdetect \
    -lopencv_calib3d

 INCLUDEPATH += /usr/local/include/opencv
```

And the most important part for Yosemite, add this into the .pro file:

```
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
```

## Refs

* http://tilomitra.com/opencv-on-mac-osx/
* http://stackoverflow.com/questions/21811858/opencv-mac-os-x-qt-creator-strange-linking-bug
