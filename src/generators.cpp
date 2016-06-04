#include "generators.h"
#include "hkxxmlreader.h"
#include "hkxfile.h"


bool readReferences(const QByteArray &line, QList <HkObjectExpSharedPtr> & children){
    qint16 size = 0;
    qint16 start;
    bool ok = true;
    for (qint16 i = 0; i < line.size(); i++){
        if (line.at(i) == '#'){
            i++;
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            children.append(HkObjectExpSharedPtr(NULL, value.toLong(&ok)));
            if (!ok){
                return false;
            }
        }else if (line.at(i) == 'n'){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            if (value == "null"){
                //refs.append(0);
            } else {
                return false;
            }
        }
    }
    return ok;
}

/**
 * hkbGenerator
 */

bool hkbGenerator::link(){
    qulonglong sig = getSignature();
    switch (sig) {
    case HKB_STATE_MACHINE_STATE_INFO:
        if (!reinterpret_cast<hkbStateMachineStateInfo *>(this)->link()){
            return false;
        }
        break;
    case HKB_STATE_MACHINE:
        if (!reinterpret_cast<hkbStateMachine *>(this)->link()){
            return false;
        }
        break;
    case HKB_MANUAL_SELECTOR_GENERATOR:
        if (!reinterpret_cast<hkbManualSelectorGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_BLENDER_GENERATOR_CHILD:
        if (!reinterpret_cast<hkbBlenderGeneratorChild *>(this)->link()){
            return false;
        }
        break;
    case HKB_BLENDER_GENERATOR:
        if (!reinterpret_cast<hkbBlenderGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_I_STATE_TAGGING_GENERATOR:
        if (!reinterpret_cast<BSiStateTaggingGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_BONE_SWITCH_GENERATOR:
        if (!reinterpret_cast<BSBoneSwitchGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_BONE_SWITCH_GENERATOR_BONE_DATA:
        if (!reinterpret_cast<BSBoneSwitchGeneratorBoneData *>(this)->link()){
            return false;
        }
        break;
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        if (!reinterpret_cast<BSCyclicBlendTransitionGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
        if (!reinterpret_cast<BSSynchronizedClipGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_MODIFIER_GENERATOR:
        if (!reinterpret_cast<hkbModifierGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_CLIP_GENERATOR:
        if (!reinterpret_cast<hkbClipGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_BEHAVIOR_GRAPH:
        if (!reinterpret_cast<hkbBehaviorGraph *>(this)->link()){
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

/**
 * hkRootLevelContainer
 */

uint hkRootLevelContainer::refCount = 0;

bool hkRootLevelContainer::readData(const HkxXmlReader &reader, int index){
    bool ok;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        //need to use strcmp for dealing with dynamically sized qbytearrays
        if (reader.getNthAttributeValueAt(index, 0) == "namedVariants"){
            int numVariants = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numVariants; j++){
                namedVariants.append(hkRootLevelContainerNamedVariant());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "name"){
                        namedVariants.last().name = reader.getElementValueAt(index);
                    }else if (reader.getNthAttributeValueAt(index, 0) == "className"){
                        namedVariants.last().className = reader.getElementValueAt(index);
                    }else if (reader.getNthAttributeValueAt(index, 0) == "variant"){
                        if (!namedVariants.last().variant.readReference(index, reader)){
                            return false;
                        }
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool hkRootLevelContainer::link(){
    if (!getParentFile()){
        return false;
    }
    for (int i = 0; i < namedVariants.size(); i++){
        if (namedVariants.at(i).variant.getReference() == -1){
            return false;
        }
        HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(namedVariants.at(i).variant.getReference());
        if (!ptr){
            return false;
        }
        if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH){
            return false;
        }
        namedVariants[i].variant = *ptr;
        /*if (!static_cast<hkbBehaviorGraph *>(namedVariants[i].variant.data())->link()){
            return false;
        }*/
    }
    return true;
}

/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

hkbStateMachineStateInfo::hkbStateMachineStateInfo(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_STATE_MACHINE_STATE_INFO, TYPE_GENERATOR);
    refCount++;
    stateId = 0;
    probability = 1;
    enable = true;
}

bool hkbStateMachineStateInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "enterNotifyEvents"){
            if (!enterNotifyEvents.readReference(index, reader)){
                return false;
            }
        }else if (text == "exitNotifyEvents"){
            if (!exitNotifyEvents.readReference(index, reader)){
                return false;
            }
        }else if (text == "transitions"){
            if (!transitions.readReference(index, reader)){
                return false;
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "stateId"){
            stateId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "probability"){
            probability = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbStateMachineStateInfo::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //enterNotifyEvents
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(enterNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            return false;
        }
        enterNotifyEvents = *ptr;
    }
    //exitNotifyEvents
    ptr = getParentFile()->findHkObject(exitNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            return false;
        }
        exitNotifyEvents = *ptr;
    }
    //transitions
    ptr = getParentFile()->findHkObject(transitions.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            return false;
        }
        transitions = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    generator = *ptr;
    /*if (!static_cast<hkbGenerator *>(generator.data())->link()){
        return false;
    }*/
    return true;
}

/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_STATE_MACHINE, TYPE_GENERATOR);
    refCount++;
    userData = 0;
    id = -1;
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
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "id"){
            id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "payload"){
            if (!payload.readReference(index, reader)){
                return false;
            }
        /*}*else if (text == "startStateChooser"){
            if (!generator.readReference(index, reader)){
                return false;
            }*/
        }else if (text == "startStateId"){
            startStateId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "returnToPreviousStateEventId"){
            returnToPreviousStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "transitionToNextHigherStateEventId"){
            transitionToNextHigherStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "transitionToNextLowerStateEventId"){
            transitionToNextLowerStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "syncVariableIndex"){
            syncVariableIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "wrapAroundStateId"){
            wrapAroundStateId = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "maxSimultaneousTransitions"){
            maxSimultaneousTransitions = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "startStateMode"){
            startStateMode = reader.getElementValueAt(index);
            ok = false;
            for (int i = 0; i < StartStateMode.size(); i++){
                if (StartStateMode.at(i) == startStateMode){
                    ok = true;
                    i = StartStateMode.size();
                }
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            ok = false;
            for (int i = 0; i < SelfTransitionMode.size(); i++){
                if (SelfTransitionMode.at(i) == startStateMode){
                    ok = true;
                    i = SelfTransitionMode.size();
                }
            }
        }else if (text == "states"){
            if (!readReferences(reader.getElementValueAt(index), states)){
                return false;
            }
        }else if (text == "wildcardTransitions"){
            if (!wildcardTransitions.readReference(index, reader)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbStateMachine::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //payload
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            return false;
        }
        payload = *ptr;
    }
    for (int i = 0; i < states.size(); i++){
        //states
        ptr = getParentFile()->findGenerator(states.at(i).getReference());
        if (!ptr){
            return false;
        }
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
            return false;
        }
        states[i] = *ptr;
        /*if (!static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->link()){
            return false;
        }*/
    }
    return true;
}

/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

hkbModifierGenerator::hkbModifierGenerator(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_MODIFIER_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbModifierGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "modifier"){
            if (!modifier.readReference(index, reader)){
                return false;
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbModifierGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //enterNotifyEvents
    HkObjectExpSharedPtr *ptr;
    //modifier
    ptr = getParentFile()->findModifier(modifier.getReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            return false;
        }
        modifier = *ptr;
        if (!static_cast<hkbModifier *>(modifier.data())->link()){
            return false;
        }
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    generator = *ptr;
    /*if (!static_cast<hkbGenerator *>(generator.data())->link()){
        return false;
    }*/
    return true;
}

/*
 * CLASS: hkbManualSelectorGenerator
*/

uint hkbManualSelectorGenerator::refCount = 0;

hkbManualSelectorGenerator::hkbManualSelectorGenerator(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_MANUAL_SELECTOR_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
    selectedGeneratorIndex = 0;
    currentGeneratorIndex = 0;
}

bool hkbManualSelectorGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "generators"){
            if (!readReferences(reader.getElementValueAt(index), generators)){
                return false;
            }
        }else if (text == "selectedGeneratorIndex"){
            selectedGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "currentGeneratorIndex"){
            currentGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbManualSelectorGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //generators
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < generators.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(generators.at(i).getReference());
        if (!ptr){
            return false;
        }
        if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            return false;
        }
        generators[i] = *ptr;
        /*if (!static_cast<hkbGenerator *>(generators.at(i).data())->link()){
            return false;
        }*/
    }
    return true;
}

/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BLENDER_GENERATOR_CHILD, TYPE_GENERATOR);
    refCount++;
    weight = 0;
    worldFromModelWeight = 0;
}

bool hkbBlenderGeneratorChild::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                return false;
            }
        }else if (text == "boneWeights"){
            if (!boneWeights.readReference(index, reader)){
                return false;
            }
        }else if (text == "weight"){
            weight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "worldFromModelWeight"){
            worldFromModelWeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbBlenderGeneratorChild::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //boneWeights
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(boneWeights.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            return false;
        }
        boneWeights = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    generator = *ptr;
    /*if (!static_cast<hkbGenerator *>(generator.data())->link()){
        return false;
    }*/
    return true;
}

