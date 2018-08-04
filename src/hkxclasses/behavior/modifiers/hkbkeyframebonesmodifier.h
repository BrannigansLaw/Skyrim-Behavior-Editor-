#ifndef HKBKEYFRAMEBONESMODIFIER_H
#define HKBKEYFRAMEBONESMODIFIER_H

#include "hkbmodifier.h"

class hkbKeyframeBonesModifier final: public hkbModifier
{
    friend class KeyframeInfoUI;
    friend class KeyframeBonesModifierUI;
public:
    hkbKeyframeBonesModifier(HkxFile *parent, long ref = 0);
    hkbKeyframeBonesModifier& operator=(const hkbKeyframeBonesModifier&) = delete;
    hkbKeyframeBonesModifier(const hkbKeyframeBonesModifier &) = delete;
    ~hkbKeyframeBonesModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfKeyframeInfos() const;
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    struct hkKeyframeInfo{
        hkKeyframeInfo() : boneIndex(-1), isValid(false){}
        hkQuadVariable keyframedPosition;
        hkQuadVariable keyframedRotation;
        int boneIndex;
        bool isValid;
    };
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    QVector <hkKeyframeInfo> keyframeInfo;
    HkxSharedPtr keyframedBonesList;
    mutable std::mutex mutex;
};

#endif // HKBKEYFRAMEBONESMODIFIER_H
