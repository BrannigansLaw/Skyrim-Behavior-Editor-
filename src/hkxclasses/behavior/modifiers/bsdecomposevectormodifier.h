#ifndef BSDECOMPOSEVECTORMODIFIER_H
#define BSDECOMPOSEVECTORMODIFIER_H

#include "hkbmodifier.h"

class BSDecomposeVectorModifier final: public hkbModifier
{
    friend class BSDecomposeVectorModifierUI;
public:
    BSDecomposeVectorModifier(HkxFile *parent, long ref = 0);
    BSDecomposeVectorModifier& operator=(const BSDecomposeVectorModifier&) = delete;
    BSDecomposeVectorModifier(const BSDecomposeVectorModifier &) = delete;
    ~BSDecomposeVectorModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable vector;
    qreal x;
    qreal y;
    qreal z;
    qreal w;
    mutable std::mutex mutex;
};

#endif // BSDECOMPOSEVECTORMODIFIER_H
