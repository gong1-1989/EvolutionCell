#include "randomutil.h"

int RandomUtil::randInt(int min, int max){
    return QRandomGenerator::global()->bounded(min,max);
}
qreal RandomUtil::randReal(qreal min, qreal max){
    return QRandomGenerator::global()->bounded(int(min*100),int(max*100)+1)/100.0;
}
QColor RandomUtil::randMonsterColor(){
    return QColor(randInt(50,100),randInt(150,255),randInt(50,100));
}