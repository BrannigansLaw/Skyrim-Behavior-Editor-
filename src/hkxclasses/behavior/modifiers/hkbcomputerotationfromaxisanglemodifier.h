#ifndef HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H
#define HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeRotationFromAxisAngleModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbComputeRotationFromAxisAngleModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbComputeRotationFromAxisAngleModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbComputeRotationFromAxisAngleModifier& operator=(const hkbComputeRotationFromAxisAngleModifier&);
    hkbComputeRotationFromAxisAngleModifier(const hkbComputeRotationFromAxisAngleModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable rotationOut;
    hkQuadVariable axis;
    qreal angleDegrees;
};

#endif // HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H
