#include <QGridLayout>

#include "kinect1recoderplayer.h"
#include "player.h"
#include "recorder.h"

Kinect1RecoderPlayer::Kinect1RecoderPlayer(QWidget *parent)
    : QMainWindow(parent) {
    tabWidget = new QTabWidget;
    tabWidget->addTab(new Recorder(), tr("Recorder"));
//    tabWidget->addTab(new Player(), tr("Player"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addWidget(tabWidget);

    QWidget *window = new QWidget();
    this->setFixedSize(640,480);
    window->setLayout(mainLayout);
    setCentralWidget(window);

    setWindowTitle(tr("Kinect Recorder and Player"));
}

Kinect1RecoderPlayer::~Kinect1RecoderPlayer()
{

}


