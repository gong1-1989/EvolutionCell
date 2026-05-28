#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include <QRandomGenerator>

class RandomUtil
{
public:
    static int randInt(int min, int max){
        if(min>max) qSwap(min,max);
        if(max<=0||min==max)return min;
        return QRandomGenerator::global()->bounded(min, max + 1);
    };
};

#endif // RANDOMUTIL_H