#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */

#include "framecompressor.h"

FrameCompressor::FrameCompressor(unsigned long numFrames, int sensorIdx) :
    numFrames_(numFrames),
    sensorIdx_(sensorIdx),
    filePath_("C:\\KinectProjects\\Kinect1RecorderPlayer\\data\\") {
    compressToDes_ = filePath_+"rgbd_"+std::to_string(sensorIdx_)+".mp4";
    colorDir_ = filePath_+"color_frame_"+std::to_string(sensorIdx_);
    depthDir_ = filePath_+"depth_frame_"+std::to_string(sensorIdx_);
//    std::cout<<colorDir_<<std::endl;
//    std::cout<<depthDir_<<std::endl;

    compress();

}


void FrameCompressor::compress(){
    //  ffmpeg -framerate 25 -i %d.jpg -c:v libx264 -pix_fmt yuv420p -b 800k out_800.mp4
    std::string cmd_color = "ffmpeg -framerate 30 -i "+
            colorDir_+
            "\\%d.jpg -c:v libx264 -pix_fmt yuv420p -b 2000k "+
            compressToDes_+
            " -y";
    system(cmd_color.c_str());
}
