#ifndef HKBTWISTMODIFIER_H
#define HKBTWISTMODIFIER_H

#include "hkbmodifier.h"

class hkbTwistModifier final: public hkbModifier
{
    friend class TwistModifierUI;
public:
    hkbTwistModifier(HkxFile *parent, long ref = 0);
    hkbTwistModifier& operator=(const hkbTwistModifier&) = delete;
    hkbTwistModifier(const hkbTwistModifier &) = delete;
    ~hkbTwistModifier();
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
    static const QStringList SetAngleMethod;
    static const QStringList RotationAxisCoordinates;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable axisOfRotation;
    qreal twistAngle;
    int startBoneIndex;
    int endBoneIndex;
    QString setAngleMethod;
    QString rotationAxisCoordinates;
    bool isAdditive;
    mutable std::mutex mutex;
};

#endif // HKBTWISTMODIFIER_H
