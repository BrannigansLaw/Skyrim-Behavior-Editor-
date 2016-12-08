#include "generators.h"

/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

hkbStateMachineStateInfo::hkbStateMachineStateInfo(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    stateId = 0;
    probability = 1;
    enable = true;
}

/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
    startStateId = -1;
    returnToPreviousStateEventId = -1;
    randomTransitionEventId = -1;
    transitionToNextHigherStateEventId = -1;
    transitionToNextLowerStateEventId = -1;
    syncVariableIndex = -1;
    wrapAroundStateId = false;
    maxSimultaneousTransitions = 32;  //Max 32, min 0.
    startStateMode = "START_STATE_MODE_DEFAULT";
    selfTransitionMode = "SELF_TRANSITION_MODE_NO_TRANSITION";
}

/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

hkbModifierGenerator::hkbModifierGenerator(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
}

/*
 * CLASS: hkbManualSelectorGenerator
*/

uint hkbManualSelectorGenerator::refCount = 0;

hkbManualSelectorGenerator::hkbManualSelectorGenerator(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
    selectedGeneratorIndex = 0;
    currentGeneratorIndex = 0;
}

/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    weight = 0;
    worldFromModelWeight = 0;
}

/*
 * CLASS: hkbBlenderGenerator
*/

uint hkbBlenderGenerator::refCount = 0;

QStringList hkbBlenderGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};

hkbBlenderGenerator::hkbBlenderGenerator(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
    referencePoseWeightThreshold = 0;
    blendParameter = 1;
    minCyclicBlendParameter = 0;
    maxCyclicBlendParameter = 0;
    indexOfSyncMasterChild = -1;
    flags = Flags.first();
    subtractLastChild = false;
}

/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
}

/*
 * CLASS: hkbClipGenerator
*/

uint hkbClipGenerator::refCount = 0;

QStringList hkbClipGenerator::PlaybackMode = {"MODE_SINGLE_PLAY", "MODE_LOOPING", "MODE_USER_CONTROLLED", "MODE_PING_PONG", "MODE_COUNT"};
QStringList hkbClipGenerator::ClipFlags = {"0", "FLAG_CONTINUE_MOTION_AT_END", "FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE", "FLAG_MIRROR", "FLAG_FORCE_DENSE_POSE", "FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS", "FLAG_IGNORE_MOTION"};

hkbClipGenerator::hkbClipGenerator(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
    cropStartAmountLocalTime = 0;
    cropEndAmountLocalTime = 0;
    startTime = 0;
    playbackSpeed = 0;
    enforcedDuration = 0;
    userControlledTimeFraction = 0;
    animationBindingIndex = -1;
    mode = PlaybackMode.first();
    flags = ClipFlags.first();
}

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(HkxFile *parent, qint16 ref): HkDynamicObject(parent, ref){
    refCount++;
    userData = 0;
}

























