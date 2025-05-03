#include "mainscene.h"
#include "config.h"
#include <QApplication>
#include <QResource>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainScene w;
    w.show();


    return a.exec();
}
