#ifndef HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H
#define HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbPoweredRagdollControlsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class PoweredRagdollControlsModifierUI;
public:
    hkbPoweredRagdollControlsModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbPoweredRagdollControlsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    hkbPoweredRagdollControlsModifier& operator=(const hkbPoweredRagdollControlsModifier&);
    hkbPoweredRagdollControlsModifier(const hkbPoweredRagdollControlsModifier &);
private:
    static QStringList Mode;    //WorldFromModelMode (WORLD_FROM_MODEL_MODE_USE_OLD=0;WORLD_FROM_MODEL_MODE_USE_INPUT=1;WORLD_FROM_MODEL_MODE_COMPUTE=2;WORLD_FROM_MODEL_MODE_NONE=3;WORLD_FROM_MODEL_MODE_RAGDOLL=4)
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal maxForce;
    qreal tau;
    qreal damping;
    qreal proportionalRecoveryVelocity;
    qreal constantRecoveryVelocity;
    HkxSharedPtr bones;
    int poseMatchingBone0;
    int poseMatchingBone1;
    int poseMatchingBone2;
    QString mode;
    HkxSharedPtr boneWeights;
};

#endif // HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H
