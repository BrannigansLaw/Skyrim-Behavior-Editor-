#ifndef BSDISTTRIGGERMODIFIER_H
#define BSDISTTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSDistTriggerModifier final: public hkbModifier
{
    friend class BSDistTriggerModifierUI;
public:
    BSDistTriggerModifier(HkxFile *parent, long ref = 0);
    BSDistTriggerModifier& operator=(const BSDistTriggerModifier&) = delete;
    BSDistTriggerModifier(const BSDistTriggerModifier &) = delete;
    ~BSDistTriggerModifier();
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
    hkQuadVariable targetPosition;
    qreal distance;
    qreal distanceTrigger;
    hkEventPayload triggerEvent;
    mutable std::mutex mutex;
};

#endif // BSDISTTRIGGERMODIFIER_H
