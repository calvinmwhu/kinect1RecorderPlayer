#ifndef KINECTDETECTOR
#define KINECTDETECTOR

#include <QMap>
#include <QVector>
#include "kinectcapturer.h"

typedef QMap<QString, QString> CameraInfo;

class KinectDetector : public QObject {
    Q_OBJECT

public slots:
    void detectCamera();

signals:
    void cameraDetected();

public:
    explicit KinectDetector(QObject* parent);
    QVector<KinectCapturer*> getCapturers();
    CameraInfo cameraInfo();

private:
    QVector<KinectCapturer*> kinectCapturers_;
};

#endif // KINECTDETECTOR

