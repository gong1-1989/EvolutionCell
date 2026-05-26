#include "mainwindow.h"
#include "Utils/configreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigReader::getInstance().loadAllConfig();
    MainWindow w;
    w.show();
    return a.exec();
}
