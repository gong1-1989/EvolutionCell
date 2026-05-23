#include "gene.h"

Gene::Gene(GameGlobal::GeneType type):m_type(type) {
    switch (type) {
    case GameGlobal::GENE_RANGE_EXTEND:
        m_name="吞噬延展";
        m_desc="碰撞判定范围提升25%";
        break;
    case GameGlobal::GENE_SPEED_UP:
        m_name="肌体迅捷";
        m_desc="永久移动速度提升15%";
        break;
    case GameGlobal::GENE_GROW_BOOST:
        m_name="裂变强化";
        m_desc="细胞成长效率提升20%";
        break;
    default:
        m_name="无";
        m_desc="";
        break;
    }
}
GameGlobal::GeneType Gene::getType()const{
    return m_type;
}
QString Gene::getName()const{
    return m_name;
}
QString Gene::getDesc()const{
    return m_desc;
}
