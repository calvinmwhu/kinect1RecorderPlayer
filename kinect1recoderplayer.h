#ifndef KINECT1RECODERPLAYER_H
#define KINECT1RECODERPLAYER_H

#include <QMainWindow>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE

class Kinect1RecoderPlayer : public QMainWindow
{
    Q_OBJECT

public:
    Kinect1RecoderPlayer(QWidget *parent = 0);
    ~Kinect1RecoderPlayer();

private:
    QTabWidget *tabWidget;
};

#endif // KINECT1RECODERPLAYER_H
