#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T13:42:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BciSift
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

SOURCES += main.cpp \
    bcisift.cpp \
    eegimage.cpp \
    scalespace.cpp \
    plotprocessing.cpp \
    dsp.cpp \
    lsl.cpp

#INCLUDEPATH += D:\opencv\build\include\
INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /Users/rramele/work/kfr/include
INCLUDEPATH += /Users/rramele/work/labstreaminglayer/build/install/lsl_Release/LSL/include

#LIBS += -LD:\opencv\build\x86\vc11\lib \
#-lopencv_core249d \
#-lopencv_highgui249d \
#-lopencv_imgproc249d \
#-lopencv_features2d249d \
#-lopencv_calib3d249d \
#-lopencv_nonfree249d

LIBS += -L/usr/local/lib -L/Users/rramele/work/labstreaminglayer/build/install/lsl_Release/LSL/lib \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_xfeatures2d \
-lopencv_calib3d \
-lopencv_imgcodecs \
-lopencv_ml \
-lopencv_flann \
-llsl64



HEADERS += \
    eegimage.h \
    plotprocessing.h \
    dsp.h \
    lsl.h
