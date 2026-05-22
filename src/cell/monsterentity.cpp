#include "monsterentity.h"

MonsterEntity::MonsterEntity() {
    m_pos=QPointF(200,200);
    m_radius=15;
    m_moveSpeed=70.0;
    m_cellcolor=QColor(60,220,80);
    m_hp=30;
}
