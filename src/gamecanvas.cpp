#include "gamecanvas.h"

GameCanvas::GameCanvas(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("进化亿重奏：细胞纪元");
    this->resize(m_winWidth,m_winHeight);
    this->setMinimumSize(800,800);
    m_gameTimer=new GameTimer(this);
    connect(m_gameTimer,&GameTimer::frameUpdate,this,&GameCanvas::onFrameUpdate);
    if(m_gameTimer) m_gameTimer->startGameLoop();
    qDebug()<<"【系统】游戏主窗口&60FPS帧循环 初始化完成";
    //读取细胞基础配置 Res/Config/cell_base.josn
    QString cellCfgPath=":/Config/Res/Config/cell_base.json";
    QJsonDocument cellDoc=JsonParser::loadJsonFile(cellCfgPath);
    QJsonArray cellArr=JsonParser::getJsonArray(cellDoc);
    //===========细胞实体测试代码=====================
    if(!cellArr.isEmpty())m_cellBaseConfig=cellArr.first().toObject();
    m_testCell=new Cell(this);
    m_testCell->initCell(m_cellBaseConfig);
    connect(EventBus::getInstance(),&EventBus::globalEvent,this,[this](const QString& evt,const QVariant& data){
        if(evt==GlobalEvent::EVT_CELL_DEATH){
            qDebug()<<"[事件总线] 收到细胞凋亡事件，坐标："<<data.toPointF();
        }
    });
    qDebug()<<"【系统】 全底层模块整合完成";
}

GameCanvas::~GameCanvas(){
    if(m_gameTimer) m_gameTimer->stopGameLoop();
}

void GameCanvas::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    m_painter=&painter;
    painter.fillRect(this->rect(),QColor(20,30,60));
    //后续：细胞、特效、UI均在此分层绘制
    if(m_testCell&&m_testCell->isAlive()){
        painter.setBrush(QColor(249,208,118));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_testCell->getCenter(),m_testCell->getRadius(),m_testCell->getRadius());
    }
}

void GameCanvas::onFrameUpdate(){
    qreal deltaTime=1.0/60.0;
    if(m_testCell&&m_testCell->isAlive()) m_testCell->updateCell(deltaTime);
    this->update();
}