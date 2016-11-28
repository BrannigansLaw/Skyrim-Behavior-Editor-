#ifndef HKBTWISTMODIFIER_H
#define HKBTWISTMODIFIER_H

#include "hkbmodifier.h"

class hkbTwistModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbTwistModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbTwistModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbTwistModifier& operator=(const hkbTwistModifier&);
    hkbTwistModifier(const hkbTwistModifier &);
private:
    static QStringList SetAngleMethod;  //(LINEAR=0;RAMPED=1)
    static QStringList RotationAxisCoordinates; //(ROTATION_AXIS_IN_MODEL_COORDINATES=0;ROTATION_AXIS_IN_LOCAL_COORDINATES=1)
    static uint refCount;
    static QString classname;
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
};

#endif // HKBTWISTMODIFIER_H
