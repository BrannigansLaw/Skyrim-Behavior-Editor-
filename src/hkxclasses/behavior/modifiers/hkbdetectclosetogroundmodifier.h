#ifndef HKBDETECTCLOSETOGROUNDMODIFIER_H
#define HKBDETECTCLOSETOGROUNDMODIFIER_H

#include "hkbmodifier.h"

class hkbDetectCloseToGroundModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class DetectCloseToGroundModifierUI;
public:
    hkbDetectCloseToGroundModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbDetectCloseToGroundModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    hkbDetectCloseToGroundModifier& operator=(const hkbDetectCloseToGroundModifier&);
    hkbDetectCloseToGroundModifier(const hkbDetectCloseToGroundModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload closeToGroundEvent;
    qreal closeToGroundHeight;
    qreal raycastDistanceDown;
    int collisionFilterInfo;
    int boneIndex;
    int animBoneIndex;
};

#endif // HKBDETECTCLOSETOGROUNDMODIFIER_H
