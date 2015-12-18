#include "kinectdetector.h"
#include "comutilities.h"
#include "AtlConv.h"

KinectDetector::KinectDetector(QObject* parent) : QObject(parent) {
}

void KinectDetector::detectCamera(){
    int numSensors;
    COM_RESULT_CHECK(NuiGetSensorCount(&numSensors));
    for(int i=0; i<numSensors; i++) {
        CComPtr<INuiSensor> sensor = nullptr;
        COM_RESULT_CHECK(NuiCreateSensorByIndex(i, &sensor));
        KinectCapturer* kinectCapturer = new KinectCapturer(i,sensor);
//        COM_RESULT_CHECK(kinectCapturer->initializeSensor());
        kinectCapturers_.push_back(kinectCapturer);
    }
    emit cameraDetected();
}

QVector<KinectCapturer*> KinectDetector::getCapturers() {
    return kinectCapturers_;
}

CameraInfo KinectDetector::cameraInfo() {
    CameraInfo info;
    for(int i=0; i<kinectCapturers_.size(); i++){
        info.insert(kinectCapturers_[i]->connectionId(), kinectCapturers_[i]->connectionStatus());
    }
    return info;
}





