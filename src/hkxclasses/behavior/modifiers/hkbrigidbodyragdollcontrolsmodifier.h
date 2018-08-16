#ifndef HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H
#define HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbRigidBodyRagdollControlsModifier final: public hkbModifier
{
    friend class RigidBodyRagdollControlsModifierUI;
public:
    hkbRigidBodyRagdollControlsModifier(HkxFile *parent, long ref = 0);
    hkbRigidBodyRagdollControlsModifier& operator=(const hkbRigidBodyRagdollControlsModifier&) = delete;
    hkbRigidBodyRagdollControlsModifier(const hkbRigidBodyRagdollControlsModifier &) = delete;
    ~hkbRigidBodyRagdollControlsModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    static uint refCount;
    static const QString classname;
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
    mutable std::mutex mutex;
};

#endif // HKBRIGIDBODYRAGDOLLCONTROLSMODIFIER_H
