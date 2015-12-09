#include "kinect1recoderplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kinect1RecoderPlayer w;
    w.show();

    return a.exec();
}
