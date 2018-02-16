#ifndef BSLOOKATMODIFIER_H
#define BSLOOKATMODIFIER_H

#include "hkbmodifier.h"

class BSLookAtModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSBoneUI;
    friend class BSLookAtModifierUI;
public:
    BSLookAtModifier(HkxFile *parent, long ref = 0);
    virtual ~BSLookAtModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfBones() const;
    int getNumberOfEyeBones() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
private:
    BSLookAtModifier& operator=(const BSLookAtModifier&);
    BSLookAtModifier(const BSLookAtModifier &);
private:
    struct BsBone{
        BsBone(): index(-1), limitAngleDegrees(0), onGain(0), offGain(0), enabled(true){}
        int index;
        hkQuadVariable fwdAxisLS;
        qreal limitAngleDegrees;
        qreal onGain;
        qreal offGain;
        bool enabled;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    bool lookAtTarget;
    QList <BsBone> bones;
    QList <BsBone> eyeBones;
    qreal limitAngleDegrees;
    qreal limitAngleThresholdDegrees;
    bool continueLookOutsideOfLimit;
    qreal onGain;
    qreal offGain;
    bool useBoneGains;
    hkQuadVariable targetLocation;
    bool targetOutsideLimits;
    int id;
    HkxSharedPtr payload;
    bool lookAtCamera;
    qreal lookAtCameraX;
    qreal lookAtCameraY;
    qreal lookAtCameraZ;
};

#endif // BSLOOKATMODIFIER_H
