#include <QGridLayout>
#include <QtDebug>
#include <QSharedPointer>

#include "recorder.h"
#include "kinectdetector.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent)
{
    detectCameraButton = new QPushButton("Detect Camera");
    detectCameraButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    connect(detectCameraButton, SIGNAL(clicked()), SLOT(detectCamera()));

    recordButton = new QPushButton("Record");
    recordButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    recordButton->setEnabled(false);


    stopButton = new QPushButton("Stop");
    stopButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    stopButton->setEnabled(false);


    uploadButton = new QPushButton("Upload");
    uploadButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    uploadButton->setEnabled(false);

    logView = new QListView();
    //QLabel* camera_label = new QLabel(tr("cameras"));

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(detectCameraButton);
    gridLayout->addWidget(recordButton);
    gridLayout->addWidget(stopButton);
    gridLayout->addWidget(uploadButton);

   // gridLayout->addWidget(camera_label, 1, 4);
    gridLayout->addWidget(logView, 0, 2, 4, 1);
    setLayout(gridLayout);
}

void Recorder::detectCamera(){
    qDebug() << "detectCamera clicked";
    QSharedPointer<KinectDetector> detector = QSharedPointer<KinectDetector>();
    detector->detectCamera();
}




