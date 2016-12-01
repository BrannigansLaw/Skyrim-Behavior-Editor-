#ifndef HKBEVENTDRIVENMODIFIER_H
#define HKBEVENTDRIVENMODIFIER_H

#include "hkbmodifier.h"

class hkbEventDrivenModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbEventDrivenModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbEventDrivenModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
protected:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder = true);
private:
    hkbEventDrivenModifier& operator=(const hkbEventDrivenModifier&);
    hkbEventDrivenModifier(const hkbEventDrivenModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxObjectExpSharedPtr modifier;
    int activateEventId;
    int deactivateEventId;
    bool activeByDefault;
};

#endif // HKBEVENTDRIVENMODIFIER_H
