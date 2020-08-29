# BciSift 

This program takes a monochannel signal, a time series of doubles, and converts that signal into an image, a standardized plot.  It takes the image and use a variant of the SIFT descriptor to capture their visual shape and retuns a descriptor which represents the shape.

The library uses:

- OpenCV
- VlFeat: their fastest SIFT implementation, a modified version.
- KFR: A very fast signal processing library.
- LSL: Labstreamlayer protocol for connecting to EEG sources.

# Usage

```
import pybcisift
import numpy as np
el = np.zeros((1,256))
el[0,23]=200
descr = pybcisift.extract(el.tolist()[0])
print(descr)
print(len(descr))
```

The list 'descr' is a SIFT^ descriptor which represents the signal shape, and can be used later for comparision.

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
* https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/UsingDynamicLibraries.html
