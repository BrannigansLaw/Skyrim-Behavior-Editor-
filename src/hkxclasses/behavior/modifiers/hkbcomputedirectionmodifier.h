#ifndef HKBCOMPUTEDIRECTIONMODIFIER_H
#define HKBCOMPUTEDIRECTIONMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeDirectionModifier final: public hkbModifier
{
    friend class ComputeDirectionModifierUI;
public:
    hkbComputeDirectionModifier(HkxFile *parent, long ref = 0);
    hkbComputeDirectionModifier& operator=(const hkbComputeDirectionModifier&) = delete;
    hkbComputeDirectionModifier(const hkbComputeDirectionModifier &) = delete;
    ~hkbComputeDirectionModifier();
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
    hkQuadVariable pointIn;
    hkQuadVariable pointOut;
    qreal groundAngleOut;
    qreal upAngleOut;
    qreal verticalOffset;
    bool reverseGroundAngle;
    bool reverseUpAngle;
    bool projectPoint;
    bool normalizePoint;
    bool computeOnlyOnce;
    bool computedOutput;
    mutable std::mutex mutex;
};

#endif // HKBCOMPUTEDIRECTIONMODIFIER_H
