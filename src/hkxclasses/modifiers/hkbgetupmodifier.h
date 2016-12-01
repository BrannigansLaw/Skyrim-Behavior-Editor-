#ifndef HKBGETUPMODIFIER_H
#define HKBGETUPMODIFIER_H

#include "hkbmodifier.h"

class hkbGetUpModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbGetUpModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbGetUpModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbGetUpModifier& operator=(const hkbGetUpModifier&);
    hkbGetUpModifier(const hkbGetUpModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable groundNormal;
    qreal duration;
    qreal alignWithGroundDuration;
    int rootBoneIndex;
    int otherBoneIndex;
    int anotherBoneIndex;
};

#endif // HKBGETUPMODIFIER_H
