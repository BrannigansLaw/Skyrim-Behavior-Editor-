#ifndef HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H
#define HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbPoweredRagdollControlsModifier: public hkbModifier
{
    friend class PoweredRagdollControlsModifierUI;
public:
    hkbPoweredRagdollControlsModifier(HkxFile *parent, long ref = 0);
    hkbPoweredRagdollControlsModifier& operator=(const hkbPoweredRagdollControlsModifier&) = delete;
    hkbPoweredRagdollControlsModifier(const hkbPoweredRagdollControlsModifier &) = delete;
    ~hkbPoweredRagdollControlsModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    HkxSharedPtr getBoneWeights() const;
    HkxSharedPtr getBones() const;
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
    static const QStringList Mode;
    static const QString classname;
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
    mutable std::mutex mutex;
};

#endif // HKBPOWEREDRAGDOLLCONTROLSMODIFIER_H
