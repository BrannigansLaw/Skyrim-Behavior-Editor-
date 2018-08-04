#ifndef HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H
#define HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeRotationFromAxisAngleModifier final: public hkbModifier
{
    friend class ComputeRotationFromAxisAngleModifierUI;
public:
    hkbComputeRotationFromAxisAngleModifier(HkxFile *parent, long ref = 0);
    hkbComputeRotationFromAxisAngleModifier& operator=(const hkbComputeRotationFromAxisAngleModifier&) = delete;
    hkbComputeRotationFromAxisAngleModifier(const hkbComputeRotationFromAxisAngleModifier &) = delete;
    ~hkbComputeRotationFromAxisAngleModifier();
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
    hkQuadVariable axis;
    qreal angleDegrees;
    mutable std::mutex mutex;
};

#endif // HKBCOMPUTEROTATIONFROMAXISANGLEMODIFIER_H
