#ifndef HKBSTATEMACHINETRANSITIONINFOARRAY_H
#define HKBSTATEMACHINETRANSITIONINFOARRAY_H

#include "hkxobject.h"

class hkbStateMachine;

class hkbStateMachineTransitionInfoArray: public HkxObject
{
    friend class hkbStateMachine;
    friend class hkbStateMachineStateInfo;
public:
    hkbStateMachineTransitionInfoArray(BehaviorFile *parent = NULL, hkbStateMachine *parentSM = NULL);
    virtual ~hkbStateMachineTransitionInfoArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    QString getTransitionNameAt(int index);
    int getNumTransitions() const;
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
protected:
private:
    static QStringList transitionFlags; //(FLAG_USE_TRIGGER_INTERVAL=1;FLAG_USE_INITIATE_INTERVAL=2;FLAG_UNINTERRUPTIBLE_WHILE_PLAYING=4;FLAG_UNINTERRUPTIBLE_WHILE_DELAYED=8;FLAG_DELAY_STATE_CHANGE=16;FLAG_DISABLED=32;FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE=64;FLAG_DISALLOW_RANDOM_TRANSITION=128;FLAG_DISABLE_CONDITION=256;FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE=512;FLAG_IS_GLOBAL_WILDCARD=1024;FLAG_IS_LOCAL_WILDCARD=2048;FLAG_FROM_NESTED_STATE_ID_IS_VALID=4096;FLAG_TO_NESTED_STATE_ID_IS_VALID=8192;FLAG_ABUT_AT_END_OF_FROM_GENERATOR=16384)
    static uint refCount;
    static QString classname;
    QList <HkTransition> transitions;
    HkxObjectExpSharedPtr parent;
};

#endif // HKBSTATEMACHINETRANSITIONINFOARRAY_H
