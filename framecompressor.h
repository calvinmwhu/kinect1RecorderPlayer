#ifndef FRAMECOMPRESSOR
#define FRAMECOMPRESSOR

#include <iostream>
#include <string>
#include <QString>

class FrameCompressor{

public:
    FrameCompressor(unsigned long numFrames, int sensorIdx);

    void compress();

private:
    const unsigned long numFrames_;
    const int sensorIdx_;
    QString filePath_;
    QString compressToDes_;
    QString compressToDes_depth;
    QString compressToDes_color;
    QString colorDir_;
    QString depthDir_;

};



#endif // FRAMECOMPRESSOR

