#ifndef HKBSTATEMACHINETRANSITIONINFOARRAY_H
#define HKBSTATEMACHINETRANSITIONINFOARRAY_H

#include "hkxobject.h"

class hkbStateMachineTransitionInfoArray: public HkxObject
{
public:
    hkbStateMachineTransitionInfoArray(BehaviorFile *parent = NULL);
    virtual ~hkbStateMachineTransitionInfoArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
protected:
private:
    struct HkTransition
    {
        HkTransition(): eventId(-1), toStateId(0), fromNestedStateId(0), toNestedStateId(0), priority(0){}
        struct HkInterval
        {
            HkInterval(): enterEventId(-1), exitEventId(-1), enterTime(0), exitTime(0){}
            int enterEventId;
            int exitEventId;
            qreal enterTime;
            qreal exitTime;
        };

        HkInterval triggerInterval;
        HkInterval initiateInterval;
        HkxObjectExpSharedPtr transition;
        HkxObjectExpSharedPtr condition;
        int eventId;
        int toStateId;
        int fromNestedStateId;
        int toNestedStateId;
        int priority;
        QString flags;
    };

    static uint refCount;
    static QString classname;
    //QVector <HkTransition> transitions;
};

#endif // HKBSTATEMACHINETRANSITIONINFOARRAY_H
