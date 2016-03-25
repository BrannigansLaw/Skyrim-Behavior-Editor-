#include "generators.h"
#include "hkxxmlreader.h"

/*
 * CLASS: hkbGeneratorExpSharedPtr
*/

bool hkbGeneratorExpSharedPtr::readReference(long index, const HkxXmlReader & reader){
    bool ok = true;
    //need to remove the '#' from the reference string
    QByteArray temp = reader.getElementValueAt(index);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (qstrcmp(temp.constData(), "null") == 0){
        setReference(-1);
    }else{
        setReference(temp.toLong(&ok));
    }
    if (!ok){
        return false;
    }
    return true;
}

/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

hkbStateMachineStateInfo::hkbStateMachineStateInfo(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_STATE_MACHINE_STATE_INFO, TYPE_GENERATOR);
    refCount++;
    stateId = 0;
    probability = 1;
    enable = true;
}

bool hkbStateMachineStateInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text(15, '\0');
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(text.constData(), "variableBindingSet") == 0){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (qstrcmp(text.constData(), "enterNotifyEvents") == 0){
            if (!enterNotifyEvents.readReference(index, reader)){
                return false;
            }
        }else if (qstrcmp(text.constData(), "exitNotifyEvents") == 0){
            if (!exitNotifyEvents.readReference(index, reader)){
                return false;
            }
        }else if (qstrcmp(text.constData(), "transitions") == 0){
            if (!transitions.readReference(index, reader)){
                return false;
            }
        }else if (qstrcmp(text.constData(), "generator") == 0){
            if (!generator.readReference(index, reader)){
                return false;
            }
        }else if (qstrcmp(text.constData(), "name") == 0){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (qstrcmp(text.constData(), "stateId") == 0){
            stateId = readSingleInt(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (qstrcmp(text.constData(), "probability") == 0){
            probability = readSingleDouble(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (qstrcmp(text.constData(), "enable") == 0){
            enable = readSingleBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_STATE_MACHINE, TYPE_GENERATOR);
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

bool hkbStateMachine::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

hkbModifierGenerator::hkbModifierGenerator(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_MODIFIER_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbModifierGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbManualSelectorGenerator
*/

uint hkbManualSelectorGenerator::refCount = 0;

hkbManualSelectorGenerator::hkbManualSelectorGenerator(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_MANUAL_SELECTOR_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
    selectedGeneratorIndex = 0;
    currentGeneratorIndex = 0;
}

bool hkbManualSelectorGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BLENDER_GENERATOR_CHILD, TYPE_GENERATOR);
    refCount++;
    weight = 0;
    worldFromModelWeight = 0;
}

bool hkbBlenderGeneratorChild::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbBlenderGenerator
*/

uint hkbBlenderGenerator::refCount = 0;

QStringList hkbBlenderGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};

hkbBlenderGenerator::hkbBlenderGenerator(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BLENDER_GENERATOR, TYPE_GENERATOR);
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

bool hkbBlenderGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BEHAVIOR_REFERENCE_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbBehaviorReferenceGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbClipGenerator
*/

uint hkbClipGenerator::refCount = 0;

QStringList hkbClipGenerator::PlaybackMode = {"MODE_SINGLE_PLAY", "MODE_LOOPING", "MODE_USER_CONTROLLED", "MODE_PING_PONG", "MODE_COUNT"};
QStringList hkbClipGenerator::ClipFlags = {"0", "FLAG_CONTINUE_MOTION_AT_END", "FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE", "FLAG_MIRROR", "FLAG_FORCE_DENSE_POSE", "FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS", "FLAG_IGNORE_MOTION"};

hkbClipGenerator::hkbClipGenerator(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_CLIP_GENERATOR, TYPE_GENERATOR);
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

bool hkbClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(HkxFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbBehaviorGraph::readData(const HkxXmlReader &reader, long index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (qstrcmp(reader.getNthAttributeValueAt(index, 0).constData(), "namedVariants") == 0){
            //
        }
        index++;
    }
    return true;
}























