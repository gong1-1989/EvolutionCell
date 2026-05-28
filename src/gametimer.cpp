#include "gametimer.h"
#include <QDebug>

GameTimer::GameTimer(QObject *parent)
    : QObject{parent},m_targetFps(60)
{
    m_frameTimer=new QTimer(this);
    int frameInterval=1000/m_targetFps;
    m_frameTimer->setInterval(frameInterval);
    connect(m_frameTimer,&QTimer::timeout,this,&GameTimer::frameUpdate);
}

void GameTimer::startGameLoop(){
    if(!m_frameTimer){
        qDebug()<<"[GameTimer]:m_frameTimer未初始化！";
        return;
    }
    if(!m_frameTimer->isActive()) m_frameTimer->start();
}

void GameTimer::stopGameLoop(){
    if(m_frameTimer&&!m_frameTimer->isActive()) m_frameTimer->stop();
}