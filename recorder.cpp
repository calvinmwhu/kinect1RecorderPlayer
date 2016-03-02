#include <QGridLayout>
#include <QtDebug>
#include <QSharedPointer>
#include <iostream>
#include <QStringList>
#include <QStringListModel>

#include "recorder.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent)
{
    detectCameraButton = new QPushButton("Detect Camera");
    detectCameraButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));

    kinectDetector_ = new KinectDetector(this);
    connect(kinectDetector_, SIGNAL(cameraDetected()), SLOT(cameraDetected()));
    connect(detectCameraButton, SIGNAL(clicked()), kinectDetector_, SLOT(detectCamera()));

    recordButton = new QPushButton("Record");
    recordButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    recordButton->setEnabled(false);    
    connect(recordButton, SIGNAL(clicked()), SLOT(startRecording()));

    stopButton = new QPushButton("Stop");
    stopButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    stopButton->setEnabled(false);

    uploadButton = new QPushButton("Upload");
    uploadButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));
    uploadButton->setEnabled(false);

    logView = new QListView();
    QStringListModel *model = new QStringListModel();
    logView->setModel(model);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(detectCameraButton);
    gridLayout->addWidget(recordButton);
    gridLayout->addWidget(stopButton);
    gridLayout->addWidget(uploadButton);

    gridLayout->addWidget(logView, 0, 2, 4, 1);
    setLayout(gridLayout);

    status = Status::DETECTING;
    statusUpdated();

}

void Recorder::cameraDetected() {
    CameraInfo info = kinectDetector_->cameraInfo();
    if(info.size()==0) {
        appendToLogView("No Kinect camera detected!");
    }else{
        QString summary;
        summary.sprintf("%d Kinect Cameras avaliable: ", info.size());
        appendToLogView(summary);
        int i=0;
        for(auto it=info.begin(); it!=info.end(); ++it) {
            QString line;
            line.sprintf("camera-%d id: ", i);
            line.append(it.key());
            line.append("   status: ");
            line.append(it.value());
            appendToLogView(line);
            i++;
        }
        status = Status::READY;
        statusUpdated();
    }
    if(status == Status::READY) {
        QVector<KinectCapturer*> capturers = kinectDetector_->getCapturers();
        for(int i=0; i<capturers.size(); i++) {
            KinectCapturer* capturer = capturers[i];
            if(capturer->connected()) {
                QThread* capturerRunner = new QThread(this);
                capturer->initializeSensor();

                capturer->moveToThread(capturerRunner);
                connect(capturer, SIGNAL(initialization(QString)), SLOT(cameraInit(QString)));
                connect(capturer, SIGNAL(started(QString)), SLOT(recordingStarted(QString)));
                connect(capturer, SIGNAL(finished(QString)), capturerRunner, SLOT(quit()));
                connect(capturer, SIGNAL(finished(QString)), SLOT(recordingStopped(QString)));
                connect(capturer, SIGNAL(frameSavedToDisk(QString)), SLOT(frameSavedToDisk(QString)));
                connect(stopButton, SIGNAL(clicked()), capturer, SLOT(finish()));
                connect(capturerRunner, SIGNAL(started()), capturer, SLOT(start()));

                capturerRunners.push_back(capturerRunner);
            }
        }
    }
}

void Recorder::cameraInit(QString msg) {
    appendToLogView(msg);
}

void Recorder::startRecording() {
    for(int i=0; i<capturerRunners.size(); i++) {
        capturerRunners[i]->start();
    }
}

void Recorder::recordingStarted(QString msg) {
    status = Status::RECORDING;
    statusUpdated();
    appendToLogView(msg);
}

void Recorder::recordingStopped(QString msg) {
//    qDebug()<<"From main thread: "<<QThread::currentThreadId();
    appendToLogView(msg);
    status = Status::STOPPING;
    statusUpdated();
}

void Recorder::frameSavedToDisk(QString msg) {
     appendToLogView(msg);
     status = Status::COMPRESSING;
     statusUpdated();
}


void Recorder::clearLogView() {
    logView->model()->removeRows(0, logView->model()->rowCount());
}

void Recorder::appendToLogView(QVariant data) {
    int row = logView->model()->rowCount();
    logView->model()->insertRow(row);
    QModelIndex index = logView->model()->index(row,0);
    logView->model()->setData(index, data);
}

void Recorder::statusUpdated() {
    switch (status) {
    case Status::DETECTING:
        detectCameraButton->setEnabled(true);
        recordButton->setEnabled(false);
        stopButton->setEnabled(false);
        uploadButton->setEnabled(false);
        break;
    case Status::READY:
        detectCameraButton->setEnabled(false);
        recordButton->setEnabled(true);
        stopButton->setEnabled(false);
        uploadButton->setEnabled(false);
        break;
    case Status::RECORDING:
        detectCameraButton->setEnabled(false);
        recordButton->setEnabled(false);
        stopButton->setEnabled(true);
        uploadButton->setEnabled(false);
        break;
    case Status::STOPPING:
    case Status::COMPRESSING:
        detectCameraButton->setEnabled(false);
        recordButton->setEnabled(false);
        stopButton->setEnabled(false);
        uploadButton->setEnabled(false);
        break;
    }
}
