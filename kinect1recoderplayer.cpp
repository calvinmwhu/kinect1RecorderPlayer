#include <QGridLayout>

#include "kinect1recoderplayer.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "histogramwidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QMediaMetaData>
#include <QtWidgets>

Kinect1RecoderPlayer::Kinect1RecoderPlayer(QWidget *parent)
    : QMainWindow(parent) {
    tabWidget = new QTabWidget;
    tabWidget->addTab(new RecorderTab(), tr("Recorder"));
    tabWidget->addTab(new PlayerTab(), tr("Player"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addWidget(tabWidget);
    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);

    setWindowTitle(tr("Kinect Recorder and Player"));
}

RecorderTab::RecorderTab(QWidget *parent)
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


PlayerTab::PlayerTab(QWidget *parent)
    : QWidget(parent)
    , videoWidget(0)
    , coverLabel(0)
    , slider(0)
#ifndef PLAYER_NO_COLOROPTIONS
    , colorDialog(0)
#endif
{
    //! [create-objs]
        player = new QMediaPlayer(this);
        // owned by PlaylistModel
        playlist = new QMediaPlaylist();
        player->setPlaylist(playlist);
    //! [create-objs]

        connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
        connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
        connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
        connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
        connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
                this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
        connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
        connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
        connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

    //! [2]
        videoWidget = new VideoWidget(this);
        player->setVideoOutput(videoWidget);

        playlistModel = new PlaylistModel(this);
        playlistModel->setPlaylist(playlist);
    //! [2]

        playlistView = new QListView(this);
        playlistView->setModel(playlistModel);
        playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));

        connect(playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));

        slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(0, player->duration() / 1000);

        labelDuration = new QLabel(this);
        connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

        labelHistogram = new QLabel(this);
        labelHistogram->setText("Histogram:");
        histogram = new HistogramWidget(this);
        QHBoxLayout *histogramLayout = new QHBoxLayout;
        histogramLayout->addWidget(labelHistogram);
        histogramLayout->addWidget(histogram, 1);

        probe = new QVideoProbe(this);
        connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), histogram, SLOT(processFrame(QVideoFrame)));
        probe->setSource(player);

        QPushButton *openButton = new QPushButton(tr("Open"), this);

        connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

        PlayerControls *controls = new PlayerControls(this);
        controls->setState(player->state());
        controls->setVolume(player->volume());
        controls->setMuted(controls->isMuted());

        connect(controls, SIGNAL(play()), player, SLOT(play()));
        connect(controls, SIGNAL(pause()), player, SLOT(pause()));
        connect(controls, SIGNAL(stop()), player, SLOT(stop()));
        connect(controls, SIGNAL(next()), playlist, SLOT(next()));
        connect(controls, SIGNAL(previous()), this, SLOT(previousClicked()));
        connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
        connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));
        connect(controls, SIGNAL(changeRate(qreal)), player, SLOT(setPlaybackRate(qreal)));

        connect(controls, SIGNAL(stop()), videoWidget, SLOT(update()));

        connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
                controls, SLOT(setState(QMediaPlayer::State)));
        connect(player, SIGNAL(volumeChanged(int)), controls, SLOT(setVolume(int)));
        connect(player, SIGNAL(mutedChanged(bool)), controls, SLOT(setMuted(bool)));

        fullScreenButton = new QPushButton(tr("FullScreen"), this);
        fullScreenButton->setCheckable(true);

    #ifndef PLAYER_NO_COLOROPTIONS
        colorButton = new QPushButton(tr("Color Options..."), this);
        colorButton->setEnabled(false);
        connect(colorButton, SIGNAL(clicked()), this, SLOT(showColorDialog()));
    #endif

        QBoxLayout *displayLayout = new QHBoxLayout;
        displayLayout->addWidget(videoWidget, 2);
        displayLayout->addWidget(playlistView);

        QBoxLayout *controlLayout = new QHBoxLayout;
        controlLayout->setMargin(0);
        controlLayout->addWidget(openButton);
        controlLayout->addStretch(1);
        controlLayout->addWidget(controls);
        controlLayout->addStretch(1);
        controlLayout->addWidget(fullScreenButton);
    #ifndef PLAYER_NO_COLOROPTIONS
        controlLayout->addWidget(colorButton);
    #endif

        QBoxLayout *layout = new QVBoxLayout;
        layout->addLayout(displayLayout);
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(slider);
        hLayout->addWidget(labelDuration);
        layout->addLayout(hLayout);
        layout->addLayout(controlLayout);
        layout->addLayout(histogramLayout);

        setLayout(layout);

        if (!player->isAvailable()) {
            QMessageBox::warning(this, tr("Service not available"),
                                 tr("The QMediaPlayer object does not have a valid service.\n"\
                                    "Please check the media service plugins are installed."));

            controls->setEnabled(false);
            playlistView->setEnabled(false);
            openButton->setEnabled(false);
    #ifndef PLAYER_NO_COLOROPTIONS
            colorButton->setEnabled(false);
    #endif
            fullScreenButton->setEnabled(false);
        }

        metaDataChanged();

        QStringList arguments = qApp->arguments();
        arguments.removeAt(0);
        addToPlaylist(arguments);
}


