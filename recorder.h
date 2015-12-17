#ifndef RECORDER
#define RECORDER

#include <QtWidgets>
#include <QMap>
#include <QString>

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
    void detectCamera();

private:
    QPushButton* detectCameraButton;
    QPushButton* recordButton;
    QPushButton* stopButton;
    QPushButton* uploadButton;
    QListView* logView;
    Status status;

private:
    void clearLogView();
    void appendToLogView(QVariant data);
    void statusUpdated();

};

#endif // RECORDER

