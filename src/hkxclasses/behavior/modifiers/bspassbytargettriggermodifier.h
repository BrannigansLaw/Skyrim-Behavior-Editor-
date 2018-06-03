#ifndef BSPASSBYTARGETTRIGGERMODIFIER_H
#define BSPASSBYTARGETTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSPassByTargetTriggerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSPassByTargetTriggerModifierUI;
public:
    BSPassByTargetTriggerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSPassByTargetTriggerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
private:
    BSPassByTargetTriggerModifier& operator=(const BSPassByTargetTriggerModifier&);
    BSPassByTargetTriggerModifier(const BSPassByTargetTriggerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable targetPosition;
    qreal radius;
    hkQuadVariable movementDirection;
    hkEventPayload triggerEvent;
};

#endif // BSPASSBYTARGETTRIGGERMODIFIER_H
