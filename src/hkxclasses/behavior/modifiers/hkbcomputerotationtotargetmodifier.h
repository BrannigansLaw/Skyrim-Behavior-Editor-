#ifndef HKBCOMPUTEROTATIONTOTARGETMODIFIER_H
#define HKBCOMPUTEROTATIONTOTARGETMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeRotationToTargetModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class ComputeRotationToTargetModifierUI;
public:
    hkbComputeRotationToTargetModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbComputeRotationToTargetModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbComputeRotationToTargetModifier& operator=(const hkbComputeRotationToTargetModifier&);
    hkbComputeRotationToTargetModifier(const hkbComputeRotationToTargetModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable rotationOut;
    hkQuadVariable targetPosition;
    hkQuadVariable currentPosition;
    hkQuadVariable currentRotation;
    hkQuadVariable localAxisOfRotation;
    hkQuadVariable localFacingDirection;
    bool resultIsDelta;
};

#endif // HKBCOMPUTEROTATIONTOTARGETMODIFIER_H
