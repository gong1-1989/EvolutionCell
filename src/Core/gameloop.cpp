#include "gameloop.h"

GameLoop::GameLoop(QObject *parent)
    : QObject{parent}
{
    m_frameTimer=new QTimer(this);
    m_deltaTime=1.0f/FRAME_RATE;
    connect(m_frameTimer,&QTimer::timeout,this,&GameLoop::onFrameTick);
}
void GameLoop::startLoop(){
    m_frameTimer->start(1000/FRAME_RATE);
}
void GameLoop::stopLoop(){
    m_frameTimer->stop();
}
void GameLoop::onFrameTick(){
    emit frameUpdate(m_deltaTime);
}
