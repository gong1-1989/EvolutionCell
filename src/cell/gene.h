#ifndef GENE_H
#define GENE_H
//基因实体模块
#include <QString>
#include "../Core/gameglobal.h"
class Gene
{
public:
    Gene(GameGlobal::GeneType type);
    GameGlobal::GeneType getType()const;
    QString getName()const;
    QString getDesc()const;
private:
    GameGlobal::GeneType m_type;
    QString m_name;
    QString m_desc;
};

#endif // GENE_H
