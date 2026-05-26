#include "GameGlobal.h"

/**
 * @brief 重构说明
 * 原代码大量重复 cfg["xxx"]["xxx"] 解析，现统一调用 ConfigReader 通用方法
 * 所有接口仅做「取值转发」，新增/修改配置仅需改 json + 接口声明，无需改动实现
 */
using namespace GameGlobal;
static ConfigReader& cfg = ConfigReader::getInstance();
//===================== 全局窗口设置 ===================
int GameGlobal::getWindowWidth(){
    return cfg.getInt(cfg.getRootConfig()["game_setting"].toObject(),"width",1280);
}
int GameGlobal::getWindowHeight(){
    return cfg.getInt(cfg.getRootConfig()["game_setting"].toObject(),"height",720);
}
int GameGlobal::getFPS(){
    return cfg.getInt(cfg.getRootConfig()["game_setting"].toObject(),"fps",60);
}

QString GameGlobal::getWindowTitle(){
    return cfg.getString(cfg.getRootConfig()["game_setting"].toObject(),"title","未知");
}
// ===================== 玩家配置 =====================
qreal GameGlobal::getPlayerSpeed()
{
    return cfg.getDouble(cfg.getRootConfig()["player_setting"].toObject(), "move_speed", 2.8);
}

int GameGlobal::getPlayerInitSize()
{
    return cfg.getInt(cfg.getRootConfig()["player_setting"].toObject(), "init_size", 20);
}

int GameGlobal::getPlayerMaxSize()
{
    return cfg.getInt(cfg.getRootConfig()["player_setting"].toObject(), "max_size", 150);
}

int GameGlobal::getGrowRatio()
{
    return cfg.getInt(cfg.getRootConfig()["player_setting"].toObject(), "grow_ratio", 8);
}

qreal GameGlobal::getSpeedBuffMult()
{
    return cfg.getDouble(cfg.getRootConfig()["player_setting"].toObject(), "speed_buff_mult", 1.6);
}

int GameGlobal::getBuffDuration()
{
    return cfg.getInt(cfg.getRootConfig()["player_setting"].toObject(), "buff_duration", 4000);
}

qreal GameGlobal::getDebuffMult()
{
    return cfg.getDouble(cfg.getRootConfig()["player_setting"].toObject(), "debuff_mult", 0.5);
}

// ===================== 怪物配置 =====================
int GameGlobal::getMaxMonsterCount()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "max_count", 18);
}

int GameGlobal::getBoundOffset()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "bound_offset", 30);
}

int GameGlobal::getMonMinSize()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "min_size", 8);
}

int GameGlobal::getMonMaxSize()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "max_size", 35);
}

qreal GameGlobal::getMonSpeedRange()
{
    return cfg.getDouble(cfg.getRootConfig()["monster_setting"].toObject(), "speed_max", 2.2);
}

int GameGlobal::getEliteMinSize()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "elite_min_size", 22);
}

int GameGlobal::getEliteMaxSize()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "elite_max_size", 45);
}

qreal GameGlobal::getEliteSpeedMult()
{
    return cfg.getDouble(cfg.getRootConfig()["monster_setting"].toObject(), "elite_speed_mult", 1.3);
}

qreal GameGlobal::getSpecialSpeedMult()
{
    return cfg.getDouble(cfg.getRootConfig()["monster_setting"].toObject(), "special_speed_mult", 1.5);
}

int GameGlobal::getProbNormal()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "prob_normal", 65);
}

int GameGlobal::getProbElite()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "prob_elite", 25);
}

int GameGlobal::getProbSpecial()
{
    return cfg.getInt(cfg.getRootConfig()["monster_setting"].toObject(), "prob_special", 10);
}

// ===================== 基因配置 =====================
int GameGlobal::getUnlockRangeNum()
{
    return cfg.getInt(cfg.getRootConfig()["gene_setting"].toObject(), "unlock_range_num", 8);
}

int GameGlobal::getUnlockSpeedNum()
{
    return cfg.getInt(cfg.getRootConfig()["gene_setting"].toObject(), "unlock_speed_num", 12);
}

int GameGlobal::getUnlockGrowNum()
{
    return cfg.getInt(cfg.getRootConfig()["gene_setting"].toObject(), "unlock_grow_num", 15);
}

