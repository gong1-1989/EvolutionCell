#include "mainwindow.h"
#include "src/gamemain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    GameMain g;
    g.show();
    return QApplication::exec();
}
