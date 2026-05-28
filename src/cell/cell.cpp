#include "cell.h"

Cell::Cell(QObject *parent)
    :QObject(parent)
    ,m_energy(0.0)
    ,m_baseEnerygyCost(0.0)
    ,m_moveSpeed(0.0)
    ,m_cellSize(32)
    ,m_pos(0,0)
    ,m_isAlive(false)
    ,m_walkTime(0.0)
{
    //初始化细胞器分化管理器
    m_organelleMgr=new OrganelleMgr(this);
    //随机出生位置
    int randX=RandomUtil::randInt(m_cellSize,1280-m_cellSize);
    int randY=RandomUtil::randInt(m_cellSize,720-m_cellSize);
    m_pos=QPointF(randX,randY);
    //随机初始移动方向
    randomChangeDir();
}

void Cell::initCell(const QJsonObject &cellCfg){
    m_energy=JsonParser::getDouble(cellCfg,"初始能量值",100.0);
    m_baseEnerygyCost=JsonParser::getDouble(cellCfg,"每秒基础能耗",0.2);
    m_moveSpeed=JsonParser::getDouble(cellCfg,"移动速度",2.5);
    m_cellSize=JsonParser::getInt(cellCfg,"基础体型像素",32);
    m_isAlive=true;
    qDebug()<<"[细胞]:初始化完成，初始能量："<<m_energy;
}

void Cell::updateCell(qreal deltaTime){
    if(!m_isAlive)return;
    m_walkTime+=deltaTime;
    if(m_walkTime>=10.0){
        randomChangeDir();
        m_walkTime=0.0;
    }
    //能量守恒（能量守恒法则：所有行为消耗能量）
    m_energy-=m_baseEnerygyCost*deltaTime;
    //能量归零👉立即凋零（死亡）
    if(m_energy<=0.0){
        cellDie();
        return;
    }
    //自主移动（基础生成行为）
    m_pos.rx()+=m_moveDir.x()*m_moveSpeed*deltaTime;
    m_pos.ry()+=m_moveDir.y()*m_moveSpeed*deltaTime;
    //边界限制（防止移出窗口）
    m_pos=Physics::clampCirclePos(m_pos,m_cellSize,1280,720);
}

QPointF Cell::getCenter()const{
    return m_pos;
}

qreal Cell::getRadius()const{
    return m_cellSize/2.0;
}

bool Cell::isAlive()const{
    return m_isAlive;
}

void Cell::cellDie(){
    m_isAlive=false;
    qDebug()<<"[细胞]：能量耗尽，自然凋亡！";
    EventBus::getInstance()->dispatchEvent(GlobalEvent::EVT_CELL_DEATH,m_pos);
}

void Cell::normalizeDir(){
    qreal len=qSqrt(m_moveDir.x()*m_moveDir.x()+m_moveDir.y()*m_moveDir.y());
    if(len>0.001) m_moveDir/len;
    else {
        //向量太小时，默认向右
        m_moveDir.setX(1.0);
        m_moveDir.setY(0.0);
    }
}

void Cell::randomChangeDir(){
    m_moveDir.setX(RandomUtil::randInt(-1,1));
    m_moveDir.setY(RandomUtil::randInt(-1,1));
    normalizeDir();
}

// ===================== 序列化实现 =====================
QJsonObject Cell::toJson() const
{
   /* QJsonObject obj;
    return obj;*/
    return QJsonObject();
}
