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


SOURCES += main.cpp

INCLUDEPATH += D:\opencv\build\include\

LIBS += -LD:\opencv\build\x86\vc11\lib \
-lopencv_core249d \
-lopencv_highgui249d \
-lopencv_imgproc249d \
-lopencv_features2d249d \
-lopencv_calib3d249d \
-lopencv_nonfree249d
