#ifndef BSSPEEDSAMPLERMODIFIER_H
#define BSSPEEDSAMPLERMODIFIER_H

#include "hkbmodifier.h"

class BSSpeedSamplerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSSpeedSamplerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSSpeedSamplerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSSpeedSamplerModifier& operator=(const BSSpeedSamplerModifier&);
    BSSpeedSamplerModifier(const BSSpeedSamplerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    int state;
    qreal direction;
    qreal goalSpeed;
    qreal speedOut;
};

#endif // BSSPEEDSAMPLERMODIFIER_H
