#include "mainwindow.h"
#include "Utils/configreader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool loadOk=ConfigReader::getInstance().loadAllConfig();
    if(!loadOk) qDebug()<<"配置文件加载失败，使用默认值运行！";
    MainWindow w;
    w.show();
    return a.exec();
}
