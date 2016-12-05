#ifndef HKBLOOKATMODIFIER_H
#define HKBLOOKATMODIFIER_H

#include "hkbmodifier.h"

class hkbLookAtModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbLookAtModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbLookAtModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbLookAtModifier& operator=(const hkbLookAtModifier&);
    hkbLookAtModifier(const hkbLookAtModifier &);
private:
    static uint refCount;
    static QString classname;
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
};

#endif // HKBLOOKATMODIFIER_H
