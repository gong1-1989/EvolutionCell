#include "gameglobal.h"
using namespace GameGlobal;
static ConfigReader& cfg=ConfigReader::getInstance();
qreal GameGlobal::getPlayerSpeed(){
    auto obj=cfg.getGemeConfig()["player_base"].toObject();
    return obj["move_speed"].toDouble(3.0);
}
int GameGlobal::getPlayerInitSize(){
    auto obj=cfg.getGemeConfig()["player_base"].toObject();
    return obj["init_size"].toInt(24);
}
int GameGlobal::getPlayerMaxSize(){
    auto obj=cfg.getGemeConfig()["player_base"].toObject();
    return obj["max_size"].toInt(80);
}
int GameGlobal::getGrowRatio(){
    auto obj=cfg.getGemeConfig()["player_base"].toObject();
    return obj["grow_ratio"].toInt(4);
}
qreal GameGlobal::getSpeedBuffMult(){
    auto obj=cfg.getGemeConfig()["buff_setting"].toObject();
    return obj["speed_buff_mult"].toDouble(1.5);
}
qreal GameGlobal::getDebuffMult(){
    auto obj=cfg.getGemeConfig()["buff_setting"].toObject();
    return obj["debuff_mult"].toDouble(0.7);
}
int GameGlobal::getBuffDuration(){
    auto obj=cfg.getGemeConfig()["buff_setting"].toObject();
    return obj["buff_duration_ms"].toInt(3000);
}
int GameGlobal::getMaxMonsterCount(){
    auto obj=cfg.getGemeConfig()["scene_setting"].toObject();
    return obj["bound_offset"].toInt(30);
}
int GameGlobal::getMonMinSize(){
    auto obj=cfg.getMonsterConfig()["normal"].toObject();
    return obj["size_min"].toInt(8);
}
int GameGlobal::getMonMaxSize(){
    auto obj=cfg.getMonsterConfig()["normal"].toObject();
    return obj["size_max"].toInt(16);
}
qreal GameGlobal::getMonSpeedRange(){
    auto obj=cfg.getMonsterConfig()["normal"].toObject();
    return obj["speed_range"].toDouble(1.50);
}
int GameGlobal::getEliteMinSize(){
    auto obj=cfg.getMonsterConfig()["elite"].toObject();
    return obj["size_min"].toInt(18);
}
int GameGlobal::getEliteMaxSize(){
    auto obj=cfg.getMonsterConfig()["elite"].toObject();
    return obj["size_max"].toInt(24);
}
qreal GameGlobal::getEliteSpeedMult(){
    auto obj=cfg.getMonsterConfig()["elite"].toObject();
    return obj["speed_mult"].toDouble(0.7);
}
qreal GameGlobal::getSpecialSpeedMult(){
    auto obj=cfg.getMonsterConfig()["special"].toObject();
    return obj["speed_mult"].toDouble(1.3);
}
int GameGlobal::getProbNoramal(){
    auto obj=cfg.getMonsterConfig()["normal"].toObject();
    return obj["probability"].toInt(70);
}
int GameGlobal::getProbElite(){
    auto obj=cfg.getMonsterConfig()["elite"].toObject();
    return obj["probability"].toInt(20);
}
int GameGlobal::getProbSpecial(){
    auto obj=cfg.getMonsterConfig()["special"].toObject();
    return obj["probability"].toInt(10);
}
int GameGlobal::getBoundOffset(){
    auto obj=cfg.getGemeConfig()["scene_setting"].toObject();
    return obj["bound_offset"].toInt(30);
}
int GameGlobal::getUnlockRangeNum(){
    auto obj=cfg.getGeneConfig()["unlock_condition"].toObject();
    return obj["range_extend"].toInt(8);
}
int GameGlobal::getUnlockSpeedNum(){
    auto obj=cfg.getGeneConfig()["unlock_condition"].toObject();
    return obj["speed_up"].toInt(10);
}
int GameGlobal::getUnlockGrowNum(){
    auto obj=cfg.getGeneConfig()["unlock_condition"].toObject();
    return obj["grow_boost"].toInt(6);
}
qreal GameGlobal::getGeneRangeRatio(){
    auto obj=cfg.getGeneConfig()["gene_ratio"].toObject();
    return obj["range"].toDouble(1.25);
}
qreal GameGlobal::getGenespeedRatio(){
    auto obj=cfg.getGeneConfig()["gene_ratio"].toObject();
    return obj["speed"].toDouble(1.15);
}
qreal GameGlobal::getGeneGrowRatio(){
    auto obj=cfg.getGeneConfig()["gene_ratio"].toObject();
    return obj["grow"].toDouble(1.2);
}

