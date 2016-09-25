#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T11:33:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDEC
TEMPLATE = app

INCLUDEPATH += /user/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lcurl -lopencv_imgproc -lopencv_videoio

SOURCES += main.cpp\
        mainwindow.cpp \
    OculusTI.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    OculusTI.h \
    player.h

FORMS    += mainwindow.ui

DISTFILES += \
    videos/rgbThermal1.avi \
    videos/rgbVideo1.avi \
    videos/rgbVideo2.avi \
    videos/rgbVideo3.avi \
    videos/rgbVideo4.avi \
    videos/rgbVideo5.avi \
    videos/thermalVideo2.avi \
    videos/thermalVideo3.avi \
    videos/thermalVideo4.avi \
    videos/thermalVideo5.avi \
    videos/Video.avi

RESOURCES     += appResources.qrc
