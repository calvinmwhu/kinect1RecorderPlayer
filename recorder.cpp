#include <QGridLayout>

#include "recorder.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent)
{
    detectCameraButton = new QPushButton("Detect Camera");
    detectCameraButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));

    recordButton = new QPushButton("Record");
    recordButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));


    stopButton = new QPushButton("Stop");
    stopButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));


    uploadButton = new QPushButton("Upload");
    uploadButton->setGeometry(QRect(QPoint(100, 100),
    QSize(200, 50)));

    camera_view = new QListView();
    //QLabel* camera_label = new QLabel(tr("cameras"));

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(detectCameraButton);
    gridLayout->addWidget(recordButton);
    gridLayout->addWidget(stopButton);
    gridLayout->addWidget(uploadButton);

   // gridLayout->addWidget(camera_label, 1, 4);
    gridLayout->addWidget(camera_view, 0, 2, 4, 1);
    setLayout(gridLayout);
}