/*
 * CLASS: hkbBlenderGenerator
*/

uint hkbBlenderGenerator::refCount = 0;

QStringList hkbBlenderGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};

hkbBlenderGenerator::hkbBlenderGenerator(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
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
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                return false;
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbBlenderGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //children
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(children.at(i).getReference());
        if (!ptr){
            return false;
        }
        if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            return false;
        }
        children[i] = *ptr;
        /*if (!static_cast<hkbBlenderGeneratorChild *>(children.at(i).data())->link()){
            return false;
        }*/
    }
    return true;
}

/*
 * CLASS: BSBoneSwitchGeneratorBoneData
*/

uint BSBoneSwitchGeneratorBoneData::refCount = 0;

BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData(BehaviorFile *parent/*, qint16 ref*/)
    : HkDynamicObject(parent/*, ref*/)
{
    refCount++;
    setType(BS_BONE_SWITCH_GENERATOR_BONE_DATA, TYPE_GENERATOR);
}

bool BSBoneSwitchGeneratorBoneData::readData(const HkxXmlReader &reader, long index){
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "pGenerator"){
            if (!pGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "spBoneWeight"){
            if (!spBoneWeight.readReference(index, reader)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //pGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pGenerator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    pGenerator = *ptr;
    /*if (!static_cast<hkbGenerator *>(pGenerator.data())->link()){
        return false;
    }*/
    //spBoneWeight
    ptr = getParentFile()->findHkObject(spBoneWeight.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            return false;
        }
        spBoneWeight = *ptr;
    }
    return true;
}

/*
 * CLASS: BSBoneSwitchGenerator
*/

uint BSBoneSwitchGenerator::refCount = 0;

BSBoneSwitchGenerator::BSBoneSwitchGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : HkDynamicObject(parent/*, ref*/),\
    userData(0)
{
    refCount++;
    setType(BS_BONE_SWITCH_GENERATOR, TYPE_GENERATOR);
}

bool BSBoneSwitchGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "ChildrenA"){
            if (!readReferences(reader.getElementValueAt(index), ChildrenA)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //pDefaultGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    pDefaultGenerator = *ptr;
    /*if (!static_cast<hkbGenerator *>(pDefaultGenerator.data())->link()){
        return false;
    }*/
    for (int i = 0; i < ChildrenA.size(); i++){
        //ChildrenA
        ptr = getParentFile()->findGenerator(ChildrenA.at(i).getReference());
        if (!ptr){
            return false;
        }
        if ((*ptr)->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            return false;
        }
        ChildrenA[i] = *ptr;
        /*if (!static_cast<BSBoneSwitchGeneratorBoneData *>(ChildrenA.at(i).data())->link()){
            return false;
        }*/
    }
    return true;
}

/*
 * CLASS: BSCyclicBlendTransitionGenerator
*/

uint BSCyclicBlendTransitionGenerator::refCount = 0;

QStringList BSCyclicBlendTransitionGenerator::BlendCurve = {"BLEND_CURVE_SMOOTH"};

BSCyclicBlendTransitionGenerator::BSCyclicBlendTransitionGenerator(BehaviorFile *parent/*, qint16 ref*/)
    :HkDynamicObject(parent/*, ref*/),\
    userData(0),\
    fBlendParameter(1),\
    fTransitionDuration(0),\
    eBlendCurve(BlendCurve.first())
{
    refCount++;
    setType(BS_CYCLIC_BLEND_TRANSITION_GENERATOR, TYPE_GENERATOR);
}

bool BSCyclicBlendTransitionGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "pBlenderGenerator"){
            if (!pBlenderGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "EventToFreezeBlendValue"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToFreezeBlendValueId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        return false;
                    }
                }else if (text == "payload"){
                    if (!eventToFreezeBlendValuePayload.readReference(index, reader)){
                        return false;
                    }
                    break;
                }
                index++;
            }
        }else if (text == "EventToCrossBlend"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToCrossBlendId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        return false;
                    }
                }else if (text == "payload"){
                    if (!eventToCrossBlendPayload.readReference(index, reader)){
                        return false;
                    }
                    break;
                }
                index++;
            }
        }else if (text == "fBlendParameter"){
            fBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "fTransitionDuration"){
            fTransitionDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "eBlendCurve"){
            eBlendCurve = reader.getElementValueAt(index);
            if (eBlendCurve == ""){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool BSCyclicBlendTransitionGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //pBlenderGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pBlenderGenerator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR){
        return false;
    }
    pBlenderGenerator = *ptr;
    /*if (!static_cast<hkbBlenderGenerator *>(pBlenderGenerator.data())->link()){
        return false;
    }*/
    return true;
}

