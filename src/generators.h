#ifndef GENERATORS_H
#define GENERATORS_H

#include "hkobject.h"

#include <QtGlobal>

class hkbGenerator: public HkDynamicObject
{
public:
    virtual ~hkbGenerator(){}
protected:
    hkbGenerator(HkxFile *parent = NULL/*, long ref = 0*/): HkDynamicObject(parent/*, ref*/){}
};

class hkbGeneratorExpSharedPtr: public QSharedDataPointer <hkbGenerator>
{
public:
    hkbGeneratorExpSharedPtr(hkbGenerator *obj = NULL, long ref = -1)
        :QSharedDataPointer(obj), reference(ref)
    {
        //
    }
    hkbGeneratorExpSharedPtr operator=(const hkbGeneratorExpSharedPtr & obj){
        static_cast<QSharedDataPointer>(*this) = static_cast<const QSharedDataPointer<hkbGenerator>>(obj);
        reference = obj.reference;
    }
    void setReference(long ref){reference = ref;}
    long getReference()const{return reference;}
    bool readReference(long index, const HkxXmlReader & reader);
private:
    long reference;
};

class hkbStateMachineStateInfo: public HkDynamicObject
{
public:
    hkbStateMachineStateInfo(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbStateMachineStateInfo(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbStateMachineStateInfo& operator=(const hkbStateMachineStateInfo&);
    hkbStateMachineStateInfo(const hkbStateMachineStateInfo &);
private:
    static uint refCount;
    HkObjectExpSharedPtr enterNotifyEvents;
    HkObjectExpSharedPtr exitNotifyEvents;
    HkObjectExpSharedPtr transitions;
    hkbGeneratorExpSharedPtr generator;
    QString name;
    ushort stateId;
    qreal probability;
    bool enable;
};

class hkbStateMachine: public HkDynamicObject
{
public:
    hkbStateMachine(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbStateMachine(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbStateMachine& operator=(const hkbStateMachine&);
    hkbStateMachine(const hkbStateMachine &);
private:
    static QStringList StartStateMode;  //{START_STATE_MODE_DEFAULT=0, START_STATE_MODE_SYNC=1, START_STATE_MODE_RANDOM=2, START_STATE_MODE_CHOOSER=3};
    static QStringList SelfTransitionMode;  //{SELF_TRANSITION_MODE_NO_TRANSITION=0, SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE=1, SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE=2};
    static uint refCount;
    ulong userData;
    QString name;
    HkObjectExpSharedPtr eventToSendWhenStateOrTransitionChanges;
    int startStateId;
    int returnToPreviousStateEventId;
    int randomTransitionEventId;
    int transitionToNextHigherStateEventId;
    int transitionToNextLowerStateEventId;
    int syncVariableIndex;
    bool wrapAroundStateId;
    ushort maxSimultaneousTransitions;  //Max 32, min 0.
    QString startStateMode;
    QString selfTransitionMode;
    QList <hkbGeneratorExpSharedPtr> states;
    HkObjectExpSharedPtr wildcardTransitions;
};

//Do not allow variables to be bound to this class in the editor...
class hkbModifierGenerator: public HkDynamicObject
{
public:
    hkbModifierGenerator(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbModifierGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbModifierGenerator& operator=(const hkbModifierGenerator&);
    hkbModifierGenerator(const hkbModifierGenerator &);
private:
    static uint refCount;
    long userData;
    QString name;
    HkObjectExpSharedPtr modifier;
    hkbGeneratorExpSharedPtr generator;
};

class hkbManualSelectorGenerator: public HkDynamicObject
{
public:
    hkbManualSelectorGenerator(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbManualSelectorGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbManualSelectorGenerator& operator=(const hkbManualSelectorGenerator&);
    hkbManualSelectorGenerator(const hkbManualSelectorGenerator &);
private:
    static uint refCount;
    long userData;
    QString name;
    QList <hkbGeneratorExpSharedPtr> generators;
    qint8 selectedGeneratorIndex;
    qint8 currentGeneratorIndex;
};

class hkbBlenderGeneratorChild: public HkDynamicObject
{
public:
    hkbBlenderGeneratorChild(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBlenderGeneratorChild(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbBlenderGeneratorChild& operator=(const hkbBlenderGeneratorChild&);
    hkbBlenderGeneratorChild(const hkbBlenderGeneratorChild &);
private:
    static uint refCount;
    hkbGeneratorExpSharedPtr generator;
    HkObjectExpSharedPtr boneWeights;
    qreal weight;
    qreal worldFromModelWeight;
};

class hkbBlenderGenerator: public HkDynamicObject
{
public:
    hkbBlenderGenerator(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBlenderGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    static QStringList Flags;   //{FLAG_SYNC=1, FLAG_SMOOTH_GENERATOR_WEIGHTS=4, FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS=8, FLAG_PARAMETRIC_BLEND=16, FLAG_IS_PARAMETRIC_BLEND_CYCLIC=32, FLAG_FORCE_DENSE_POSE=64};
    static uint refCount;
    ulong userData;
    QString name;
    qreal referencePoseWeightThreshold;
    qreal blendParameter;
    qreal minCyclicBlendParameter;
    qreal maxCyclicBlendParameter;
    qint16 indexOfSyncMasterChild;
    QString flags;
    bool subtractLastChild;
    QList <hkbGeneratorExpSharedPtr> children;
};

class hkbBehaviorReferenceGenerator: public HkDynamicObject
{
public:
    hkbBehaviorReferenceGenerator(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorReferenceGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbBehaviorReferenceGenerator& operator=(const hkbBehaviorReferenceGenerator&);
    hkbBehaviorReferenceGenerator(const hkbBehaviorReferenceGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    QString behaviorName;
};

class hkbClipGenerator: public HkDynamicObject
{
public:
    hkbClipGenerator(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbClipGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbClipGenerator& operator=(const hkbClipGenerator&);
    hkbClipGenerator(const hkbClipGenerator &);
private:
    static QStringList PlaybackMode;    //{MODE_SINGLE_PLAY=0,MODE_LOOPING=1,MODE_USER_CONTROLLED=2,MODE_PING_PONG=3,MODE_COUNT=4};
    static QStringList ClipFlags;   //(FLAG_CONTINUE_MOTION_AT_END=1;FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE=2;FLAG_MIRROR=4;FLAG_FORCE_DENSE_POSE=8;FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS=16;FLAG_IGNORE_MOTION=32)*/
    static uint refCount;
    ulong userData;
    QString name;
    QString animationName;
    HkObjectExpSharedPtr triggers;
    qreal cropStartAmountLocalTime;
    qreal cropEndAmountLocalTime;
    qreal startTime;
    qreal playbackSpeed;
    qreal enforcedDuration;
    qreal userControlledTimeFraction;
    qint16 animationBindingIndex;
    QString mode;
    QString flags;
};

class hkbBehaviorGraph: public HkDynamicObject
{
public:
    hkbBehaviorGraph(HkxFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraph(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
private:
    hkbBehaviorGraph& operator=(const hkbBehaviorGraph&);
    hkbBehaviorGraph(const hkbBehaviorGraph &);
private:
    static uint refCount;
    static QStringList VariableMode;    //{VARIABLE_MODE_DISCARD_WHEN_INACTIVE=0, VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE=1};
    qint64 userData;
    QString name;
    QString variableMode;
    hkbGeneratorExpSharedPtr rootGenerator;
    HkObjectExpSharedPtr data;
};

#endif // GENERATORS_H
