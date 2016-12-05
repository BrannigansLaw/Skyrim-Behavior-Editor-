#ifndef BSINTERPVALUEMODIFIER_H
#define BSINTERPVALUEMODIFIER_H

#include "hkbmodifier.h"

class BSInterpValueModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSInterpValueModifier(HkxFile *parent, long ref = 0);
    virtual ~BSInterpValueModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSInterpValueModifier& operator=(const BSInterpValueModifier&);
    BSInterpValueModifier(const BSInterpValueModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal source;
    qreal target;
    qreal result;
    qreal gain;
};

#endif // BSINTERPVALUEMODIFIER_H
