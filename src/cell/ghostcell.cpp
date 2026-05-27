#include "GhostCell.h"

GhostCell::GhostCell()
    : m_size(10)
    , m_bornTime(0)
    , m_moveSpeed(GameGlobal::Const::GHOST_MOVE_SPEED)
    , m_law(GameGlobal::LAW_FISSION)
{
}

void GhostCell::initGhost(qreal x, qreal y, int size, GameGlobal::LifeLaw lawType)
{
    m_pos = {x, y};
    m_size = size;
    m_law = lawType;

    // 按生命法则设置半透明残影颜色
    switch (lawType)
    {
    case GameGlobal::LAW_FISSION:
        m_color = QColor(255, 80, 80, 120);
        break;
    case GameGlobal::LAW_SYMBIOSIS:
        m_color = QColor(80, 255, 120, 120);
        break;
    case GameGlobal::LAW_ILLUSION:
        m_color = QColor(160, 80, 255, 120);
        break;
    }
}

void GhostCell::update(qreal playerX, qreal playerY, const QList<MonsterCell>& monsterList)
{
    // 1. 寻找最近怪物
    const MonsterCell* target = nullptr;
    qreal minDist = 99999.0;
    for (const auto& mon : monsterList)
    {
        qreal dist = sqrt(pow(mon.getX() - m_pos.x(), 2) + pow(mon.getY() - m_pos.y(), 2));
        if (dist < minDist)
        {
            minDist = dist;
            target = &mon;
        }
    }

    // 2. 优先向怪物移动
    if (target)
    {
        qreal dx = target->getX() - m_pos.x();
        qreal dy = target->getY() - m_pos.y();
        qreal dist = sqrt(dx * dx + dy * dy);
        if (dist > 2.0)
        {
            m_pos.rx() += dx / dist * m_moveSpeed;
            m_pos.ry() += dy / dist * m_moveSpeed;
        }
        return;
    }

    // 3. 无怪物则跟随玩家
    qreal dx = playerX - m_pos.x();
    qreal dy = playerY - m_pos.y();
    qreal dist = sqrt(dx * dx + dy * dy);
    if (dist > 80.0)
    {
        m_pos.rx() += dx / dist * m_moveSpeed;
        m_pos.ry() += dy / dist * m_moveSpeed;
    }
}

bool GhostCell::isLifeExpired(qint64 nowTime) const
{
    return nowTime - m_bornTime > ConfigReader::getInstance().getInt("rollback_ghost_setting", "ghost_life_time", 5000);
}

bool GhostCell::checkAttackHit(const MonsterCell& mon) const
{
    return CollisionUtil::circleCollision(
        m_pos.x(), m_pos.y(), m_size / 2.0,
        mon.getX(), mon.getY(), mon.getSize() / 2.0
        );
}

// 只读接口
qreal GhostCell::getX() const { return m_pos.x(); }
qreal GhostCell::getY() const { return m_pos.y(); }
int GhostCell::getSize() const { return m_size; }
QColor GhostCell::getColor() const { return m_color; }
qint64 GhostCell::getBornTime() const { return m_bornTime; }

void GhostCell::setBornTime(qint64 time) { m_bornTime = time; }

// ===================== 序列化实现 =====================
QJsonObject GhostCell::toJson() const
{
    QJsonObject obj;
    obj["pos_x"]    = m_pos.x();
    obj["pos_y"]    = m_pos.y();
    obj["size"]     = m_size;
    obj["born_time"]= m_bornTime;
    obj["law_type"] = (int)m_law;
    return obj;
}

void GhostCell::fromJson(const QJsonObject& obj)
{
    m_pos.setX(obj["pos_x"].toDouble(0));
    m_pos.setY(obj["pos_y"].toDouble(0));
    m_size      = obj["size"].toInt(10);
    m_bornTime  = obj["born_time"].toVariant().toLongLong();
    m_law       = static_cast<GameGlobal::LifeLaw>(obj["law_type"].toInt(0));

    // 恢复颜色
    switch (m_law)
    {
    case GameGlobal::LAW_FISSION: m_color = QColor(255, 80, 80, 120); break;
    case GameGlobal::LAW_SYMBIOSIS: m_color = QColor(80, 255, 120, 120); break;
    case GameGlobal::LAW_ILLUSION: m_color = QColor(160, 80, 255, 120); break;
    }
}