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
    //读取细胞基础配置 :/Config/Res/Config/cell_base.json
    QString cellCfgPath=":/Config/Res/Config/cell_base.json";
    //===========细胞实体测试代码=====================
    QJsonDocument cellDoc=JsonParser::loadJsonFile(cellCfgPath);
    QJsonArray cellArr=JsonParser::getJsonArray(cellDoc);
    if(!cellArr.isEmpty())m_cellBaseConfig=cellArr.first().toObject();
    m_testCell=new Cell(this);
    m_testCell->initCell(m_cellBaseConfig);
    //===========end=============
    //读取分化配置 :/Config/Res/Config/organelle_diff.json
    QJsonDocument diffDoc=JsonParser::loadJsonFile(":/Config/Res/Config/organelle_diff.json");
    m_testCell->getOrganelleMgr()->loadDiffConfig(diffDoc);
    //读取突变配置 :/Config/Res/Config/gene_mutation.json
    QJsonDocument geneDoc=JsonParser::loadJsonFile(":/Config/Res/Config/gene_mutation.json");
    m_testCell->getGeneMgr()->loadMutateConfig(geneDoc);
    //读取谱系配置
    LineageTree* lineage=new LineageTree(this);
    QJsonDocument lineageDoc=JsonParser::loadJsonFile(":/Config/Res/Config/lineage_tree.json");
    lineage->loadLineageConfig(lineageDoc);
    //受控引导：设置演化倾向权重
    m_testCell->getOrganelleMgr()->setDiffTendency(0.3);
    m_testCell->getGeneMgr()->setMutateTendency(0.3);

    connect(EventBus::getInstance(),&EventBus::globalEvent,this,[this](const QString& evt,const QVariant& data){
        if(evt==GlobalEvent::EVT_CELL_DEATH){
            qDebug()<<"[事件总线] 收到细胞凋亡事件，坐标："<<data.toPointF();
        }
        if(evt==GlobalEvent::EVT_CELL_DIFF){
            qDebug()<<"[事件总线] 细胞AI自主完成分化";
        }
        if(evt==GlobalEvent::EVT_HGT){
            qDebug()<<"[事件总线] 触发科普弹窗：首次基因转移";
        }
        if(evt==GlobalEvent::EVT_LINEAGE_UPDATE){
            qDebug()<<"[事件总线] 谱系保存成功";
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