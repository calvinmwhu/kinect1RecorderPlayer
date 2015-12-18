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
    videowidget.cpp \
    player.cpp \
    recorder.cpp \
    kinectdetector.cpp \
#    comutilities.cpp
    kinectcapturer.cpp

HEADERS  += kinect1recoderplayer.h \
    histogramwidget.h \
    playercontrols.h \
    playlistmodel.h \
    videowidget.h \
    rapidjson/error/en.h \
    rapidjson/error/error.h \
    rapidjson/internal/biginteger.h \
    rapidjson/internal/diyfp.h \
    rapidjson/internal/dtoa.h \
    rapidjson/internal/ieee754.h \
    rapidjson/internal/itoa.h \
    rapidjson/internal/meta.h \
    rapidjson/internal/pow10.h \
    rapidjson/internal/stack.h \
    rapidjson/internal/strfunc.h \
    rapidjson/internal/strtod.h \
    rapidjson/msinttypes/inttypes.h \
    rapidjson/msinttypes/stdint.h \
    rapidjson/allocators.h \
    rapidjson/document.h \
    rapidjson/encodedstream.h \
    rapidjson/encodings.h \
    rapidjson/filereadstream.h \
    rapidjson/filestream.h \
    rapidjson/filewritestream.h \
    rapidjson/memorybuffer.h \
    rapidjson/memorystream.h \
    rapidjson/prettywriter.h \
    rapidjson/rapidjson.h \
    rapidjson/reader.h \
    rapidjson/stringbuffer.h \
    rapidjson/writer.h \
    player.h \
    recorder.h \
    kinectdetector.h \
    comutilities.h \
    kinectcapturer.h
   # stringconverter.h

win32:CONFIG(release, debug|release): LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v1.8/lib/amd64/Kinect10.lib"
else:win32:CONFIG(debug, debug|release): LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v1.8/lib/amd64/Kinect10.lib"

INCLUDEPATH += "C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc"
DEPENDPATH += "C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc"

win32:CONFIG(release, debug|release): LIBS += "C:/opencv/build/x64/vc10/lib/opencv_core249.lib"
else:win32:CONFIG(debug, debug|release): LIBS += "C:/opencv/build/x64/vc10/lib/opencv_core249d.lib"

INCLUDEPATH += "C:/opencv/build/include"
DEPENDPATH += "C:/opencv/build/include"

win32:CONFIG(release, debug|release): LIBS += "C:/opencv/build/x64/vc10/lib/opencv_highgui249.lib"
else:win32:CONFIG(debug, debug|release): LIBS += "C:/opencv/build/x64/vc10/lib/opencv_highgui249d.lib"

message(libraries: $$LIBS)
message(inc paths: $$INCLUDEPATH)

