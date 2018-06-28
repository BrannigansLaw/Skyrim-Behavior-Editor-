#ifndef BSDISTTRIGGERMODIFIER_H
#define BSDISTTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSDistTriggerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSDistTriggerModifierUI;
public:
    BSDistTriggerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSDistTriggerModifier();
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
    BSDistTriggerModifier& operator=(const BSDistTriggerModifier&);
    BSDistTriggerModifier(const BSDistTriggerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable targetPosition;
    qreal distance;
    qreal distanceTrigger;
    hkEventPayload triggerEvent;
};

#endif // BSDISTTRIGGERMODIFIER_H
