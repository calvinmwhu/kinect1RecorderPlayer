#ifndef KINECT1RECODERPLAYER_H
#define KINECT1RECODERPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtWidgets>

#include "videowidget.h"


QT_BEGIN_NAMESPACE
class QTabWidget;
class QAbstractItemView;
class QListView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QVideoProbe;
class QVideoWidget;
QT_END_NAMESPACE


class PlaylistModel;
class HistogramWidget;


// recorder tab, for recording kinect color, depth, and skeleton frames
class RecorderTab : public QWidget
{
    Q_OBJECT

public:
    explicit RecorderTab(QWidget *parent = 0);

private:
    QPushButton* detectCameraButton;
    QPushButton* recordButton;
    QPushButton* stopButton;
    QPushButton* uploadButton;
    QListView* camera_view;

};


// player tab, for playback
class PlayerTab : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerTab(QWidget *parent = 0);


signals:
    void fullScreenChanged(bool fullScreen);

private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void displayErrorMessage();

#ifndef PLAYER_NO_COLOROPTIONS
    void showColorDialog();
#endif
    void addToPlaylist(const QStringList &fileNames);


private:
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    VideoWidget *videoWidget;
    QLabel *coverLabel;
    QSlider *slider;
    QLabel *labelDuration;
    QPushButton *fullScreenButton;
#ifndef PLAYER_NO_COLOROPTIONS
    QPushButton *colorButton;
    QDialog *colorDialog;
#endif

    QLabel *labelHistogram;
    HistogramWidget *histogram;
    QVideoProbe *probe;

    PlaylistModel *playlistModel;
    QAbstractItemView *playlistView;
    QString trackInfo;
    QString statusInfo;
    qint64 duration;
};


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
