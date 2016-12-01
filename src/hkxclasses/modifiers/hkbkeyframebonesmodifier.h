#ifndef HKBKEYFRAMEBONESMODIFIER_H
#define HKBKEYFRAMEBONESMODIFIER_H

#include "hkbmodifier.h"

class hkbKeyframeBonesModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbKeyframeBonesModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbKeyframeBonesModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbKeyframeBonesModifier& operator=(const hkbKeyframeBonesModifier&);
    hkbKeyframeBonesModifier(const hkbKeyframeBonesModifier &);
private:
    struct hkKeyframeInfo{
        hkKeyframeInfo()
            : boneIndex(-1),
              isValid(false)
        {
            //
        }

        hkQuadVariable keyframedPosition;
        hkQuadVariable keyframedRotation;
        int boneIndex;
        bool isValid;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    QList <hkKeyframeInfo> keyframeInfo;
    HkxObjectExpSharedPtr keyframedBonesList;
};

#endif // HKBKEYFRAMEBONESMODIFIER_H
