#include "framecompressor.h"



FrameCompressor::FrameCompressor(unsigned long numFrames, int sensorIdx) :
    numFrames_(numFrames),
    sensorIdx_(sensorIdx),
    filePath_("C:\\KinectProjects\\Kinect1RecorderPlayer\\data\\") {

}
