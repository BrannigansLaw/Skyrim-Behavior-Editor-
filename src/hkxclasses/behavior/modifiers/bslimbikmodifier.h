#ifndef BSLIMBIKMODIFIER_H
#define BSLIMBIKMODIFIER_H

#include "hkbmodifier.h"

class BSLimbIKModifier final: public hkbModifier
{
    friend class BSLimbIKModifierUI;
public:
    BSLimbIKModifier(HkxFile *parent, long ref = 0);
    BSLimbIKModifier& operator=(const BSLimbIKModifier&) = delete;
    BSLimbIKModifier(const BSLimbIKModifier &) = delete;
    ~BSLimbIKModifier();
public:
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
    qreal limitAngleDegrees;
    int startBoneIndex;
    int endBoneIndex;
    qreal gain;
    qreal boneRadius;
    qreal castOffset;
    mutable std::mutex mutex;
};

#endif // BSLIMBIKMODIFIER_H
