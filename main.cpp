#include "mainscene.h"
#include "config.h"
#include <QApplication>
#include <QResource>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();
    QMediaPlayer bgMusic;
    QAudioOutput audioOutput;
    bgMusic.setAudioOutput(&audioOutput);
    bgMusic.setSource(QUrl(MUSIC_PATH));
    bgMusic.play();
    return a.exec();
}
