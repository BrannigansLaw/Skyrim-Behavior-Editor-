#ifndef GENERATORS_H
#define GENERATORS_H

#include "hkobject.h"

#include <QtGlobal>

/**
 * Adding new generator classes:
 *
 * Need to add to hkbGenerator::link()
 * Need to add to MainWindow::drawBehaviorGraph()
 * Need to add to BehaviorFile::parse()
 */

class hkbGenerator: public HkDynamicObject
{
    friend class BehaviorGraphView;
public:
    virtual ~hkbGenerator(){}
    bool link();
protected:
    hkbGenerator(BehaviorFile *parent = NULL/*, long ref = 0*/): HkDynamicObject(parent/*, ref*/){}
private:
    QList <GeneratorIcon *> icons;
};

class hkRootLevelContainer: public HkObject
{
    friend class BehaviorGraphView;
public:
    hkRootLevelContainer(BehaviorFile *parent = NULL/*, long ref = 0*/): HkObject(parent/*, ref*/){refCount++;setType(HK_ROOT_LEVEL_CONTAINER, TYPE_OTHER);}
    virtual ~hkRootLevelContainer(){refCount--;}
    //bool readData(const HkxXmlReader & reader, int startIndex);
    bool link();
    bool readData(const HkxXmlReader & reader, int index);
private:
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
private:
    static uint refCount;
    struct hkRootLevelContainerNamedVariant
    {
        hkRootLevelContainerNamedVariant(QString varname = "default", QString classname = "default"):name(varname), className(classname){}
        QString name;
        QString className;
        HkObjectExpSharedPtr variant;  //This can be one of many (any?) types.
    };
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

class hkbStateMachineStateInfo: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbStateMachineStateInfo(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbStateMachineStateInfo(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QString getName() const{return name;}
private:
    hkbStateMachineStateInfo& operator=(const hkbStateMachineStateInfo&);
    hkbStateMachineStateInfo(const hkbStateMachineStateInfo &);
private:
    static uint refCount;
    HkObjectExpSharedPtr enterNotifyEvents;
    HkObjectExpSharedPtr exitNotifyEvents;
    HkObjectExpSharedPtr transitions;
    HkObjectExpSharedPtr generator;
    QString name;
    ushort stateId;
    qreal probability;
    bool enable;
};

class hkbStateMachine: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbStateMachine(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbStateMachine(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QString getName() const{return name;}
private:
    hkbStateMachine& operator=(const hkbStateMachine&);
    hkbStateMachine(const hkbStateMachine &);
private:
    static QStringList StartStateMode;  //{START_STATE_MODE_DEFAULT=0, START_STATE_MODE_SYNC=1, START_STATE_MODE_RANDOM=2, START_STATE_MODE_CHOOSER=3};
    static QStringList SelfTransitionMode;  //{SELF_TRANSITION_MODE_NO_TRANSITION=0, SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE=1, SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE=2};
    static uint refCount;
    ulong userData;
    QString name;
    //HkObjectExpSharedPtr eventToSendWhenStateOrTransitionChanges;
    int id;
    HkObjectExpSharedPtr payload;
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
    QList <HkObjectExpSharedPtr> states;
    HkObjectExpSharedPtr wildcardTransitions;
};

//Do not allow variables to be bound to this class in the editor...
class hkbModifierGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbModifierGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbModifierGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbModifierGenerator& operator=(const hkbModifierGenerator&);
    hkbModifierGenerator(const hkbModifierGenerator &);
private:
    static uint refCount;
    long userData;
    QString name;
    HkObjectExpSharedPtr modifier;
    HkObjectExpSharedPtr generator;
};

class hkbManualSelectorGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbManualSelectorGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbManualSelectorGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbManualSelectorGenerator& operator=(const hkbManualSelectorGenerator&);
    hkbManualSelectorGenerator(const hkbManualSelectorGenerator &);
private:
    static uint refCount;
    long userData;
    QString name;
    QList <HkObjectExpSharedPtr> generators;
    qint8 selectedGeneratorIndex;
    qint8 currentGeneratorIndex;
};

class hkbBlenderGeneratorChild: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBlenderGeneratorChild(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBlenderGeneratorChild(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBlenderGeneratorChild& operator=(const hkbBlenderGeneratorChild&);
    hkbBlenderGeneratorChild(const hkbBlenderGeneratorChild &);
private:
    static uint refCount;
    HkObjectExpSharedPtr generator;
    HkObjectExpSharedPtr boneWeights;
    qreal weight;
    qreal worldFromModelWeight;
};

class hkbBlenderGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBlenderGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBlenderGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBlenderGenerator& operator=(const hkbBlenderGenerator&);
    hkbBlenderGenerator(const hkbBlenderGenerator &);
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
    QList <HkObjectExpSharedPtr> children;
};

class BSBoneSwitchGeneratorBoneData: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSBoneSwitchGeneratorBoneData(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSBoneSwitchGeneratorBoneData(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSBoneSwitchGeneratorBoneData& operator=(const BSBoneSwitchGeneratorBoneData&);
    BSBoneSwitchGeneratorBoneData(const BSBoneSwitchGeneratorBoneData &);
private:
    static uint refCount;
    HkObjectExpSharedPtr pGenerator;
    HkObjectExpSharedPtr spBoneWeight;
};

class BSBoneSwitchGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSBoneSwitchGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSBoneSwitchGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSBoneSwitchGenerator& operator=(const BSBoneSwitchGenerator&);
    BSBoneSwitchGenerator(const BSBoneSwitchGenerator &);
private:
    static uint refCount;
    long userData;
    QString name;
    HkObjectExpSharedPtr pDefaultGenerator;
    QList <HkObjectExpSharedPtr> ChildrenA;
};

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSCyclicBlendTransitionGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSCyclicBlendTransitionGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSCyclicBlendTransitionGenerator& operator=(const BSCyclicBlendTransitionGenerator&);
    BSCyclicBlendTransitionGenerator(const BSCyclicBlendTransitionGenerator &);
private:
    static QStringList BlendCurve;   //BLEND_CURVE_SMOOTH
    static uint refCount;
    ulong userData;
    QString name;
    HkObjectExpSharedPtr pBlenderGenerator;
    int eventToFreezeBlendValueId;
    HkObjectExpSharedPtr eventToFreezeBlendValuePayload;
    int eventToCrossBlendId;
    HkObjectExpSharedPtr eventToCrossBlendPayload;
    qreal fBlendParameter;
    qreal fTransitionDuration;
    QString eBlendCurve;
};

class BSiStateTaggingGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSiStateTaggingGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSiStateTaggingGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSiStateTaggingGenerator& operator=(const BSiStateTaggingGenerator&);
    BSiStateTaggingGenerator(const BSiStateTaggingGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    HkObjectExpSharedPtr pDefaultGenerator;
    int iStateToSetAs;
    int iPriority;
};

class hkbBehaviorReferenceGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorReferenceGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorReferenceGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBehaviorReferenceGenerator& operator=(const hkbBehaviorReferenceGenerator&);
    hkbBehaviorReferenceGenerator(const hkbBehaviorReferenceGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    QString behaviorName;
};

class hkbClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbClipGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbClipGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
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

class BSSynchronizedClipGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSSynchronizedClipGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSSynchronizedClipGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSSynchronizedClipGenerator& operator=(const BSSynchronizedClipGenerator&);
    BSSynchronizedClipGenerator(const BSSynchronizedClipGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    HkObjectExpSharedPtr pClipGenerator;
    QString SyncAnimPrefix;
    bool bSyncClipIgnoreMarkPlacement;
    qreal fGetToMarkTime;
    qreal fMarkErrorThreshold;
    bool bLeadCharacter;
    bool bReorientSupportChar;
    bool bApplyMotionFromRoot;
    int sAnimationBindingIndex;
};

class hkbPoseMatchingGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbPoseMatchingGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbPoseMatchingGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbPoseMatchingGenerator& operator=(const hkbPoseMatchingGenerator&);
    hkbPoseMatchingGenerator(const hkbPoseMatchingGenerator &);
private:
    static QStringList Flags;
    static QStringList Mode;
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
    QList <HkObjectExpSharedPtr> children;
    hkVector4 worldFromModelRotation;
    qreal blendSpeed;
    qreal minSpeedToSwitch;
    qreal minSwitchTimeNoError;
    qreal minSwitchTimeFullError;
    int startPlayingEventId;
    int startMatchingEventId;
    int rootBoneIndex;
    int otherBoneIndex;
    int anotherBoneIndex;
    int pelvisIndex;
    QString mode;
};

class BSOffsetAnimationGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSOffsetAnimationGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSOffsetAnimationGenerator(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    BSOffsetAnimationGenerator& operator=(const BSOffsetAnimationGenerator&);
    BSOffsetAnimationGenerator(const BSOffsetAnimationGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    HkObjectExpSharedPtr pDefaultGenerator;
    HkObjectExpSharedPtr pOffsetClipGenerator;
    qreal fOffsetVariable;
    qreal fOffsetRangeStart;
    qreal fOffsetRangeEnd;
};

class hkbBehaviorGraph: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorGraph(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraph(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBehaviorGraph& operator=(const hkbBehaviorGraph&);
    hkbBehaviorGraph(const hkbBehaviorGraph &);
private:
    static uint refCount;
    static QStringList VariableMode;    //{VARIABLE_MODE_DISCARD_WHEN_INACTIVE=0, VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE=1};
    qint64 userData;
    QString name;
    QString variableMode;
    HkObjectExpSharedPtr rootGenerator;
    HkObjectExpSharedPtr data;
};

#endif // GENERATORS_H
