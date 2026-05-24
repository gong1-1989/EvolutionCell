#include "gamecanvas.h"
#include "Core/gamerender.h"

GameCanvas::GameCanvas(QWidget *parent)
    : QWidget{parent},k_w(false),k_s(false),k_a(false)
    ,k_d(false),isinitializd(false)
{
    refreshTimer=new QTimer(this);
    refreshTimer->setInterval(16);
    connect(refreshTimer,&QTimer::timeout,this,&GameCanvas::updateCellPos);
    refreshTimer->start();
    setFocusPolicy(Qt::StrongFocus);//开启键盘焦点，才能接收按键
}

void GameCanvas::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    if(!isinitializd){
        m_gameMgr.initScene(width(),height());
        isinitializd=true;
    }
    QPainter p(this);
    GameRender::drawAll(&p,m_gameMgr,rect());
}
void GameCanvas::updateCellPos(){
    m_gameMgr.frameUpdate(k_w,k_a,k_s,k_d,width(),height());
    update();
}

void GameCanvas::reserGameCanvas(){
    m_gameMgr.reserNewGame();
    isinitializd=false;
    update();
}
void GameCanvas::loadGameBySlot(const QString &path){
    m_gameMgr.reserNewGame();
    m_gameMgr.loadFromSaveSlot(path);
    isinitializd=false;
    update();
}
void GameCanvas::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        k_w=true;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        k_s=true;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        k_a=true;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        k_d=true;
        break;
    case Qt::Key_Escape:
        if(m_gameMgr.getGameState()==GameGlobal::RUNING) m_gameMgr.pauseGame();
        else m_gameMgr.resumeGame();
        break;
    case Qt::Key_1:
        if(m_gameMgr.getGameState()==GameGlobal::RUNING) m_gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_LIGHT);
        break;
    case Qt::Key_2:
        if(m_gameMgr.getGameState()==GameGlobal::RUNING) m_gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_DEEP);
        break;
    case Qt::Key_3:
        if(m_gameMgr.getGameState()==GameGlobal::RUNING) m_gameMgr.executeBodyDecompose(GameGlobal::DECOMPOSE_FULL);
        break;
    case Qt::Key_T:
        m_gameMgr.saveToSaveSlot();
        break;
    default:
        break;
    }
}
void GameCanvas::keyReleaseEvent(QKeyEvent *e){
    switch (e->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        k_w=false;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        k_s=false;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        k_a=false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        k_d=false;
        break;
    default:
        break;
    }
}