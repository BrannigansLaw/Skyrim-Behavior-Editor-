#ifndef HKBEXTRACTRAGDOLLPOSEMODIFIER_H
#define HKBEXTRACTRAGDOLLPOSEMODIFIER_H

#include "hkbmodifier.h"

class hkbExtractRagdollPoseModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class ExtractRagdollPoseModifierUI;
public:
    hkbExtractRagdollPoseModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbExtractRagdollPoseModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbExtractRagdollPoseModifier& operator=(const hkbExtractRagdollPoseModifier&);
    hkbExtractRagdollPoseModifier(const hkbExtractRagdollPoseModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    int poseMatchingBone0;
    int poseMatchingBone1;
    int poseMatchingBone2;
    bool enableComputeWorldFromModel;
};

#endif // HKBEXTRACTRAGDOLLPOSEMODIFIER_H
