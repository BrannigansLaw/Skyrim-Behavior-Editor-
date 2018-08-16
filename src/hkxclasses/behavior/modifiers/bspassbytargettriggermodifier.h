#ifndef BSPASSBYTARGETTRIGGERMODIFIER_H
#define BSPASSBYTARGETTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSPassByTargetTriggerModifier final: public hkbModifier
{
    friend class BSPassByTargetTriggerModifierUI;
public:
    BSPassByTargetTriggerModifier(HkxFile *parent, long ref = 0);
    BSPassByTargetTriggerModifier& operator=(const BSPassByTargetTriggerModifier&) = delete;
    BSPassByTargetTriggerModifier(const BSPassByTargetTriggerModifier &) = delete;
    ~BSPassByTargetTriggerModifier();
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
    qreal radius;
    hkQuadVariable movementDirection;
    hkEventPayload triggerEvent;
    mutable std::mutex mutex;
};

#endif // BSPASSBYTARGETTRIGGERMODIFIER_H
