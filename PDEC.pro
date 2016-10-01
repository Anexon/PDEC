#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T11:33:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDEC
TEMPLATE = app

INCLUDEPATH += /user/include
LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_flann -lcurl -lopencv_imgproc -lopencv_nonfree

SOURCES += main.cpp\
        mainwindow.cpp \
    OculusTI.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    OculusTI.h \
    player.h

FORMS    += mainwindow.ui

RESOURCES     += appResources.qrc
