#ifndef BSRAGDOLLCONTACTLISTENERMODIFIER_H
#define BSRAGDOLLCONTACTLISTENERMODIFIER_H

#include "hkbmodifier.h"

class BSRagdollContactListenerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSRagdollContactListenerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSRagdollContactListenerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSRagdollContactListenerModifier& operator=(const BSRagdollContactListenerModifier&);
    BSRagdollContactListenerModifier(const BSRagdollContactListenerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    int id;
    HkxObjectExpSharedPtr payload;
    HkxObjectExpSharedPtr bones;
};

#endif // BSRAGDOLLCONTACTLISTENERMODIFIER_H
