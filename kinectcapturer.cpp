#include "kinectcapturer.h"
#include <QCoreApplication>
#include "opencv2\opencv.hpp"

KinectCapturer::KinectCapturer(int sensorIdx, const CComPtr<INuiSensor> &sensor) :
    filePath_("C:\\KinectProjects\\Kinect1RecorderPlayer\\data\\"),
    sensorIdx_(sensorIdx),
    sensor_(sensor),
    ended(false),
    frame_count_(0){
    DWORD width = 0;
    DWORD height = 0;

    NuiImageResolutionToSize(cDepthResolution, width, height);
    m_depthWidth  = static_cast<LONG>(width);
    m_depthHeight = static_cast<LONG>(height);

    NuiImageResolutionToSize(cColorResolution, width, height);
    m_colorWidth  = static_cast<LONG>(width);
    m_colorHeight = static_cast<LONG>(height);

    color_buffer = new BYTE[KinectCapturer::m_colorWidth*KinectCapturer::m_colorHeight*3*KinectCapturer::num_frames];
    depth_buffer  = new BYTE[KinectCapturer::m_depthWidth*KinectCapturer::m_depthHeight*KinectCapturer::num_frames];
    color_frame = new BYTE[KinectCapturer::m_colorWidth*KinectCapturer::m_colorHeight*3];
    depth_frame = new BYTE[KinectCapturer::m_depthWidth*KinectCapturer::m_depthHeight];

}

void KinectCapturer::initializeSensor() {
    qDebug()<<"capturer::initialize get called from: "<<QThread::currentThreadId();

    QString initingMsg;
    QString initedMsg;

    // initialize sensor
    initingMsg.sprintf("initializing Camera-%d ... ", sensorIdx_);
    qDebug()<<initingMsg;
//    // initialization logic:
    COM_RESULT_CHECK(sensor_->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX));
    COM_RESULT_CHECK(sensor_->NuiGetCoordinateMapper(&mapper_));
    COM_RESULT_CHECK(sensor_->NuiImageStreamOpen(
                         NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
                         cDepthResolution,
                         0,
                         2,
                         nullptr,
                         &m_pDepthStreamHandle));
    COM_RESULT_CHECK(sensor_->NuiImageStreamOpen(
                         NUI_IMAGE_TYPE_COLOR,
                         cColorResolution,
                         0,
                         2,
                         nullptr,
                         &m_pColorStreamHandle));

    m_hStopStreamEventThread = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hLastFrameEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    COM_RESULT_CHECK(sensor_->NuiSetFrameEndEvent(m_hLastFrameEvent, 0));

    initedMsg.sprintf("Camera-%d initialized", sensorIdx_);
    qDebug()<<initedMsg;
}

