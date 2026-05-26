#include "MonsterCell.h"

MonsterCell::MonsterCell()
    : m_type(GameGlobal::NORMAL)
    , m_size(10)
    , m_speed(1.0)
    , m_dirX(1.0)
    , m_dirY(1.0)
    ,m_isChasing(false)
    ,m_detectRange(150.0)
{
}

void MonsterCell::initMonster(GameGlobal::MonsterType type, int canvasW, int canvasH)
{
    m_type = type;
    int boundOffset = GameGlobal::getBoundOffset();

    // 随机位置（避开画布边界）
    m_pos.setX(RandomUtil::randInt(boundOffset, canvasW - boundOffset));
    m_pos.setY(RandomUtil::randInt(boundOffset, canvasH - boundOffset));

    // 随机基础大小
    int minSize = GameGlobal::getMonMinSize();
    int maxSize = GameGlobal::getMonMaxSize();
    m_size = RandomUtil::randInt(minSize, maxSize);

    // 随机移动方向
    double angle = RandomUtil::randInt(0, 360) * M_PI / 180.0;
    m_dirX = cos(angle);
    m_dirY = sin(angle);

    // 根据类型设置 速度、颜色、体型
    switch (m_type)
    {
    case GameGlobal::NORMAL:
        m_color = QColor(60, 180, 60);
        m_speed = GameGlobal::getMonSpeedRange() * 0.8;
        m_detectRange=150.0;    //普通怪物感知近
        break;

    case GameGlobal::ELITE:
        m_color = QColor(220, 180, 20);
        m_size = RandomUtil::randInt(GameGlobal::getEliteMinSize(), GameGlobal::getEliteMaxSize());
        m_speed = GameGlobal::getMonSpeedRange() * GameGlobal::getEliteSpeedMult();
        m_detectRange=200.0;    //精英怪物感知更远
        break;

    case GameGlobal::SPECIAL:
        m_color = QColor(140, 60, 200);
        m_speed = GameGlobal::getMonSpeedRange() * GameGlobal::getSpecialSpeedMult();
        m_detectRange=250.0;    //特殊怪物感知最远
        break;
    }
}

void MonsterCell::move(int canvasW, int canvasH,qreal playerX,qreal playerY)
{
    int boundOffset = GameGlobal::getBoundOffset();
    qreal halfSize = m_size / 2.0;

    //==========================================================
    //追击模式：朝玩家移动
    if(m_isChasing&&playerX>=0&&playerY>=0){
        qreal dx=playerX-m_pos.x();
        qreal dy=playerY-m_pos.y();
        qreal dist=sqrt(dx*dx+dy*dy);
        if(dist>1.0){
            m_dirX= dx/dist;
            m_dirY=dy/dist;
        }
    }
    //漫游模式：随机反弹移动
    else{
        //随机小概率改变方向（防止一直直线）
        if(RandomUtil::randInt(1,100)<=3){
            double angle=RandomUtil::randInt(0,360)*M_PI/180.0;
            m_dirX=cos(angle);
            m_dirY=sin(angle);
        }
    }
    // 位置更新
    m_pos.rx() += m_dirX * m_speed;
    m_pos.ry() += m_dirY * m_speed;

    // 左右边界反弹
    if (m_pos.x() - halfSize < boundOffset ){
        m_pos.ry()=boundOffset+halfSize;
        m_dirX *= -1.0;
    }else if( m_pos.x() + halfSize > canvasW - boundOffset){
        m_pos.ry()=canvasW-boundOffset-halfSize;
        m_dirX *= -1.0;
    }
    // 上下边界反弹
    if (m_pos.y() - halfSize < boundOffset ){
        m_pos.ry()=boundOffset+halfSize;
        m_dirY *= -1.0;
    }else if( m_pos.y() + halfSize > canvasH - boundOffset){
        m_pos.ry()=canvasH-boundOffset-halfSize;
        m_dirY *= -1.0;
    }
}

// 只读接口实现
qreal MonsterCell::getX() const { return m_pos.x(); }
qreal MonsterCell::getY() const { return m_pos.y(); }
int MonsterCell::getSize() const { return m_size; }
QColor MonsterCell::getColor() const { return m_color; }
GameGlobal::MonsterType MonsterCell::getType() const { return m_type; }

// ===================== 序列化实现 =====================
QJsonObject MonsterCell::toJson() const
{
    QJsonObject obj;
    obj["type"]     = (int)m_type;
    obj["pos_x"]    = m_pos.x();
    obj["pos_y"]    = m_pos.y();
    obj["size"]     = m_size;
    obj["speed"]    = m_speed;
    obj["dir_x"]    = m_dirX;
    obj["dir_y"]    = m_dirY;
    obj["is_chase"] = m_isChasing;
    return obj;
}

void MonsterCell::fromJson(const QJsonObject& obj)
{
    m_type      = static_cast<GameGlobal::MonsterType>(obj["type"].toInt(0));
    m_pos.setX(obj["pos_x"].toDouble(0));
    m_pos.setY(obj["pos_y"].toDouble(0));
    m_size      = obj["size"].toInt(10);
    m_speed     = obj["speed"].toDouble(1.0);
    m_dirX      = obj["dir_x"].toDouble(1.0);
    m_dirY      = obj["dir_y"].toDouble(1.0);
    m_isChasing = obj["id_chase"].toBool(false);
}
//=================== AI接口 ==================
bool MonsterCell::isChasing(){ return m_isChasing; }
void MonsterCell::setChase(bool chase){ m_isChasing=chase; }
qreal MonsterCell::getDetectRange(){ return m_detectRange; }
void MonsterCell::setDetecRange(qreal range){ m_detectRange=range; }