#include "kinectdetector.h"
#include "comutilities.h"
#include "AtlConv.h"

KinectDetector::KinectDetector() {

}

SS KinectDetector::detectCamera(){
    int numSensors;
    SS cameraInfo;
    COM_RESULT_CHECK(NuiGetSensorCount(&numSensors));
    for(int i=0; i<numSensors; i++) {
        CComPtr<INuiSensor> sensor = nullptr;
        COM_RESULT_CHECK(NuiCreateSensorByIndex(i, &sensor));
        CW2A pszConverted(sensor->NuiDeviceConnectionId());
        std::string id(pszConverted);
        std::string status;
        if(sensor->NuiStatus() == S_OK) {
            status = "Connected";
        }else{
            status = TO_STRING(sensor->NuiStatus());
        }
        cameraInfo[id] = status;
    }
    return cameraInfo;
}



