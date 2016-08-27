#ifndef HKBSTATEMACHINEEVENTPROPERTYARRAY_H
#define HKBSTATEMACHINEEVENTPROPERTYARRAY_H

#include "hkxobject.h"

class hkbStateMachineEventPropertyArray: public HkxObject
{
public:
    hkbStateMachineEventPropertyArray(BehaviorFile *parent = NULL);
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
protected:
private:
    struct HkEvent
    {
        int id;
        HkxObjectExpSharedPtr payload;
    };
    static uint refCount;
    //QVector <HkEvent> events;
};

#endif // HKBSTATEMACHINEEVENTPROPERTYARRAY_H
