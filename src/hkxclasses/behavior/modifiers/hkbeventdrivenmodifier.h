#ifndef HKBEVENTDRIVENMODIFIER_H
#define HKBEVENTDRIVENMODIFIER_H

#include "hkbmodifier.h"

class hkbEventDrivenModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class EventDrivenModifierUI;
public:
    hkbEventDrivenModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbEventDrivenModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbEventDrivenModifier& operator=(const hkbEventDrivenModifier&);
    hkbEventDrivenModifier(const hkbEventDrivenModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr modifier;
    int activateEventId;
    int deactivateEventId;
    bool activeByDefault;
};

#endif // HKBEVENTDRIVENMODIFIER_H
