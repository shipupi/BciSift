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

# Evita los errores de auto header en las cabeceras tipo c++11
CONFIG += c++14

TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

SOURCES += main.cpp \
    bcisift.cpp \
    eegimage.cpp \
    scalespace.cpp \
    plotprocessing.cpp \
    dsp.cpp \
    lsl.cpp \
    lsltransmitter.cpp \
    decoder.cpp \
    serializer.cpp \
    unp.cpp

#INCLUDEPATH += D:\opencv\build\include\

INCLUDEPATH += /Users/rramele/work/opencv/include/
INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /Users/rramele/work/kfr/include
INCLUDEPATH += /Users/rramele/work/labstreaminglayer/build/install/LSL/include/
INCLUDEPATH += /Users/rramele/work/vlfeat/

#LIBS += -LD:\opencv\build\x86\vc11\lib \
#-lopencv_core249d \
#-lopencv_highgui249d \
#-lopencv_imgproc249d \
#-lopencv_features2d249d \
#-lopencv_calib3d249d \
#-lopencv_nonfree249d

LIBS += -L/usr/local/lib -L/Users/rramele/work/labstreaminglayer/build/install/LSL/lib/ -L/Users/rramele/work/vlfeat/bin/maci64/ \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_xfeatures2d \
-lopencv_calib3d \
-lopencv_imgcodecs \
-lopencv_ml \
-lopencv_flann \
-llsl64 \
-lvl



HEADERS += \
    eegimage.h \
    plotprocessing.h \
    dsp.h \
    lsl.h \
    decoder.h \
    serializer.h \
    unp.h \
    spellerletter.h
