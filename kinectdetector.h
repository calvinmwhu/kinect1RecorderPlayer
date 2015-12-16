#ifndef KINECTDETECTOR
#define KINECTDETECTOR

#include <atlbase.h>
#include <map>
#include <iostream>
#include <string>
#include <memory>
#include "NuiApi.h"

typedef std::map<std::string,std::string> SS;

class KinectDetector {
public:
    explicit KinectDetector();
    SS detectCamera();
};

#endif // KINECTDETECTOR

