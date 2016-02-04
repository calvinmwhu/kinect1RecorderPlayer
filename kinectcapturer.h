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
    static const long long num_frames  = 100;

    KinectCapturer(int sensorIdx, const CComPtr<INuiSensor>& sensor);
    QString connectionId();
    QString connectionStatus();
    bool connected();
    void initializeSensor();

    void enter()
        { ::EnterCriticalSection(&m_rep); }
    void leave()
        { ::LeaveCriticalSection(&m_rep); }

signals:
    void initialization(QString msg);
    void started(QString msg);
    void finished(QString msg);

public slots:
    void start();
    void finish();

private:
    void	convertFrameToPointCloud();
    void    extractFrames();
    void writeOutput();
    void	cleanUp();

private:
    int sensorIdx_;
    CComPtr<INuiSensor> sensor_;
    bool ended;
    long long frame_count_;

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

    // synchronization
    CRITICAL_SECTION m_rep;
};

#endif // KINECTCAPTURER

