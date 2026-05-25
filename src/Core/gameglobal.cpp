#include "gameglobal.h"
using namespace GameGlobal;
static ConfigReader& cfg=ConfigReader::getInstance();
qreal GameGlobal::getPlayerSpeed(){
    auto obj=cfg.getGemeConfig().value("player_base").toObject();
    return obj.value("move_speed").toDouble(3.0);
}
int GameGlobal::getPlayerInitSize(){
    auto obj=cfg.getGemeConfig().value("player_base").toObject();
    return obj.value("init_size").toInt(24);
}
int GameGlobal::getPlayerMaxSize(){
    auto obj=cfg.getGemeConfig().value("player_base").toObject();
    return obj.value("max_size").toInt(80);
}
int GameGlobal::getGrowRatio(){
    auto obj=cfg.getGemeConfig().value("player_base").toObject();
    return obj.value("grow_ratio").toInt(4);
}
qreal GameGlobal::getSpeedBuffMult(){
    auto obj=cfg.getGemeConfig().value("buff_setting").toObject();
    return obj.value("speed_buff_mult").toDouble(1.5);
}
qreal GameGlobal::getDebuffMult(){
    auto obj=cfg.getGemeConfig().value("buff_setting").toObject();
    return obj.value("debuff_mult").toDouble(0.7);
}
int GameGlobal::getBuffDuration(){
    auto obj=cfg.getGemeConfig().value("buff_setting").toObject();
    return obj.value("buff_duration_ms").toInt(3000);
}
int GameGlobal::getMaxMonsterCount(){
    auto obj=cfg.getGemeConfig().value("scene_setting").toObject();
    return obj.value("bound_offset").toInt(30);
}
int GameGlobal::getMonMinSize(){
    auto obj=cfg.getMonsterConfig().value("normal").toObject();
    return obj.value("size_min").toInt(8);
}
int GameGlobal::getMonMaxSize(){
    auto obj=cfg.getMonsterConfig().value("normal").toObject();
    return obj.value("size_max").toInt(16);
}
qreal GameGlobal::getMonSpeedRange(){
    auto obj=cfg.getMonsterConfig().value("normal").toObject();
    return obj.value("speed_range").toDouble(1.50);
}
int GameGlobal::getEliteMinSize(){
    auto obj=cfg.getMonsterConfig().value("elite").toObject();
    return obj.value("size_min").toInt(18);
}
int GameGlobal::getEliteMaxSize(){
    auto obj=cfg.getMonsterConfig().value("elite").toObject();
    return obj.value("size_max").toInt(24);
}
qreal GameGlobal::getEliteSpeedMult(){
    auto obj=cfg.getMonsterConfig().value("elite").toObject();
    return obj["speed_mult"].toDouble(0.7);
}
qreal GameGlobal::getSpecialSpeedMult(){
    auto obj=cfg.getMonsterConfig().value("special").toObject();
    return obj["speed_mult"].toDouble(1.3);
}
int GameGlobal::getProbNoramal(){
    auto obj=cfg.getMonsterConfig().value("normal").toObject();
    return obj.value("probability").toInt(70);
}
int GameGlobal::getProbElite(){
    auto obj=cfg.getMonsterConfig().value("elite").toObject();
    return obj.value("probability").toInt(20);
}
int GameGlobal::getProbSpecial(){
    auto obj=cfg.getMonsterConfig().value("special").toObject();
    return obj.value("probability").toInt(10);
}
int GameGlobal::getBoundOffset(){
    auto obj=cfg.getGemeConfig().value("scene_setting").toObject();
    return obj.value("bound_offset").toInt(30);
}
int GameGlobal::getUnlockRangeNum(){
    auto obj=cfg.getGeneConfig().value("unlock_condition").toObject();
    return obj.value("range_extend").toInt(8);
}
int GameGlobal::getUnlockSpeedNum(){
    auto obj=cfg.getGeneConfig().value("unlock_condition").toObject();
    return obj.value("speed_up").toInt(10);
}
int GameGlobal::getUnlockGrowNum(){
    auto obj=cfg.getGeneConfig().value("unlock_condition").toObject();
    return obj.value("grow_boost").toInt(6);
}
qreal GameGlobal::getGeneRangeRatio(){
    auto obj=cfg.getGeneConfig().value("gene_ratio").toObject();
    return obj.value("range").toDouble(1.25);
}
qreal GameGlobal::getGenespeedRatio(){
    auto obj=cfg.getGeneConfig().value("gene_ratio").toObject();
    return obj.value("speed").toDouble(1.15);
}
qreal GameGlobal::getGeneGrowRatio(){
    auto obj=cfg.getGeneConfig().value("gene_ratio").toObject();
    return obj.value("grow").toDouble(1.2);
}
qreal GameGlobal::getLightDocmposeSpeedLoss(){
    auto obj=cfg.getGemeConfig().value("decompose_attr").toObject();
    return obj.value("light_speed_loss").toDouble(0.1);
}
qreal GameGlobal::getLightDocmposeCritGain(){
    auto obj=cfg.getGemeConfig().value("decompose_attr").toObject();
    return obj.value("light_crit_gain").toDouble(0.15);
}
qreal GameGlobal::getDeepDocmposeHPLoss(){
    auto obj=cfg.getGemeConfig().value("decompose_attr").toObject();
    return obj.value("deep_hp_loss").toDouble(0.15);
}
qreal GameGlobal::getDeepDocmposeAtkGain(){
    auto obj=cfg.getGemeConfig().value("decompose_attr").toObject();
    return obj.value("deep_atk_gain").toDouble(0.25);
}
qreal GameGlobal::getFullDocmposeExtremeAt(){
    auto obj=cfg.getGemeConfig().value("decompose_attr").toObject();
    return obj.value("full_atk_gain").toDouble(0.4);
}
int GameGlobal::getDecomposerRisk(DecomposeLevel lv){
    switch (lv) {
    case DECOMPOSE_LIGHT:
        return 1;
        break;
    case DECOMPOSE_DEEP:
        return 2;
        break;
    case DECOMPOSE_FULL:
        return 3;
        break;
    default:
        return 0;
        break;
    }
}
int GameGlobal::getMaxSymbiosisCount(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("max_count").toInt(6);
}
int GameGlobal::getSingleRejectValue(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("single_reject").toInt(8);
}
int GameGlobal::getRejectWarningThreshold(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("warn_threshold").toInt(25);
}
int GameGlobal::getRejectDangerThreshold(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("danger_threshold").toInt(50);
}
int GameGlobal::getTempSymbiosisDuration(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("temp_duration").toInt(8000);
}
qreal GameGlobal::getSymbiosisFollowRange(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("follow_range").toDouble(60.0);
}
qreal GameGlobal::getSymAttackRange(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("attack_range").toDouble(25.0);
}
int GameGlobal::getNormalSymAttack(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("atk_normal").toInt(1);
}
int GameGlobal::getEliteSymAttack(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("atk_elite").toInt(3);
}
int GameGlobal::getSpecialSymAttack(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("atk_special").toInt(5);
}
int GameGlobal::getSymAttackCdMs(){
    auto obj=cfg.getGemeConfig().value("symbiosis_setting").toObject();
    return obj.value("attack_cd").toInt(600);
}