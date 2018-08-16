#ifndef HKBDAMPINGMODIFIER_H
#define HKBDAMPINGMODIFIER_H

#include "hkbmodifier.h"

class hkbDampingModifier final: public hkbModifier
{
    friend class DampingModifierUI;
public:
    hkbDampingModifier(HkxFile *parent, long ref = 0);
    hkbDampingModifier& operator=(const hkbDampingModifier&) = delete;
    hkbDampingModifier(const hkbDampingModifier &) = delete;
    ~hkbDampingModifier();
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
    qreal kP;
    qreal kI;
    qreal kD;
    bool enableScalarDamping;
    bool enableVectorDamping;
    qreal rawValue;
    qreal dampedValue;
    hkQuadVariable rawVector;
    hkQuadVariable dampedVector;
    hkQuadVariable vecErrorSum;
    hkQuadVariable vecPreviousError;
    qreal errorSum;
    qreal previousError;
    mutable std::mutex mutex;
};

#endif // HKBDAMPINGMODIFIER_H
