#include "SymbiosisCell.h"

SymbiosisCell::SymbiosisCell()
    : m_mode(GameGlobal::SYMBIO_PERM)
    , m_sourceType(GameGlobal::NORMAL)
    , m_size(10)
    , m_bornTime(0)
    , m_lastAttackTime(0)
    , m_moveSpeed(GameGlobal::Const::SYM_MOVE_SPEED)
{
}

void SymbiosisCell::initSymbiosis(GameGlobal::MonsterType monType, qreal x, qreal y)
{
    m_sourceType = monType;
    m_pos = {x, y};

    // 颜色/尺寸规则（可后续迁移至配置）
    switch (monType)
    {
    case GameGlobal::NORMAL:
        m_size = 10;
        m_color = QColor(100, 200, 100);
        break;
    case GameGlobal::ELITE:
        m_size = 14;
        m_color = QColor(255, 220, 60);
        break;
    case GameGlobal::SPECIAL:
        m_size = 12;
        m_color = QColor(150, 60, 230);
        break;
    }
}

void SymbiosisCell::update(qreal playerX, qreal playerY, qreal followRange, const QList<MonsterCell>& monsterList)
{
    // 1. 寻找最近怪物
    const MonsterCell* nearestMon = nullptr;
    qreal minDist = 999999.0;
    for (const auto& mon : monsterList)
    {
        qreal dist = sqrt(pow(mon.getX() - m_pos.x(), 2) + pow(mon.getY() - m_pos.y(), 2));
        if (dist < minDist)
        {
            minDist = dist;
            nearestMon = &mon;
        }
    }

    // 2. 优先向怪物移动
    if (nearestMon)
    {
        qreal dx = nearestMon->getX() - m_pos.x();
        qreal dy = nearestMon->getY() - m_pos.y();
        qreal dist = sqrt(dx * dx + dy * dy);
        if (dist > 1)
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
    if (dist > followRange)
    {
        m_pos.rx() += dx / dist * m_moveSpeed;
        m_pos.ry() += dy / dist * m_moveSpeed;
    }
}

bool SymbiosisCell::isTempExpired(qint64 currentTime) const
{
    if (m_mode != GameGlobal::SYMBIO_TEMP)
        return false;
    return currentTime - m_bornTime > GameGlobal::getTempSymbiosisDuration();
}

bool SymbiosisCell::canAttack(qint64 currentTime) const
{
    int cd = GameGlobal::getSymAttackCdMs();
    return (currentTime - m_lastAttackTime) >= cd;
}

void SymbiosisCell::onAttackTriggered(qint64 currentTime)
{
    m_lastAttackTime = currentTime;
}

// 只读接口
qreal SymbiosisCell::getX() const { return m_pos.x(); }
qreal SymbiosisCell::getY() const { return m_pos.y(); }
int SymbiosisCell::getSize() const { return m_size; }
QColor SymbiosisCell::getColor() const { return m_color; }
GameGlobal::SymbiosisMode SymbiosisCell::getMode() const { return m_mode; }
GameGlobal::MonsterType SymbiosisCell::getSourceType() const { return m_sourceType; }
qint64 SymbiosisCell::getBornTime() const { return m_bornTime; }
qint64 SymbiosisCell::getLastAttackTime() const { return m_lastAttackTime; }

// 设置接口
void SymbiosisCell::setSymbiosisMode(GameGlobal::SymbiosisMode mode) { m_mode = mode; }
void SymbiosisCell::setBornTime(qint64 time) { m_bornTime = time; }
void SymbiosisCell::setLastAttackTime(qint64 time) { m_lastAttackTime = time; }

// ===================== 序列化实现 =====================
QJsonObject SymbiosisCell::toJson() const
{
    QJsonObject obj;
    obj["mode"]         = (int)m_mode;
    obj["source_type"]  = (int)m_sourceType;
    obj["pos_x"]        = m_pos.x();
    obj["pos_y"]        = m_pos.y();
    obj["born_time"]    = m_bornTime;
    obj["last_attack_time"] = m_lastAttackTime;
    return obj;
}

void SymbiosisCell::fromJson(const QJsonObject& obj)
{
    m_mode = static_cast<GameGlobal::SymbiosisMode>(obj["mode"].toInt(0));
    m_sourceType = static_cast<GameGlobal::MonsterType>(obj["source_type"].toInt(0));
    m_pos.setX(obj["pos_x"].toDouble(0));
    m_pos.setY(obj["pos_y"].toDouble(0));
    m_bornTime = obj["born_time"].toVariant().toLongLong();
    m_lastAttackTime = obj["last_attack_time"].toVariant().toLongLong();
}