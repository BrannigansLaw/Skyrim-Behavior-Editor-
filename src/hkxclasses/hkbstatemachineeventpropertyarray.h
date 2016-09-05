#ifndef HKBSTATEMACHINEEVENTPROPERTYARRAY_H
#define HKBSTATEMACHINEEVENTPROPERTYARRAY_H

#include "hkxobject.h"

class hkbStateMachineEventPropertyArray: public HkxObject
{
public:
    hkbStateMachineEventPropertyArray(BehaviorFile *parent = NULL);
    virtual ~hkbStateMachineEventPropertyArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
protected:
private:
    struct HkEvent
    {
        int id;
        HkxObjectExpSharedPtr payload;
    };
    static uint refCount;
    static QString classname;
    //QVector <HkEvent> events;
};

#endif // HKBSTATEMACHINEEVENTPROPERTYARRAY_H
