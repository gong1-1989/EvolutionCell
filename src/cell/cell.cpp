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
    //初始化突变管理器
    m_geneMgr=new GeneMgr(this);
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
    //============= AI行为树：优先级执行 ===============
    //======= 优先级1：致命危险检测（暂占位，后续生态模块补充）======
    //======= 优先级2：能量危机检测 ========
    m_energy-=m_baseEnerygyCost*deltaTime;   //能量守恒（能量守恒法则：所有行为消耗能量）
    //叠加早衰能耗
    if(m_geneMgr->isPrematureAging()){
        qreal addRate=m_geneMgr->getAgingEnergyAdd()/100.0;
        m_energy-=m_baseEnerygyCost*addRate*deltaTime;
    }
    //能量归零👉立即凋零（死亡）
    if(m_energy<=0.0){
        cellDie();
        return;
    }
    //======== 优先级3：生态事件相应（暂占位） =========
    //======== 优先级4：玩家指令相应（叠加引导权重，权限锁控制） =============
    //======== 优先级5：跨层迁徙判定（暂占位） ==============
    //======== 优先级6：自主演化：分化 + 突变 + 水平基因转移 =============
    //6.1 细胞分化（结合玩家分化倾向）
    if(!m_organelleMgr->reachDiffMaxLimit()){
        //随机判定 + 玩家权重影响，AI自主决定是否分化
        qreal diffRand=RandomUtil::randInt(0,10)/10.0;
        if(diffRand<0.02+m_organelleMgr->getDiffTendency()){
            //随机选择分化等级
            int levelRand=RandomUtil::randInt(0,3);
            DiffLevel level=static_cast<DiffLevel>(levelRand);
            m_organelleMgr->doDifferentiate(level);
            //推送分化事件
            EventBus::getInstance()->dispatchEvent(GlobalEvent::EVT_CELL_DIFF);
        }
    }
    //6.2 自发突变（AI自主触发）
    MutateType mutTypr=m_geneMgr->doSpontaneousMutate();
    if(mutTypr!=MutateType::Neutral){
        EventBus::getInstance()->dispatchEvent(GlobalEvent::EVT_GENE_MUTATE);
    }
    //6.3 水平基因转移（异种接触后AI自主触发）
    bool contactOther=false;        //后续群落模块补充真实接触判定
    if(contactOther){
        m_geneMgr->doHorizontalGeneTransfer();
    }
    //============= 优先级7：群落&种间关系（暂占位） ==========
    //============= 优先级8：常规移动 ===============
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

OrganelleMgr* Cell::getOrganelleMgr()const{
    return m_organelleMgr;
}

GeneMgr* Cell::getGeneMgr()const{
    return m_geneMgr;
}