#ifndef BSPASSBYTARGETTRIGGERMODIFIER_H
#define BSPASSBYTARGETTRIGGERMODIFIER_H

#include "hkbmodifier.h"

class BSPassByTargetTriggerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSPassByTargetTriggerModifier(BehaviorFile *parent, long ref = 0);
    virtual ~BSPassByTargetTriggerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
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
    int id;
    HkxObjectExpSharedPtr payload;
};

#endif // BSPASSBYTARGETTRIGGERMODIFIER_H
