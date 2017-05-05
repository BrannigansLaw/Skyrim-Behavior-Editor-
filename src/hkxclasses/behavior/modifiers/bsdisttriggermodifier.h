#ifndef BSDISTTRIGGERMODIFIER_H
#define BSDISTTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSDistTriggerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSDistTriggerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSDistTriggerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
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
