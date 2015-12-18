#ifndef KINECTCAPTURER
#define KINECTCAPTURER

#include <windows.h>
#include <QThread>
#include <QDebug>
#include <atlbase.h>
#include <iostream>
#include <vector>
#include "NuiApi.h"
#include "comutilities.h"
#include "AtlConv.h"

typedef std::vector<BYTE> VBYTE;

class KinectCapturer : public QObject {
    Q_OBJECT

public:
    static const int                    cBytesPerPixel   = 4;

    static const NUI_IMAGE_RESOLUTION   cDepthResolution = NUI_IMAGE_RESOLUTION_640x480;
    static const NUI_IMAGE_RESOLUTION   cColorResolution = NUI_IMAGE_RESOLUTION_640x480;
    static const long long num_frames  = 1000;

    KinectCapturer(int sensorIdx, const CComPtr<INuiSensor>& sensor);
    QString connectionId();
    QString connectionStatus();

signals:
    void started(QString msg);
    void finished(QString msg);

public slots:
    void start();
    void finish();

private:
    HRESULT initializeSensor();
    HRESULT	convertFrameToPointCloud();

private:
    int sensorIdx_;
    CComPtr<INuiSensor> sensor_;
    bool ended;

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

    std::vector<BYTE> *color_frame;
    std::vector<BYTE> *depth_frame;
    std::vector<BYTE> *color_buffer;
    std::vector<BYTE> *depth_buffer;

};

#endif // KINECTCAPTURER

