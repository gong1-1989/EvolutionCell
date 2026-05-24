#ifndef GAMEGLOBAL_H
#define GAMEGLOBAL_H
//全局常量
#include <QtMath>
#include <QColor>
#include "Utils/configreader.h"
namespace GameGlobal {
//=====全局数值读取接口====
//玩家配置
qreal getPlayerSpeed();       //玩家移动速度
int getPlayerInitSize();      //玩家初始尺寸
int getPlayerMaxSize();       //玩家最大尺寸
int getGrowRatio();             //吞噬配比1/4
//buff属性
qreal getSpeedBuffMult();  //精英怪BUFF倍率
qreal getDebuffMult();
int getBuffDuration();       //BUFF持续时间（ms）
//怪物配置
enum MonsterType{NORAMAL,ELITE,SPECIAL};//怪物类型
int getMaxMonsterCount();     //怪物数量上限
int getMonMinSize();       //怪物最小尺寸
int getMonMaxSize();      //怪物最大尺寸
qreal getMonSpeedRange();    //怪物移动速度
int getEliteMinSize();        //
int getEliteMaxSize();        //
qreal getEliteSpeedMult();   //精英怪移动速度变慢
qreal getSpecialSpeedMult(); //特殊怪移动变快
//各种怪物生成概率%
int getProbNoramal();
int getProbElite();
int getProbSpecial();
//画布边界预留边距
int getBoundOffset();
//HUD配置
const int HUD_BAR_WIDTH=150;
const int HUD_BAR_HEIFHT=12;
const int HUD_PADDING=15;
const QColor HUD_TEXT_COLOR=QColor(220,220,220);
const QColor PROGRESS_BG_COLOR=QColor(60,60,80);
const QColor PROGRESS_FG_COLOR=QColor(70,180,255);
enum GameState{RUNING,PAUSED};
//基因系统
enum GeneType{GENE_NONE,GENE_RANGE_EXTEND,GENE_SPEED_UP,GENE_GROW_BOOST};
//基因解锁所需吞噬数量
int getUnlockRangeNum();
int getUnlockSpeedNum();
int getUnlockGrowNum();
//基因属性倍率
qreal getGeneRangeRatio();
qreal getGenespeedRatio();
qreal getGeneGrowRatio();

//========三大生命法则======
enum LifeLaw{
    LAW_FISSION,        //裂变法则：躯体解构、近战爆发
    LAW_SYMBIOSIS,      //共生法则：族群集合、生产续航
    LAW_ILLUSION        //虚妄法则：时空拟态、偷袭控场
};
//=======四级躯体拆解等级====
enum DecomposeLevel{
    DECOMPOSE_NONE,     //原生未拆解
    DECOMPOSE_LIGHT,    //微拆解：低风险小幅蜕变
    DECOMPOSE_DEEP,     //深度解构：属性大幅取舍
    DECOMPOSE_FULL      //完全重构：突破原生细胞形态
};
qreal getLightDocmposeSpeedLoss();//微拆解移速损耗
qreal getLightDocmposeCritGain();//微拆解暴击增益
qreal getDeepDocmposeHPLoss();  //深度解构生命损耗
qreal getDeepDocmposeAtkGain();//深度解构攻击增益
qreal getFullDocmposeExtremeAt();//完全重构极限攻击
int getDecomposerRisk(DecomposeLevel lv);
//======演化素材=====
enum EvolutionMaterial{
    MATERIAL_ORGAN,
    MATERIAL_GENE,
    MATERIAL_LAW_CRYST
};
//======共生模式枚举====
enum SymbiosisMode{
    SYMBIO_TEMP,        //临时寄生：限时生效
    SYMBIO_PERM,        //永久共生：长期绑定
    SYMBIO_ABSORB       //吞噬同化：完全变为族群
};
//======基因排斥等级====
enum RejectLevel{
    REJECT_SAFE,        //安全范围，无负面影响
    REJECT_WARNING,     //轻度排斥，小幅属性衰弱
    REJECT_DANGER       //高危排斥，大幅战力损耗
};
int getMaxSymbiosisCount();     //最大共生容纳数量
int getSingleRejectValue();     //单个共生体排斥增加值
int getRejectWarningThreshold();//排斥分级阈值
int getRejectDangerThreshold();
int getTempSymbiosisDuration(); //临时寄生持续时间（ms）
qreal getSymbiosisFollowRange();//共生体跟随距离范围
}

#endif // GAMEGLOBAL_H
