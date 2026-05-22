#ifndef GENTCELL_H
#define GENTCELL_H
//谱系细胞派生类
#include "cellentity.h"

//肌系：赤红输出细胞
class MuscleCell:public Cellentity{
public:
    MuscleCell();
};
//盾系：幽蓝防御细胞
class ShieldCell:public Cellentity{
public:
    ShieldCell();
};

//术系：紫晶法术孢子细胞
class MagicCell:public Cellentity
{
public:
    MagicCell();
};

#endif // GENTCELL_H
