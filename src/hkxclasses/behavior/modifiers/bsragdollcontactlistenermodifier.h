#ifndef BSRAGDOLLCONTACTLISTENERMODIFIER_H
#define BSRAGDOLLCONTACTLISTENERMODIFIER_H

#include "hkbmodifier.h"

class BSRagdollContactListenerModifier final: public hkbModifier
{
    friend class BSRagdollContactListenerModifierUI;
public:
    BSRagdollContactListenerModifier(HkxFile *parent, long ref = 0);
    BSRagdollContactListenerModifier& operator=(const BSRagdollContactListenerModifier&) = delete;
    BSRagdollContactListenerModifier(const BSRagdollContactListenerModifier &) = delete;
    ~BSRagdollContactListenerModifier();
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
    hkEventPayload contactEvent;
    HkxSharedPtr bones;
    mutable std::mutex mutex;
};

#endif // BSRAGDOLLCONTACTLISTENERMODIFIER_H
