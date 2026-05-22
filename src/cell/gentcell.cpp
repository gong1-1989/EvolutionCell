#include "gentcell.h"

MuscleCell::MuscleCell() {
    m_radius=14;
    m_moveSpeed=130.0;
    m_cellcolor=QColor(210,40,40);
    m_hp=60;
    m_attack=18;
    m_attackRange=40;
    m_attackCd=0.6f;
}
ShieldCell::ShieldCell(){
    m_radius=18;
    m_moveSpeed=90.0;
    m_cellcolor=QColor(40,80,210);
    m_hp=120;
    m_attack=8;
    m_attackRange=35;
    m_attackCd=0.9f;
}
MagicCell::MagicCell(){
    m_radius=12;
    m_moveSpeed=90.0;
    m_cellcolor=QColor(130,40,200);
    m_hp=50;
    m_attack=12;
    m_attackRange=60;
    m_attackCd=0.75f;
}