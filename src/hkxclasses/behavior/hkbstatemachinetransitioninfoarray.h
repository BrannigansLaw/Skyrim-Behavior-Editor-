#ifndef HKBSTATEMACHINETRANSITIONINFOARRAY_H
#define HKBSTATEMACHINETRANSITIONINFOARRAY_H

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/behavior/hkbexpressioncondition.h"

class hkbStateMachine;

class hkbStateMachineTransitionInfoArray: public HkxObject
{
    friend class hkbStateMachine;
    friend class hkbStateMachineStateInfo;
    friend class StateMachineUI;
    friend class StateUI;
public:
    hkbStateMachineTransitionInfoArray(HkxFile *parent, hkbStateMachine *parentSM, long ref = -1);
    virtual ~hkbStateMachineTransitionInfoArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QString evaluateDataValidity();
    static QString getClassname();
    QString getTransitionNameAt(int index);
    int getNumTransitions() const;
    void addTransition();
    void removeTransition(int index);
    void removeTransitionToState(ulong stateId);
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    bool merge(HkxObject *recessiveObject);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    struct HkTransition
    {
        HkTransition()
            : eventId(-1),
              toStateId(0),
              fromNestedStateId(0),
              toNestedStateId(0),
              priority(0),
              flags("0")
        {
            //
        }

        bool operator==(const HkTransition & other) const;

        bool usingTriggerInterval() const{
            if (
                    triggerInterval.enterEventId > -1 ||
                    triggerInterval.exitEventId > -1 ||
                    triggerInterval.enterTime > 0.000000 ||
                    triggerInterval.exitTime > 0.000000
                    )
            {
                return true;
            }
            return false;
        }

        bool usingInitiateInterval() const{
            if (
                    initiateInterval.enterEventId > -1 ||
                    initiateInterval.exitEventId > -1 ||
                    initiateInterval.enterTime > 0.000000 ||
                    initiateInterval.exitTime > 0.000000
                    )
            {
                return true;
            }
            return false;
        }

        struct HkInterval
        {
            HkInterval()
                : enterEventId(-1),
                  exitEventId(-1),
                  enterTime(0),
                  exitTime(0)
            {
                //
            }
            bool operator!=(const HkInterval & other) const{
                if (
                        enterEventId == other.enterEventId ||
                        exitEventId == other.exitEventId ||
                        enterTime == other.enterTime ||
                        exitTime == other.exitTime
                   )
                {
                    return false;
                }
                return true;
            }
            int enterEventId;
            int exitEventId;
            qreal enterTime;
            qreal exitTime;
        };

        HkInterval triggerInterval;
        HkInterval initiateInterval;
        HkxSharedPtr transition;
        HkxSharedPtr condition;
        int eventId;
        ulong toStateId;
        ulong fromNestedStateId;
        ulong toNestedStateId;
        int priority;
        QString flags;
    };
protected:
private:
    enum INTERVAL_TYPE {
        NONE = -1,
        TRIGGER = 0,
        INITIATE = 1
    };
    static QStringList transitionFlags; //(FLAG_USE_TRIGGER_INTERVAL=1;FLAG_USE_INITIATE_INTERVAL=2;FLAG_UNINTERRUPTIBLE_WHILE_PLAYING=4;FLAG_UNINTERRUPTIBLE_WHILE_DELAYED=8;FLAG_DELAY_STATE_CHANGE=16;FLAG_DISABLED=32;FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE=64;FLAG_DISALLOW_RANDOM_TRANSITION=128;FLAG_DISABLE_CONDITION=256;FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE=512;FLAG_IS_GLOBAL_WILDCARD=1024;FLAG_IS_LOCAL_WILDCARD=2048;FLAG_FROM_NESTED_STATE_ID_IS_VALID=4096;FLAG_TO_NESTED_STATE_ID_IS_VALID=8192;FLAG_ABUT_AT_END_OF_FROM_GENERATOR=16384)
    static uint refCount;
    static QString classname;
    QList <HkTransition> transitions;
    HkxObject *parent;
};

#endif // HKBSTATEMACHINETRANSITIONINFOARRAY_H
