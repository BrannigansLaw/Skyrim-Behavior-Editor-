#ifndef HKBCOMPUTEDIRECTIONMODIFIER_H
#define HKBCOMPUTEDIRECTIONMODIFIER_H

#include "hkbmodifier.h"

class hkbComputeDirectionModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbComputeDirectionModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbComputeDirectionModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbComputeDirectionModifier& operator=(const hkbComputeDirectionModifier&);
    hkbComputeDirectionModifier(const hkbComputeDirectionModifier &);
private:
    static uint refCount;
    static QString classname;
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
};

#endif // HKBCOMPUTEDIRECTIONMODIFIER_H
