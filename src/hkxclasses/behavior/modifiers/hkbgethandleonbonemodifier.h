#ifndef HKBGETHANDLEONBONEMODIFIER_H
#define HKBGETHANDLEONBONEMODIFIER_H

#include "hkbmodifier.h"

class hkbGetHandleOnBoneModifier final: public hkbModifier
{
    friend class GetHandleOnBoneModifierUI;
public:
    hkbGetHandleOnBoneModifier(HkxFile *parent, long ref = 0);
    hkbGetHandleOnBoneModifier& operator=(const hkbGetHandleOnBoneModifier&) = delete;
    hkbGetHandleOnBoneModifier(const hkbGetHandleOnBoneModifier &) = delete;
    ~hkbGetHandleOnBoneModifier();
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
    HkxSharedPtr handleOut;
    QString localFrameName;
    int ragdollBoneIndex;
    int animationBoneIndex;
    mutable std::mutex mutex;
};

#endif // HKBGETHANDLEONBONEMODIFIER_H