void PlayerTab::open()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
    addToPlaylist(fileNames);
}

void PlayerTab::addToPlaylist(const QStringList& fileNames)
{
    foreach (QString const &argument, fileNames) {
        QFileInfo fileInfo(argument);
        if (fileInfo.exists()) {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
                playlist->load(url);
            } else
                playlist->addMedia(url);
        } else {
            QUrl url(argument);
            if (url.isValid()) {
                playlist->addMedia(url);
            }
        }
    }
}

void PlayerTab::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    slider->setMaximum(duration / 1000);
}

void PlayerTab::positionChanged(qint64 progress)
{
    if (!slider->isSliderDown()) {
        slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void PlayerTab::metaDataChanged()
{
    if (player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(player->metaData(QMediaMetaData::Title).toString()));

        if (coverLabel) {
            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void PlayerTab::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if(player->position() <= 5000)
        playlist->previous();
    else
        player->setPosition(0);
}

void PlayerTab::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

void PlayerTab::playlistPositionChanged(int currentItem)
{
    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

void PlayerTab::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void PlayerTab::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void PlayerTab::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void PlayerTab::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void PlayerTab::videoAvailableChanged(bool available)
{
    if (!available) {
        disconnect(fullScreenButton, SIGNAL(clicked(bool)),
                    videoWidget, SLOT(setFullScreen(bool)));
        disconnect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));
        videoWidget->setFullScreen(false);
    } else {
        connect(fullScreenButton, SIGNAL(clicked(bool)),
                videoWidget, SLOT(setFullScreen(bool)));
        connect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));

        if (fullScreenButton->isChecked())
            videoWidget->setFullScreen(true);
    }
#ifndef PLAYER_NO_COLOROPTIONS
    colorButton->setEnabled(available);
#endif
}

void PlayerTab::setTrackInfo(const QString &info)
{
    trackInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void PlayerTab::setStatusInfo(const QString &info)
{
    statusInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void PlayerTab::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

void PlayerTab::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
}

#ifndef PLAYER_NO_COLOROPTIONS
void PlayerTab::showColorDialog()
{
    if (!colorDialog) {
        QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
        brightnessSlider->setRange(-100, 100);
        brightnessSlider->setValue(videoWidget->brightness());
        connect(brightnessSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setBrightness(int)));
        connect(videoWidget, SIGNAL(brightnessChanged(int)), brightnessSlider, SLOT(setValue(int)));

        QSlider *contrastSlider = new QSlider(Qt::Horizontal);
        contrastSlider->setRange(-100, 100);
        contrastSlider->setValue(videoWidget->contrast());
        connect(contrastSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setContrast(int)));
        connect(videoWidget, SIGNAL(contrastChanged(int)), contrastSlider, SLOT(setValue(int)));

        QSlider *hueSlider = new QSlider(Qt::Horizontal);
        hueSlider->setRange(-100, 100);
        hueSlider->setValue(videoWidget->hue());
        connect(hueSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setHue(int)));
        connect(videoWidget, SIGNAL(hueChanged(int)), hueSlider, SLOT(setValue(int)));

        QSlider *saturationSlider = new QSlider(Qt::Horizontal);
        saturationSlider->setRange(-100, 100);
        saturationSlider->setValue(videoWidget->saturation());
        connect(saturationSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setSaturation(int)));
        connect(videoWidget, SIGNAL(saturationChanged(int)), saturationSlider, SLOT(setValue(int)));

        QFormLayout *layout = new QFormLayout;
        layout->addRow(tr("Brightness"), brightnessSlider);
        layout->addRow(tr("Contrast"), contrastSlider);
        layout->addRow(tr("Hue"), hueSlider);
        layout->addRow(tr("Saturation"), saturationSlider);

        QPushButton *button = new QPushButton(tr("Close"));
        layout->addRow(button);

        colorDialog = new QDialog(this);
        colorDialog->setWindowTitle(tr("Color Options"));
        colorDialog->setLayout(layout);

        connect(button, SIGNAL(clicked()), colorDialog, SLOT(close()));
    }
    colorDialog->show();
}
#endif

Kinect1RecoderPlayer::~Kinect1RecoderPlayer()
{

}


