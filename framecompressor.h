#ifndef FRAMECOMPRESSOR
#define FRAMECOMPRESSOR

#include <iostream>
#include <string>


class FrameCompressor{

public:
    FrameCompressor(unsigned long numFrames, int sensorIdx);

    void compress();

private:
    const unsigned long numFrames_;
    const int sensorIdx_;
    const std::string filePath_;
    std::string compressToDes_;
    std::string colorDir_;
    std::string depthDir_;

};



#endif // FRAMECOMPRESSOR

