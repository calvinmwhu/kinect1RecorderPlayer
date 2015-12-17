#include <QGridLayout>
#include <QtDebug>
#include <QSharedPointer>
#include <iostream>
#include <QStringList>
#include <QStringListModel>

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

void Recorder::detectCamera(){
    QSharedPointer<KinectDetector> detector = QSharedPointer<KinectDetector>();
    auto info = detector->detectCamera();
    clearLogView();
    if(info.size()==0) {
        appendToLogView("No Kinect camera detected!");
    }else{
        QString summary;
        summary.sprintf("%d Kinect Cameras avaliable: ", info.size());
        appendToLogView(summary);
        int i=1;
        for(auto it=info.begin(); it!=info.end(); ++it) {
            QString id = QString::fromStdString(it->first);
            QString status = QString::fromStdString(it->second);
            QString line;
            line.sprintf("camera-%d id: ", i);
            line.append(id);
            line.append("   status: ");
            line.append(status);
            appendToLogView(line);
            i++;
        }
        status = Status::READY;
        statusUpdated();
    }
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
    case Status::STOPPING:
    case Status::COMPRESSING:
        detectCameraButton->setEnabled(false);
        recordButton->setEnabled(false);
        stopButton->setEnabled(false);
        uploadButton->setEnabled(false);
        break;
    }
}



