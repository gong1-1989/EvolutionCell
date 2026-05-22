#include "gentcell.h"

MuscleCell::MuscleCell() {
    m_radius=14;
    m_moveSpeed=130.0;
    m_cellcolor=QColor(210,40,40);
}
ShieldCell::ShieldCell(){
    m_radius=18;
    m_moveSpeed=90.0;
    m_cellcolor=QColor(40,80,210);
}
MagicCell::MagicCell(){
    m_radius=12;
    m_moveSpeed=90.0;
    m_cellcolor=QColor(130,40,200);
}