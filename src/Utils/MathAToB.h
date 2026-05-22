#ifndef MATHATOB_H
#define MATHATOB_H
#include <QtMath>
#include <QPointF>

class MathAToB{
public:
    static qreal aToB(const QPointF& a,const QPointF& b){
        qreal x=a.x()-b.x();
        qreal y=a.y()-b.y();
        return qSqrt(x*x+y*y);
    }
    static QPointF AMoveFromB(const QPointF& a,const QPointF& b){

        qreal x=a.x()-b.x();
        qreal y=a.y()-b.y();
        qreal len=qSqrt(x*x+y*y);
        if(len>0){
            x/=len;
            y/=len;
        }
        return QPointF(x,y);
    }
};

#endif // MATHATOB_H
