#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string.h>
#include <QtDebug>

#include "framecompressor.h"

FrameCompressor::FrameCompressor(unsigned long numFrames, int sensorIdx) :
    numFrames_(numFrames),
    sensorIdx_(sensorIdx),
    filePath_("C:\\KinectProjects\\Kinect1RecorderPlayer\\data\\") {
//    compressToDes_ = filePath_+"rgbd_"+std::to_string(sensorIdx_)+".mp4";


//    compressToDes_color = filePath_+"rgb_"+std::to_string(sensorIdx_)+".mp4";
//    compressToDes_depth = filePath_+"d_"+std::to_string(sensorIdx_)+".mp4";

//    colorDir_ = filePath_+"color_frame_"+std::to_string(sensorIdx_);
//    depthDir_ = filePath_+"depth_frame_"+std::to_string(sensorIdx_);

    compressToDes_ = QString("%1rgbd_%2.mp4").arg(filePath_, QString::number(sensorIdx_));
    compressToDes_color = QString("%1rgb_%2.mp4").arg(filePath_, QString::number(sensorIdx_));
    compressToDes_depth = QString("%1d_%2.mp4").arg(filePath_, QString::number(sensorIdx_));
    colorDir_ = QString("%1color_frame_%2").arg(filePath_, QString::number(sensorIdx_));
    depthDir_ = QString("%1depth_frame_%2").arg(filePath_, QString::number(sensorIdx_));
    compress();

}


void FrameCompressor::compress(){

      QString cmd_color = QString("ffmpeg -framerate 30 -i %1\\%d.jpg -c:v libx264 -pix_fmt yuv420p -b 2000k %2 -y")
            .arg(colorDir_, compressToDes_color);

      QString cmd_depth = QString("ffmpeg -framerate 30 -i %1\\%d.jpg -c:v libx264 -pix_fmt yuv420p -b 2000k %2 -y")
            .arg(depthDir_, compressToDes_depth);

      QString cmd_combine = QString("ffmpeg -i %1 -i %2 -filter_complex \"[0:v]pad=iw*2:ih[int];[int][1:v]overlay=W/2:0[vid]\" -map [vid] -c:v libx264 -b:v 2000k -preset veryfast %3 -y")
              .arg(compressToDes_color, compressToDes_depth, compressToDes_);

      system(cmd_color.toStdString().c_str());
      system(cmd_depth.toStdString().c_str());
      system(cmd_combine.toStdString().c_str());
}
