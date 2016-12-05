#ifndef BSDECOMPOSEVECTORMODIFIER_H
#define BSDECOMPOSEVECTORMODIFIER_H

#include "hkbmodifier.h"

class BSDecomposeVectorModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSDecomposeVectorModifier(HkxFile *parent, long ref = 0);
    virtual ~BSDecomposeVectorModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSDecomposeVectorModifier& operator=(const BSDecomposeVectorModifier&);
    BSDecomposeVectorModifier(const BSDecomposeVectorModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable vector;
    qreal x;
    qreal y;
    qreal z;
    qreal w;
};

#endif // BSDECOMPOSEVECTORMODIFIER_H
