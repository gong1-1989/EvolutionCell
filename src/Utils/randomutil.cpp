#include "RandomUtil.h"

int RandomUtil::randInt(int min, int max)
{
    if(min>max) qSwap(min,max);
    if(max<=0||min==max)return min;
    return QRandomGenerator::global()->bounded(min, max + 1);
}