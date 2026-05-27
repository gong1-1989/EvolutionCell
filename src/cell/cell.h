#ifndef CELL_H
#define CELL_H
#include <QPointF>
#include "Core/gameglobal.h"
#include "Utils/configreader.h"
#include "Utils/CollisionUtil.h"
/**
 * @brief 统一生物基类
 * 玩家、怪物、共生体、残影 全部继承此类
 */
class Cell
{
public:
    Cell();
    virtual ~Cell()=default;
    //======================= 通用属性 ==========================
    QPointF pos;                                //位置
    int bodySize;                               //体型
    qreal moveSpeed;                            //移动速度
    qreal attack;                               //攻击
    GameGlobal::GeneAttr geneAttr;              //基因属性
    int evoPoint;                               //进化度
    int mutationPoint;                          //变异度
    GameGlobal::MutationLevel mutationLv;       //变异等级
    int genePoint[6];                           //基因点
    qreal cp;                                   //综合战力

    //========================== 通用方法 ===============================
    //计算综合战力CP
    virtual void calcCp();
    //移动
    virtual void move(qreal dx,qreal dy,int canvasW,int canvasH);
    //进化升级
    virtual void evolution();
    //获取基因点数
    itn getTotalGenePoint()const;
    //获取对于属性基因点
    int getGenePoint(GameGlobal::GeneAttr attr)const;
    //消耗基因点
    bool costGenePoint(GameGlobal::GeneAttr attr,int cost);
    // ===================== 序列化接口（存档专用） =====================
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& obj);
protected:
    //================== 从配置加载基础属性 ============
    void loadBaseConfig(const QString& group);

private:
    ConfigReader& cfg;
};

#endif // CELL_H
