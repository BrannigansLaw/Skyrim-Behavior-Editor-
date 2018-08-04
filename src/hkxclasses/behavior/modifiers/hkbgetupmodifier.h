#ifndef HKBGETUPMODIFIER_H
#define HKBGETUPMODIFIER_H

#include "hkbmodifier.h"

class hkbGetUpModifier final: public hkbModifier
{
    friend class GetUpModifierUI;
public:
    hkbGetUpModifier(HkxFile *parent, long ref = 0);
    hkbGetUpModifier& operator=(const hkbGetUpModifier&) = delete;
    hkbGetUpModifier(const hkbGetUpModifier &) = delete;
    ~hkbGetUpModifier();
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
    hkQuadVariable groundNormal;
    qreal duration;
    qreal alignWithGroundDuration;
    int rootBoneIndex;
    int otherBoneIndex;
    int anotherBoneIndex;
    mutable std::mutex mutex;
};

#endif // HKBGETUPMODIFIER_H
