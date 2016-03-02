#ifndef KINECTCAPTURER
#define KINECTCAPTURER

#include <windows.h>
#include <QThread>
#include <QDebug>
#include <atlbase.h>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "NuiApi.h"
#include "comutilities.h"
#include "AtlConv.h"

#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output

typedef std::vector<BYTE> VBYTE;

class KinectCapturer : public QObject {
    Q_OBJECT

public:
    static const int                    cBytesPerPixel   = 4;

    static const NUI_IMAGE_RESOLUTION   cDepthResolution = NUI_IMAGE_RESOLUTION_640x480;
    static const NUI_IMAGE_RESOLUTION   cColorResolution = NUI_IMAGE_RESOLUTION_640x480;
    static const long long num_frames  = 100;

    KinectCapturer(int sensorIdx, const CComPtr<INuiSensor>& sensor);
    QString connectionId();
    QString connectionStatus();
    bool connected();
    void initializeSensor();

signals:
    void initialization(QString msg);
    void started(QString msg);
    void finished(QString msg, unsigned long frameCount);
    void frameSavedToDisk(QString msg, int sensorIdx);

public slots:
    void start();
    void finish();

private:
    void convertFrameToPointCloud();
    void extractFrames();
    void writeSkeletonToJSON(NUI_SKELETON_DATA* skeleton, rapidjson::Writer<rapidjson::StringBuffer>& w );
    void writeOutput();
    void cleanUp();

private:
    std::string filePath_;
    int sensorIdx_;
    CComPtr<INuiSensor> sensor_;
    bool ended;
    unsigned long frame_count_;

    CComPtr<INuiCoordinateMapper> mapper_;
    HANDLE                  m_pDepthStreamHandle;
    HANDLE					m_hLastFrameEvent;
    HANDLE                  m_pColorStreamHandle;
    HANDLE					m_hEventThread;
    HANDLE					m_hStopStreamEventThread;
    HANDLE					m_hPointCloudEvent;

    LONG                    m_depthWidth;
    LONG                    m_depthHeight;

    LONG                    m_colorWidth;
    LONG                    m_colorHeight;

    BYTE *color_frame;
    BYTE *depth_frame;
    BYTE *color_buffer;
    BYTE *depth_buffer;
    std::queue<std::string> skeletonQueue_;

};

#endif // KINECTCAPTURER