qreal GameGlobal::getGeneRangeRatio()
{
    return cfg.getDouble(cfg.getRootConfig()["gene_setting"].toObject(), "range_ratio", 1.25);
}

qreal GameGlobal::getGeneSpeedRatio()
{
    return cfg.getDouble(cfg.getRootConfig()["gene_setting"].toObject(), "speed_ratio", 1.2);
}

qreal GameGlobal::getGeneGrowRatio()
{
    return cfg.getDouble(cfg.getRootConfig()["gene_setting"].toObject(), "grow_ratio", 1.18);
}

// ===================== 躯体解构配置 =====================
qreal GameGlobal::getLightDecomposeSpeedLoss()
{
    return cfg.getDouble(cfg.getRootConfig()["decompose_setting"].toObject(), "light_speed_loss", 0.15);
}

qreal GameGlobal::getLightDecomposeCritGain()
{
    return cfg.getDouble(cfg.getRootConfig()["decompose_setting"].toObject(), "light_crit_gain", 0.2);
}

qreal GameGlobal::getDeepDecomposeHPLoss()
{
    return cfg.getDouble(cfg.getRootConfig()["decompose_setting"].toObject(), "deep_hp_loss", 0.25);
}

qreal GameGlobal::getDeepDecomposeAtkGain()
{
    return cfg.getDouble(cfg.getRootConfig()["decompose_setting"].toObject(), "deep_atk_gain", 0.35);
}

qreal GameGlobal::getFullDecomposeExtremeAtk()
{
    return cfg.getDouble(cfg.getRootConfig()["decompose_setting"].toObject(), "full_extreme_atk", 0.6);
}

int GameGlobal::getDecomposeRisk(DecomposeLevel lv)
{
    switch (lv)
    {
    case DECOMPOSE_LIGHT: return 10;
    case DECOMPOSE_DEEP:  return 25;
    case DECOMPOSE_FULL:  return 45;
    default: return 0;
    }
}

// ===================== 共生体系配置 =====================
int GameGlobal::getMaxSymbiosisCount()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "max_count", 6);
}

int GameGlobal::getSingleRejectValue()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "single_reject", 8);
}

int GameGlobal::getRejectWarningThreshold()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "warn_threshold", 25);
}

int GameGlobal::getRejectDangerThreshold()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "danger_threshold", 50);
}

int GameGlobal::getTempSymbiosisDuration()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "temp_duration", 8000);
}

qreal GameGlobal::getSymbiosisFollowRange()
{
    return cfg.getDouble(cfg.getRootConfig()["symbiosis_setting"].toObject(), "follow_range", 150.0);
}

qreal GameGlobal::getSymAttackRange()
{
    return cfg.getDouble(cfg.getRootConfig()["symbiosis_setting"].toObject(), "attack_range", 30.0);
}

int GameGlobal::getNormalSymAttack()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "atk_normal", 1);
}

int GameGlobal::getEliteSymAttack()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "atk_elite", 3);
}

int GameGlobal::getSpecialSymAttack()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "atk_special", 5);
}

int GameGlobal::getSymAttackCdMs()
{
    return cfg.getInt(cfg.getRootConfig()["symbiosis_setting"].toObject(), "attack_cd", 500);
}

// ===================== 回溯 & 残影配置 =====================
int GameGlobal::getMaxHistoryNode()
{
    return cfg.getInt(cfg.getRootConfig()["rollback_ghost_setting"].toObject(), "max_history_node", 8);
}

int GameGlobal::getRollbackCostRisk()
{
    return cfg.getInt(cfg.getRootConfig()["rollback_ghost_setting"].toObject(), "rollback_cost_risk", 10);
}

int GameGlobal::getRollbackCostReject()
{
    return cfg.getInt(cfg.getRootConfig()["rollback_ghost_setting"].toObject(), "rollback_cost_reject", 15);
}

int GameGlobal::getGhostLifeTime()
{
    return cfg.getInt(cfg.getRootConfig()["rollback_ghost_setting"].toObject(), "ghost_life_time", 5000);
}

qreal GameGlobal::getGhostAttackMult()
{
    return cfg.getDouble(cfg.getRootConfig()["rollback_ghost_setting"].toObject(), "ghost_attack_mult", 0.6);
}