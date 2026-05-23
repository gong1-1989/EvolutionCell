#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H
//随机数工具
#include <QRandomGenerator>
#include <QColor>
class RandomUtil
{
public:
    static int randInt(int min,int max);
    static qreal randReal(qreal min,qreal max);
    static QColor randMonsterColor();
};

#endif // RANDOMUTIL_H
