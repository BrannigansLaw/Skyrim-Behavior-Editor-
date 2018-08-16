#ifndef HKBLOOKATMODIFIER_H
#define HKBLOOKATMODIFIER_H

#include "hkbmodifier.h"

class hkbLookAtModifier final: public hkbModifier
{
    friend class LookAtModifierUI;
public:
    hkbLookAtModifier(HkxFile *parent, long ref = 0);
    hkbLookAtModifier& operator=(const hkbLookAtModifier&) = delete;
    hkbLookAtModifier(const hkbLookAtModifier &) = delete;
    ~hkbLookAtModifier();
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
    hkQuadVariable targetWS;
    hkQuadVariable headForwardLS;
    hkQuadVariable neckForwardLS;
    hkQuadVariable neckRightLS;
    hkQuadVariable eyePositionHS;
    qreal newTargetGain;
    qreal onGain;
    qreal offGain;
    qreal limitAngleDegrees;
    qreal limitAngleLeft;
    qreal limitAngleRight;
    qreal limitAngleUp;
    qreal limitAngleDown;
    int headIndex;
    int neckIndex;
    bool isOn;
    bool individualLimitsOn;
    bool isTargetInsideLimitCone;
    mutable std::mutex mutex;
};

#endif // HKBLOOKATMODIFIER_H
