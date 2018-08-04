#ifndef BSEVENTONDEACTIVATEMODIFIER_H
#define BSEVENTONDEACTIVATEMODIFIER_H

#include "hkbmodifier.h"

class BSEventOnDeactivateModifier final: public hkbModifier
{
    friend class BSEventOnDeactivateModifierUI;
public:
    BSEventOnDeactivateModifier(HkxFile *parent, long ref = 0);
    BSEventOnDeactivateModifier& operator=(const BSEventOnDeactivateModifier&) = delete;
    BSEventOnDeactivateModifier(const BSEventOnDeactivateModifier &) = delete;
    ~BSEventOnDeactivateModifier();
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
    hkEventPayload event;
    mutable std::mutex mutex;
};

#endif // BSEVENTONDEACTIVATEMODIFIER_H
