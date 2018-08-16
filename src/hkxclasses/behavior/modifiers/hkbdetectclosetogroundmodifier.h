#ifndef HKBDETECTCLOSETOGROUNDMODIFIER_H
#define HKBDETECTCLOSETOGROUNDMODIFIER_H

#include "hkbmodifier.h"

class hkbDetectCloseToGroundModifier final: public hkbModifier
{
    friend class DetectCloseToGroundModifierUI;
public:
    hkbDetectCloseToGroundModifier(HkxFile *parent, long ref = 0);
    hkbDetectCloseToGroundModifier& operator=(const hkbDetectCloseToGroundModifier&) = delete;
    hkbDetectCloseToGroundModifier(const hkbDetectCloseToGroundModifier &) = delete;
    ~hkbDetectCloseToGroundModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload closeToGroundEvent;
    qreal closeToGroundHeight;
    qreal raycastDistanceDown;
    int collisionFilterInfo;
    int boneIndex;
    int animBoneIndex;
    mutable std::mutex mutex;
};

#endif // HKBDETECTCLOSETOGROUNDMODIFIER_H
