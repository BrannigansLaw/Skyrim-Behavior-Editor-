#ifndef HKBDAMPINGMODIFIER_H
#define HKBDAMPINGMODIFIER_H

#include "hkbmodifier.h"

class hkbDampingModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbDampingModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbDampingModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbDampingModifier& operator=(const hkbDampingModifier&);
    hkbDampingModifier(const hkbDampingModifier &);
private:
    static uint refCount;
    static QString classname;
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
};

#endif // HKBDAMPINGMODIFIER_H
