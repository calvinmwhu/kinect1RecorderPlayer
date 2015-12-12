#ifndef RECORDER
#define RECORDER

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QListView;
class QLabel;
class QPushButton;
QT_END_NAMESPACE


// recorder, for recording kinect color, depth, and skeleton frames
class Recorder : public QWidget
{
    Q_OBJECT

public:
    explicit Recorder(QWidget *parent = 0);

private:
    QPushButton* detectCameraButton;
    QPushButton* recordButton;
    QPushButton* stopButton;
    QPushButton* uploadButton;
    QListView* camera_view;

};

#endif // RECORDER