/*
 * CLASS: BSiStateTaggingGenerator
*/

uint BSiStateTaggingGenerator::refCount = 0;

BSiStateTaggingGenerator::BSiStateTaggingGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : HkDynamicObject(parent/*, ref*/),\
      userData(1),\
      iStateToSetAs(-1),\
      iPriority(0)
{
    setType(BS_I_STATE_TAGGING_GENERATOR, TYPE_GENERATOR);
    refCount++;
}

bool BSiStateTaggingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "iStateToSetAs"){
            iStateToSetAs = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "iPriority"){
            iPriority = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool BSiStateTaggingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //pDefaultGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        return false;
    }
    if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        return false;
    }
    pDefaultGenerator = *ptr;
    /*if (!static_cast<hkbGenerator *>(pDefaultGenerator.data())->link()){
        return false;
    }*/
    return true;
}

/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BEHAVIOR_REFERENCE_GENERATOR, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbBehaviorReferenceGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "behaviorName"){
            behaviorName = reader.getElementValueAt(index);
            if (behaviorName == ""){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorReferenceGenerator::link(){
    return true;
}

/*
 * CLASS: hkbClipGenerator
*/

uint hkbClipGenerator::refCount = 0;

QStringList hkbClipGenerator::PlaybackMode = {"MODE_SINGLE_PLAY", "MODE_LOOPING", "MODE_USER_CONTROLLED", "MODE_PING_PONG", "MODE_COUNT"};
QStringList hkbClipGenerator::ClipFlags = {"0", "FLAG_CONTINUE_MOTION_AT_END", "FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE", "FLAG_MIRROR", "FLAG_FORCE_DENSE_POSE", "FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS", "FLAG_IGNORE_MOTION"};

hkbClipGenerator::hkbClipGenerator(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
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
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "animationName"){
            animationName = reader.getElementValueAt(index);
            if (animationName == ""){
                return false;
            }
        }else if (text == "triggers"){
            if (!triggers.readReference(index, reader)){
                return false;
            }
        }else if (text == "cropStartAmountLocalTime"){
            cropStartAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "cropEndAmountLocalTime"){
            cropEndAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "startTime"){
            startTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "playbackSpeed"){
            playbackSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "enforcedDuration"){
            enforcedDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "userControlledTimeFraction"){
            userControlledTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "animationBindingIndex"){
            animationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                return false;
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbClipGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //triggers
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(triggers.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
            return false;
        }
        triggers = *ptr;
    }
    return true;
}

/*
 * CLASS: BSSynchronizedClipGenerator
*/

uint BSSynchronizedClipGenerator::refCount = 0;

BSSynchronizedClipGenerator::BSSynchronizedClipGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : HkDynamicObject(parent/*, ref*/),\
    userData(0),\
    bSyncClipIgnoreMarkPlacement(false),\
    fGetToMarkTime(0),\
    fMarkErrorThreshold(0),\
    bLeadCharacter(false),\
    bReorientSupportChar(false),\
    bApplyMotionFromRoot(false),\
    sAnimationBindingIndex(-1)
{
    refCount++;
    setType(BS_SYNCHRONIZED_CLIP_GENERATOR, TYPE_GENERATOR);
}

bool BSSynchronizedClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "pClipGenerator"){
            if (!pClipGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "SyncAnimPrefix"){
            SyncAnimPrefix = reader.getElementValueAt(index);
            if (SyncAnimPrefix == ""){
                return false;
            }
        }else if (text == "bSyncClipIgnoreMarkPlacement"){
            bSyncClipIgnoreMarkPlacement = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "fGetToMarkTime"){
            fGetToMarkTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "fMarkErrorThreshold"){
            fMarkErrorThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "bLeadCharacter"){
            bLeadCharacter = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "bReorientSupportChar"){
            bReorientSupportChar = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "bApplyMotionFromRoot"){
            bApplyMotionFromRoot = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                return false;
            }
        }else if (text == "sAnimationBindingIndex"){
            sAnimationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool BSSynchronizedClipGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    //pClipGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pClipGenerator.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
            return false;
        }
        pClipGenerator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(BehaviorFile *parent/*, qint16 ref*/): HkDynamicObject(parent/*, ref*/){
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    refCount++;
    userData = 0;
}

bool hkbBehaviorGraph::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                return false;
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                return false;
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                return false;
            }
        }else if (text == "variableMode"){
            variableMode = reader.getElementValueAt(index);
            if (variableMode == ""){
                return false;
            }
        }else if (text == "rootGenerator"){
            if (!rootGenerator.readReference(index, reader)){
                return false;
            }
        }else if (text == "data"){
            if (!data.readReference(index, reader)){
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraph::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        return false;
    }
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(rootGenerator.getReference());
    if (!ptr){
        return false;
    }
    rootGenerator = *ptr;
    if (rootGenerator->getSignature() != HKB_STATE_MACHINE){
        return false;
    }
    /*if (!static_cast<hkbStateMachine *>(rootGenerator.data())->link()){
        return false;
    }*/
    /*ptr = getParentFile()->findGenerator(data.getReference());
    if (!ptr){
        return false;
    }
    data = *ptr;
    if (data->getSignature() != HKB_BEHAVIOR_GRAPH_DATA){
        return false;
    }
    if (!static_cast<hkbBehaviorGraph *>(data.data())->link()){
        return false;
    }*/
    return true;
}





















