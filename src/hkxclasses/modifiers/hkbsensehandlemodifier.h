#ifndef HKBSENSEHANDLEMODIFIER_H
#define HKBSENSEHANDLEMODIFIER_H

#include "hkbmodifier.h"

class hkbSenseHandleModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbSenseHandleModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbSenseHandleModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbSenseHandleModifier& operator=(const hkbSenseHandleModifier&);
    hkbSenseHandleModifier(const hkbSenseHandleModifier &);
private:
    struct hkRanges{
        hkRanges()
            : id(-1),
              minDistance(0),
              maxDistance(0),
              ignoreHandle(false)
        {
            //
        }

        int id;
        HkxObjectExpSharedPtr payload;
        qreal minDistance;
        qreal maxDistance;
        bool ignoreHandle;
    };

    static QStringList SensingMode; //(SENSE_IN_NEARBY_RIGID_BODIES=0;SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER=1;SENSE_IN_OTHER_CHARACTER_RIGID_BODIES=2;SENSE_IN_THIS_CHARACTER_RIGID_BODIES=3;SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES=4;SENSE_IN_GIVEN_RIGID_BODY=5;SENSE_IN_OTHER_CHARACTER_SKELETON=6;SENSE_IN_THIS_CHARACTER_SKELETON=7;SENSE_IN_GIVEN_CHARACTER_SKELETON=8;SENSE_IN_GIVEN_LOCAL_FRAME_GROUP=9)
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable sensorLocalOffset;
    QList <hkRanges> ranges;
    HkxObjectExpSharedPtr handleOut;
    HkxObjectExpSharedPtr handleIn;
    QString localFrameName;
    QString sensorLocalFrameName;
    qreal minDistance;
    qreal maxDistance;
    qreal distanceOut;
    int collisionFilterInfo;
    int sensorRagdollBoneIndex;
    int sensorAnimationBoneIndex;
    QString sensingMode;
    bool extrapolateSensorPosition;
    bool keepFirstSensedHandle;
    bool foundHandleOut;
};

#endif // HKBSENSEHANDLEMODIFIER_H
