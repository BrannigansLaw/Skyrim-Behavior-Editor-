#ifndef HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H
#define HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbRigidBodyRagdollControlsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class RigidBodyRagdollControlsModifierUI;
public:
    hkbRigidBodyRagdollControlsModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbRigidBodyRagdollControlsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbRigidBodyRagdollControlsModifier& operator=(const hkbRigidBodyRagdollControlsModifier&);
    hkbRigidBodyRagdollControlsModifier(const hkbRigidBodyRagdollControlsModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal hierarchyGain;
    qreal velocityDamping;
    qreal accelerationGain;
    qreal velocityGain;
    qreal positionGain;
    qreal positionMaxLinearVelocity;
    qreal positionMaxAngularVelocity;
    qreal snapGain;
    qreal snapMaxLinearVelocity;
    qreal snapMaxAngularVelocity;
    qreal snapMaxLinearDistance;
    qreal snapMaxAngularDistance;
    qreal durationToBlend;
    HkxSharedPtr bones;
};

#endif // HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H
