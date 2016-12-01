#ifndef HKBSTATEMACHINEEVENTPROPERTYARRAY_H
#define HKBSTATEMACHINEEVENTPROPERTYARRAY_H

#include "hkxobject.h"

class hkbStateMachineEventPropertyArray: public HkxObject
{
    friend class StateUI;
public:
    hkbStateMachineEventPropertyArray(BehaviorFile *parent, long ref = -1);
    virtual ~hkbStateMachineEventPropertyArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    int getLastEventIndex() const;
    bool write(HkxXMLWriter *writer);
protected:
private:
    struct HkEvent
    {
        HkEvent(): id(-1){}
        int id;
        HkxObjectExpSharedPtr payload;
    };
    void addEvent(const HkEvent & event = HkEvent());
    void setEventId(int index, int id);
    void removeEvent(int index);
private:
    static uint refCount;
    static QString classname;
    QList <HkEvent> events;
};

#endif // HKBSTATEMACHINEEVENTPROPERTYARRAY_H
