#ifndef RECORDER
#define RECORDER

#include <QtWidgets>
#include <QMap>
#include <QString>
#include <QThread>
#include "kinectdetector.h"
#include "framecompressor.h"


QT_BEGIN_NAMESPACE
class QListView;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

enum class Status { DETECTING, READY, RECORDING, STOPPING, COMPRESSING, UPLOADING };

// recorder, for recording kinect color, depth, and skeleton frames
class Recorder : public QWidget
{
    Q_OBJECT

public:
    explicit Recorder(QWidget *parent = 0);

private slots:
    void cameraDetected();
    void cameraInit(QString msg);
    void startRecording();
    void recordingStarted(QString msg);
    void recordingStopped(QString msg, unsigned long numFrames);
    void frameSavedToDisk(QString msg, int sensorIdx);

private:
    QPushButton* detectCameraButton;
    QPushButton* recordButton;
    QPushButton* stopButton;
    QPushButton* uploadButton;
    QListView* logView;
    KinectDetector *kinectDetector_;
    QVector<QThread*> capturerRunners;
    QVector<FrameCompressor*> frameCompressors_;
    Status status;
    unsigned long numFrames_;

private:
    void clearLogView();
    void appendToLogView(QVariant data);
    void statusUpdated();
};

#endif // RECORDER

