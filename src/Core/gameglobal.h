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
}

#endif // GAMEGLOBAL_H
