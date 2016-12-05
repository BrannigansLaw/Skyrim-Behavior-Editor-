#ifndef BSLIMBIKMODIFIER_H
#define BSLIMBIKMODIFIER_H

#include "hkbmodifier.h"

class BSLimbIKModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSLimbIKModifier(HkxFile *parent, long ref = 0);
    virtual ~BSLimbIKModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSLimbIKModifier& operator=(const BSLimbIKModifier&);
    BSLimbIKModifier(const BSLimbIKModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal limitAngleDegrees;
    int startBoneIndex;
    int endBoneIndex;
    qreal gain;
    qreal boneRadius;
    qreal castOffset;
};

#endif // BSLIMBIKMODIFIER_H
