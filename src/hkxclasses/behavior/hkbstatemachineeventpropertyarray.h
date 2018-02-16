#ifndef HKBSTATEMACHINEEVENTPROPERTYARRAY_H
#define HKBSTATEMACHINEEVENTPROPERTYARRAY_H

#include "src/hkxclasses/hkxobject.h"

class hkbStateMachineEventPropertyArray: public HkxObject
{
    friend class StateUI;
public:
    hkbStateMachineEventPropertyArray(HkxFile *parent, long ref = -1);
    virtual ~hkbStateMachineEventPropertyArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    int getLastEventIndex() const;
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
protected:
private:
    void addEvent(const hkEventPayload & event = hkEventPayload());
    void setEventId(int index, int id);
    void removeEvent(int index);
private:
    static uint refCount;
    static QString classname;
    QList <hkEventPayload> events;
};

#endif // HKBSTATEMACHINEEVENTPROPERTYARRAY_H
