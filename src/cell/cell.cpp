#include "cell.h"

Cell::Cell()
    :pos(0,0)
    ,bodySize(0)
    ,moveSpeed(0)
    ,attack(0)
    ,geneAttr(GameGlobal::GeneAttr::None)
    ,evoPoint(0)
    ,mutationLv(GameGlobal::MutationLevel::Normal)
    ,cp(0){
    for(int i=0;i<6;i++) genePoint[i]=0;
}
/**
 * @brief Cell::loadBaseConfig 加载基础配置
 * @param group 数据分类名
 */
void Cell::loadBaseConfig(const QString &group){
    bodySize=cfg.getInt(group,"base_size",10);
    moveSpeed=cfg.getDouble(group,"base_speed",1.0);
    attack=cfg.getDouble(group,"base_attack",1.0);
}
/**
 * @brief Cell::calcCp 计算综合战力CP
 */
void Cell::calcCp(){
    //从配置读取权重值
    qreal wSize=cfg.getDouble("cp_weight","size",0.3);
    qreal wSpeed=cfg.getDouble("cp_weight","speed",0.2);
    qreal wAtk=cfg.getDouble("cp_weight","attack",0.3);
    qreal wGene=cfg.getDouble("cp_weight","gene",0.2);
    //计算基础战力
    cp=bodySize*wSize+moveSpeed*wSpeed+attack*wAtk+getTotalGenePoint()*wGene;
    //从配置读取变异等级倍率
    QString mulGrop="mutation_multi";
    switch (mutationLv) {
    case GameGlobal::MutationLevel::Normal:
        cp*=cfg.getDouble(mulGrop,"normal",1.0);
        break;
    case GameGlobal::MutationLevel::Rare:
        cp*=cfg.getDouble(mulGrop,"rare",1.2);
        break;
    case GameGlobal::MutationLevel::Epic:
        cp*=cfg.getDouble(mulGrop,"eoic",1.5);
        break;
    case GameGlobal::MutationLevel::Legend:
        cp*=cfg.getDouble(mulGrop,"legend",2.0);
        break;
    }
}
/**
 * @brief Cell::move 通用移动方式
 * @param dx 水平移动方向
 * @param dy 垂直移动方向
 * @param canvasW 屏幕宽度
 * @param canvasH 屏幕高度
 */
void Cell::move(qreal dx, qreal dy, int canvasW, int canvasH){
    int bound=cfg.getInt("base_setting","bound_offse",10);
    qreal half=bodySize/2.0;
    pos.rx()+=dx*moveSpeed;
    pos.ry()+=dy*moveSpeed;
    // 左右边界反弹
    if (pos.x() - half < bound ){
        pos.ry()=bound+half;
        dx *= -1.0;
    }else if( pos.x() + half > canvasW - bound){
        pos.ry()=canvasW-bound-half;
        dx *= -1.0;
    }
    // 上下边界反弹
    if (pos.y() - half< bound ){
        pos.ry()=bound+half;
        dy *= -1.0;
    }else if( pos.y() + half > canvasH - bound){
        pos.ry()=canvasH-bound-half;
        dy *= -1.0;
    }
}

void Cell::evolution(){
    QString group="evolution_setting";
    int lv1=cfg.getInt(group,"rare_need",50);
    int lv2=cfg.getInt(group,"epic_nee",150);
    int lv3=cfg.getInt(group,"legend_need",400);
    if(mutationPoint>=lv3) mutationLv=GameGlobal::MutationLevel::Legend;
    else if(mutationPoint>=lv2) mutationLv=GameGlobal::MutationLevel::Epic;
    else if(mutationPoint>=lv1) mutationLv=GameGlobal::MutationLevel::Rare;
    else mutationLv=GameGlobal::MutationLevel::Normal;
    calcCp();
}

int Cell::getTotalGenePoint()const{
    return genePoint[0]+genePoint[1]+genePoint[2]+genePoint[3]+genePoint[4]+genePoint[5];
}
int Cell::getGenePoint(GameGlobal::GeneAttr attr) const{
    return genePoint[(int)attr];
}
bool Cell::costGenePoint(GameGlobal::GeneAttr attr, int cost){
    int idx=(int)attr;
    if(genePoint[idx]>=cost){
        genePoint[idx]-=cost;
        return true;
    }
    return false;
}
// ===================== 序列化实现 =====================
QJsonObject Cell::toJson() const
{
    QJsonObject obj;
    return obj;
}

void Cell::fromJson(const QJsonObject& obj)
{

}