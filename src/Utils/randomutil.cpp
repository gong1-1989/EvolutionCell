#include "RandomUtil.h"

int RandomUtil::randInt(int min, int max)
{
    return QRandomGenerator::global()->bounded(min, max + 1);
}