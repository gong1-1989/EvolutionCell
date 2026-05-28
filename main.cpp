#include "mainwindow.h"
#include "gamecanvas.h"
//#include "Utils/configreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ConfigReader::getInstance().loadAllConfig();
    GameCanvas g;
    g.show();
    return a.exec();
}