void KinectCapturer::extractFrames() {
    HANDLE events[] = {m_hStopStreamEventThread, m_hLastFrameEvent};
    while(!ended && frame_count_<num_frames) {
//        qDebug()<<"new command1  \n ";

        DWORD ret = WaitForMultipleObjects(ARRAYSIZE(events), events, FALSE, INFINITE);
//        qDebug()<<"new command2  \n ";

        if (WAIT_OBJECT_0 == ret)
            break;
        if (WAIT_OBJECT_0 + 1 == ret) {
//            qDebug()<<"get frame: "<<frame_count_;
            ResetEvent(m_hLastFrameEvent);
            convertFrameToPointCloud();
        }
        QCoreApplication::processEvents();
    }
    writeOutput();
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

bool KinectCapturer::connected() {
    return sensor_->NuiStatus() == S_OK;
}

void KinectCapturer::start() {
    qDebug()<<"capturer::start get called from: "<<QThread::currentThreadId();
    QString startMsg;
    QString endMsg;
    // begin recording/extracting frames
    startMsg.sprintf("Camera-%d recording in progress ...", sensorIdx_);
    emit started(startMsg);
    extractFrames();
    // finishing
    endMsg.sprintf("Camera-%d recording ended", sensorIdx_);
    emit finished(endMsg);
}

void KinectCapturer::finish() {
    qDebug()<<"Worker::stop get called from?: "<<QThread::currentThreadId();
    ended = true;
}

void KinectCapturer::writeOutput() {
    std::string FILE_PREFIX="C:\\KinectProjects\\Kinect1RecorderPlayer\\data\\";
    qDebug()<<"frames location: "<<QString::fromStdString(FILE_PREFIX);
    for (int i = 0; i< frame_count_; i++){
//       qDebug()<<"capturer: outputing frame "<<i;
       std::string filename = FILE_PREFIX + "depth_frame_"+ std::to_string(static_cast<unsigned long long>(sensorIdx_));
//       qDebug()<<"filename "<<filename;
      cv::Mat color_image = cv::Mat(KinectCapturer::m_colorHeight, KinectCapturer::m_colorWidth, CV_8UC3, KinectCapturer::color_buffer + i * 640*480*3, cv::Mat::AUTO_STEP);
      cv::Mat depth_image = cv::Mat(KinectCapturer::m_depthHeight, KinectCapturer::m_depthWidth, CV_8UC1, KinectCapturer::depth_buffer + i * 640*480, cv::Mat::AUTO_STEP);
      cv::imwrite(FILE_PREFIX + "depth_frame_"+ std::to_string(static_cast<unsigned long long>(sensorIdx_)) +"\\" + std::to_string(static_cast<unsigned long long>(i)) + ".jpg", depth_image);
      cv::imwrite(FILE_PREFIX + "color_frame_"+ std::to_string(static_cast<unsigned long long>(sensorIdx_)) +"\\" + std::to_string(static_cast<unsigned long long>(i)) + ".jpg", color_image);
    }
}

void KinectCapturer::convertFrameToPointCloud() {
    NUI_IMAGE_FRAME imageFrame;
    NUI_IMAGE_FRAME depthFrame;
    NUI_SKELETON_FRAME skeletonFrame;
    COM_RESULT_CHECK(sensor_->NuiSkeletonGetNextFrame(0, &skeletonFrame));
    COM_RESULT_CHECK(sensor_->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame));
    COM_RESULT_CHECK(sensor_->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &depthFrame));

    CComPtr<INuiFrameTexture> texture;
    BOOL aaa = false;
    COM_RESULT_CHECK(sensor_->NuiImageFrameGetDepthImagePixelFrameTexture(
                         m_pDepthStreamHandle, &depthFrame, &aaa, &texture));


    NUI_LOCKED_RECT LockedColorRect;
    NUI_LOCKED_RECT LockedDepthRect;

    COM_RESULT_CHECK(imageFrame.pFrameTexture->LockRect(0, &LockedColorRect, NULL, 0));
    COM_RESULT_CHECK(texture->LockRect(0, &LockedDepthRect, NULL, 0));

    int safeWidth = m_colorWidth - 1, safeHeight = m_colorHeight - 1;
    NUI_DEPTH_IMAGE_PIXEL* depth_data = (NUI_DEPTH_IMAGE_PIXEL*) LockedDepthRect.pBits;

    static const float bad_point = std::numeric_limits<float>::quiet_NaN ();

    for (int i = 0; i != m_depthWidth * m_depthHeight; i++) {
         color_frame[3*i] = 0x00;
         color_frame[3*i + 1] = 0xff;
         color_frame[3*i + 2] = 0x00;
         depth_frame[i] = 0xff;
    }

    NUI_COLOR_IMAGE_POINT *pColorPoints = new NUI_COLOR_IMAGE_POINT[640*480];
    mapper_->MapDepthFrameToColorFrame(cDepthResolution, 640*480, depth_data, NUI_IMAGE_TYPE_COLOR, cColorResolution, 640*480, pColorPoints);
    cv::Mat image = cv::Mat(m_colorHeight, m_colorWidth, CV_8UC4, LockedColorRect.pBits, cv::Mat::AUTO_STEP);
    for(int j = 0; j < m_depthHeight; j++) {
        for(int i = 0; i < m_depthWidth; i++, depth_data++) {
            USHORT player = depth_data->playerIndex;
            USHORT depthValue = depth_data->depth;
            if (!( player == 0 ||
                pColorPoints[j*m_depthWidth+i].x < 0 || pColorPoints[j*m_depthWidth+i].x > safeWidth || pColorPoints[j*m_depthWidth+i].y < 0 || pColorPoints[j*m_depthWidth+i].y > safeHeight))

            {
                depth_frame[j*m_depthWidth+i] = (BYTE) ((depthValue-1024)/8);
                cv::Vec4b color = image.at<cv::Vec4b>(pColorPoints[j*m_depthWidth+i].y,pColorPoints[j*m_depthWidth+i].x);
                color_frame[3*(j*m_depthWidth+i)] = color[0];
                color_frame[3*(j*m_depthWidth+i)+1] = color[1];
                color_frame[3*(j*m_depthWidth+i)+2] = color[2];
            }

        }
    }

    COM_RESULT_CHECK(imageFrame.pFrameTexture->UnlockRect(0));
    COM_RESULT_CHECK(texture->UnlockRect(0));
    COM_RESULT_CHECK(sensor_->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame));
    COM_RESULT_CHECK(sensor_->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &depthFrame));

// will figure out a way to better represent skeleton data
    NUI_SKELETON_DATA* pSkeletonData = skeletonFrame.SkeletonData;
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writeSkeletonToJSON(pSkeletonData, writer);
    skeletonQueue.push(s.GetString());

    memcpy(color_buffer + frame_count_ * 640*480*3, color_frame, 640*480*3);
    memcpy(depth_buffer + frame_count_ * 640*480, depth_frame, 640*480);
    frame_count_++;
}

void KinectCapturer::writeSkeletonToJSON(NUI_SKELETON_DATA* skeleton, rapidjson::Writer<rapidjson::StringBuffer>& w ) {
    w.StartObject();
    w.String("eTrackingState");
    w.Uint(skeleton->eTrackingState);
    w.String("dwTrackingID");
    w.Uint(skeleton->dwTrackingID);
    w.String("dwEnrollmentIndex");
    w.Uint(skeleton->dwEnrollmentIndex);
    w.String("Position");
    w.StartArray();
    w.Double(skeleton->Position.x);
    w.Double(skeleton->Position.y);
    w.Double(skeleton->Position.z);
    w.EndArray();
    w.String("SkeletonPositions");
    w.StartArray();
    for (int i =0; i!=NUI_SKELETON_POSITION_COUNT; i++) {
        w.StartArray();
        w.Double(skeleton->SkeletonPositions[i].x);
        w.Double(skeleton->SkeletonPositions[i].y);
        w.Double(skeleton->SkeletonPositions[i].z);
        w.EndArray();
    }
    w.EndArray();
    w.String("eSkeletonPositionTrackingState");
    w.StartArray();
    for (int i =0; i!=NUI_SKELETON_POSITION_COUNT; i++) {
        w.Uint(skeleton->eSkeletonPositionTrackingState[i]);
    }
    w.EndArray();
    w.EndObject();
}


void KinectCapturer::cleanUp(){
}

