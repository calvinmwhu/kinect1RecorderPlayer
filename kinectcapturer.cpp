#include "kinectcapturer.h"
#include <QCoreApplication>

KinectCapturer::KinectCapturer(int sensorIdx, const CComPtr<INuiSensor> &sensor) :
    sensorIdx_(sensorIdx),
    sensor_(sensor),
    ended(false) {
    DWORD width = 0;
    DWORD height = 0;

    NuiImageResolutionToSize(cDepthResolution, width, height);
    m_depthWidth  = static_cast<LONG>(width);
    m_depthHeight = static_cast<LONG>(height);

    NuiImageResolutionToSize(cColorResolution, width, height);
    m_colorWidth  = static_cast<LONG>(width);
    m_colorHeight = static_cast<LONG>(height);

    color_buffer = new std::vector<BYTE>(KinectCapturer::m_colorWidth*KinectCapturer::m_colorHeight*3*KinectCapturer::num_frames);
    depth_buffer  = new std::vector<BYTE>(KinectCapturer::m_depthWidth*KinectCapturer::m_depthHeight*KinectCapturer::num_frames);
    color_frame = new std::vector<BYTE>(KinectCapturer::m_colorWidth*KinectCapturer::m_colorHeight*3);
    depth_frame = new std::vector<BYTE>(KinectCapturer::m_depthWidth*KinectCapturer::m_depthHeight);

}

HRESULT KinectCapturer::initializeSensor() {
    COM_RESULT_CHECK(sensor_->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX));
    COM_RESULT_CHECK(sensor_->NuiGetCoordinateMapper(&mapper_));
    COM_RESULT_CHECK(sensor_->NuiImageStreamOpen(
                         NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
                         cDepthResolution,
                         0,
                         2,
                         NULL,
                         &m_pDepthStreamHandle));
    COM_RESULT_CHECK(sensor_->NuiImageStreamOpen(
                         NUI_IMAGE_TYPE_COLOR,
                         cColorResolution,
                         0,
                         2,
                         NULL,
                         &m_pColorStreamHandle));
    COM_RESULT_CHECK(sensor_->NuiSkeletonTrackingEnable(NULL, NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE));

    return S_OK;
}

QString KinectCapturer::connectionId(){
    CW2A pszConverted(sensor_->NuiDeviceConnectionId());
    std::string id(pszConverted);
    return QString::fromStdString(id);
}

QString KinectCapturer::connectionStatus(){
        std::string status;
        if(sensor_->NuiStatus() == S_OK) {
            status = "Connected";
        }else{
            status = TO_STRING(sensor_->NuiStatus());
        }
        return QString::fromStdString(status);
}

void KinectCapturer::start() {
    qDebug()<<"Worker::start get called from?: "<<QThread::currentThreadId();
    initializeSensor();
    qDebug()<<"Worker::start get called from?: "<<QThread::currentThreadId();
    QString startMsg;
    startMsg.sprintf("Camera-%d recording", sensorIdx_);
    QString endMsg;
    endMsg.sprintf("Camera-%d stopped recording", sensorIdx_);
    emit started(startMsg);
    while(!ended) {
        QThread::msleep(10);
        QCoreApplication::processEvents();
    }
    emit finished(endMsg);
}

void KinectCapturer::finish() {
    qDebug()<<"Worker::stop get called from?: "<<QThread::currentThreadId();
    ended = true;
}
