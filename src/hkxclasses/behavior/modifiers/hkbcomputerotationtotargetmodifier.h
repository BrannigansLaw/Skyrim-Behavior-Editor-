#ifndef HKBCOMPUTEROTATIONTOTARGETMODIFIER_H
#define HKBCOMPUTEROTATIONTOTARGETMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeRotationToTargetModifier final: public hkbModifier
{
    friend class ComputeRotationToTargetModifierUI;
public:
    hkbComputeRotationToTargetModifier(HkxFile *parent, long ref = 0);
    hkbComputeRotationToTargetModifier& operator=(const hkbComputeRotationToTargetModifier&) = delete;
    hkbComputeRotationToTargetModifier(const hkbComputeRotationToTargetModifier &) = delete;
    ~hkbComputeRotationToTargetModifier();
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
    hkQuadVariable rotationOut;
    hkQuadVariable targetPosition;
    hkQuadVariable currentPosition;
    hkQuadVariable currentRotation;
    hkQuadVariable localAxisOfRotation;
    hkQuadVariable localFacingDirection;
    bool resultIsDelta;
    mutable std::mutex mutex;
};

#endif // HKBCOMPUTEROTATIONTOTARGETMODIFIER_H
