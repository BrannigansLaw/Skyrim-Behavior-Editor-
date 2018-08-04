#ifndef HKBSENSEHANDLEMODIFIER_H
#define HKBSENSEHANDLEMODIFIER_H

#include "hkbmodifier.h"

class hkbSenseHandleModifier final: public hkbModifier
{
    friend class SenseHandleModifierUI;
    friend class RangesUI;
public:
    hkbSenseHandleModifier(HkxFile *parent, long ref = 0);
    hkbSenseHandleModifier& operator=(const hkbSenseHandleModifier&) = delete;
    hkbSenseHandleModifier(const hkbSenseHandleModifier &) = delete;
    ~hkbSenseHandleModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfRanges() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    struct hkRanges{
        hkRanges() : minDistance(0), maxDistance(0), ignoreHandle(false){}
        hkEventPayload event;
        qreal minDistance;
        qreal maxDistance;
        bool ignoreHandle;
    };
private:
    static uint refCount;
    static const QString classname;
    static const QStringList SensingMode;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable sensorLocalOffset;
    QVector <hkRanges> ranges;
    HkxSharedPtr handleOut;
    HkxSharedPtr handleIn;
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
    mutable std::mutex mutex;
};

#endif // HKBSENSEHANDLEMODIFIER_H
