#ifndef HKBEXTRACTRAGDOLLPOSEMODIFIER_H
#define HKBEXTRACTRAGDOLLPOSEMODIFIER_H

#include "hkbmodifier.h"

class hkbExtractRagdollPoseModifier final: public hkbModifier
{
    friend class ExtractRagdollPoseModifierUI;
public:
    hkbExtractRagdollPoseModifier(HkxFile *parent, long ref = 0);
    hkbExtractRagdollPoseModifier& operator=(const hkbExtractRagdollPoseModifier&) = delete;
    hkbExtractRagdollPoseModifier(const hkbExtractRagdollPoseModifier &) = delete;
    ~hkbExtractRagdollPoseModifier();
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
    int poseMatchingBone0;
    int poseMatchingBone1;
    int poseMatchingBone2;
    bool enableComputeWorldFromModel;
    mutable std::mutex mutex;
};

#endif // HKBEXTRACTRAGDOLLPOSEMODIFIER_H
