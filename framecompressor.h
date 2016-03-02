#ifndef FRAMECOMPRESSOR
#define FRAMECOMPRESSOR

#include <string>


class FrameCompressor{

public:
    FrameCompressor(unsigned long numFrames, int sensorIdx);

private:
    const unsigned long numFrames_;
    const int sensorIdx_;
    const std::string filePath_;

};



#endif // FRAMECOMPRESSOR

