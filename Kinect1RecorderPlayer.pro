#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T15:37:45
#
#-------------------------------------------------

QT       += core gui network \
      xml \
      multimedia \
      multimediawidgets \
      widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kinect1RecorderPlayer
TEMPLATE = app


SOURCES += main.cpp\
        kinect1recoderplayer.cpp \
    histogramwidget.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    videowidget.cpp

HEADERS  += kinect1recoderplayer.h \
    histogramwidget.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h
