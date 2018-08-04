#ifndef BSSPEEDSAMPLERMODIFIER_H
#define BSSPEEDSAMPLERMODIFIER_H

#include "hkbmodifier.h"

class BSSpeedSamplerModifier final: public hkbModifier
{
    friend class BSSpeedSamplerModifierUI;
public:
    BSSpeedSamplerModifier(HkxFile *parent, long ref = 0);
    BSSpeedSamplerModifier& operator=(const BSSpeedSamplerModifier&) = delete;
    BSSpeedSamplerModifier(const BSSpeedSamplerModifier &) = delete;
    ~BSSpeedSamplerModifier();
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
    int state;
    qreal direction;
    qreal goalSpeed;
    qreal speedOut;
    mutable std::mutex mutex;
};

#endif // BSSPEEDSAMPLERMODIFIER_H
