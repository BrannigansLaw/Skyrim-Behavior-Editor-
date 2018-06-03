#ifndef BSRAGDOLLCONTACTLISTENERMODIFIER_H
#define BSRAGDOLLCONTACTLISTENERMODIFIER_H

#include "hkbmodifier.h"

class BSRagdollContactListenerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSRagdollContactListenerModifierUI;
public:
    BSRagdollContactListenerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSRagdollContactListenerModifier();
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
    BSRagdollContactListenerModifier& operator=(const BSRagdollContactListenerModifier&);
    BSRagdollContactListenerModifier(const BSRagdollContactListenerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload contactEvent;
    HkxSharedPtr bones;
};

#endif // BSRAGDOLLCONTACTLISTENERMODIFIER_H
