#ifndef HKBGETHANDLEONBONEMODIFIER_H
#define HKBGETHANDLEONBONEMODIFIER_H

#include "hkbmodifier.h"

class hkbGetHandleOnBoneModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class GetHandleOnBoneModifierUI;
public:
    hkbGetHandleOnBoneModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbGetHandleOnBoneModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbGetHandleOnBoneModifier& operator=(const hkbGetHandleOnBoneModifier&);
    hkbGetHandleOnBoneModifier(const hkbGetHandleOnBoneModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr handleOut;
    QString localFrameName;
    int ragdollBoneIndex;
    int animationBoneIndex;
};

#endif // HKBGETHANDLEONBONEMODIFIER_H
