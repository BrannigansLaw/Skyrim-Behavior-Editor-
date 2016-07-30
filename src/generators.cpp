#include "generators.h"
#include "hkxxmlreader.h"
#include "hkxfile.h"
#include "modifiers.h"

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
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
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
    case BS_OFFSET_ANIMATION_GENERATOR:
        if (!reinterpret_cast<BSOffsetAnimationGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_POSE_MATCHING_GENERATOR:
        if (!reinterpret_cast<hkbPoseMatchingGenerator *>(this)->link()){
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

QString hkbGenerator::getName() const{
    qulonglong sig = getSignature();
    switch (sig) {
    case HKB_STATE_MACHINE_STATE_INFO:
        return static_cast<hkbStateMachineStateInfo const *>(this)->getName();
    case HKB_STATE_MACHINE:
        return static_cast<hkbStateMachine const *>(this)->getName();
    case HKB_MANUAL_SELECTOR_GENERATOR:
        return static_cast<hkbManualSelectorGenerator const *>(this)->getName();
    case HKB_BLENDER_GENERATOR:
        return static_cast<hkbBlenderGenerator const *>(this)->getName();
    case BS_I_STATE_TAGGING_GENERATOR:
        return static_cast<BSiStateTaggingGenerator const *>(this)->getName();
    case BS_BONE_SWITCH_GENERATOR:
        return static_cast<BSBoneSwitchGenerator const *>(this)->getName();
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        return static_cast<BSCyclicBlendTransitionGenerator const *>(this)->getName();
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
        return static_cast<BSSynchronizedClipGenerator const *>(this)->getName();
    case HKB_MODIFIER_GENERATOR:
        return static_cast<hkbModifierGenerator const *>(this)->getName();
    case BS_OFFSET_ANIMATION_GENERATOR:
        return static_cast<BSOffsetAnimationGenerator const *>(this)->getName();
    case HKB_POSE_MATCHING_GENERATOR:
        return static_cast<hkbPoseMatchingGenerator const *>(this)->getName();
    case HKB_CLIP_GENERATOR:
        return static_cast<hkbClipGenerator const *>(this)->getName();
    case HKB_BEHAVIOR_GRAPH:
        return static_cast<hkbBehaviorGraph const *>(this)->getName();
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
        if (reader.getNthAttributeValueAt(index, 0) == "namedVariants"){
            int numVariants = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkRootLevelContainer: readData()!\nAttempt to read the number of variants failed!");
                //return false;
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
        HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(namedVariants.at(i).variant.getReference());
        if (!ptr){
            writeToLog("hkRootLevelContainer: link()!\nUnable to link variant reference "+QString::number(namedVariants.at(i).variant.getReference())+"!");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH){
            writeToLog("hkRootLevelContainer: link()!\nThe linked object is not a HKB_BEHAVIOR_GRAPH!");
            setDataValidity(false);
            namedVariants[i].variant = *ptr;
        }else{
            namedVariants[i].variant = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

hkbStateMachineStateInfo::hkbStateMachineStateInfo(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      name("State"),
      stateId(0),
      probability(1),
      enable(true)
{
    setType(HKB_STATE_MACHINE_STATE_INFO, TYPE_GENERATOR);
    refCount++;
}

bool hkbStateMachineStateInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "enterNotifyEvents"){
            if (!enterNotifyEvents.readReference(index, reader)){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'enterNotifyEvents' reference!\nObject Reference: "+ref);
            }
        }else if (text == "exitNotifyEvents"){
            if (!exitNotifyEvents.readReference(index, reader)){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'exitNotifyEvents' reference!\nObject Reference: "+ref);
            }
        }else if (text == "transitions"){
            if (!transitions.readReference(index, reader)){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'transitions' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "stateId"){
            stateId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'stateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "probability"){
            probability = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'probability' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbStateMachineStateInfo: readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbStateMachineStateInfo: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //enterNotifyEvents
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(enterNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'enterNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        enterNotifyEvents = *ptr;
    }
    //exitNotifyEvents
    ptr = getParentFile()->findHkObject(exitNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'exitNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        exitNotifyEvents = *ptr;
    }
    //transitions
    ptr = getParentFile()->findHkObject(transitions.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'transitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
            setDataValidity(false);
        }
        transitions = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog("hkbStateMachineStateInfo: link()!\nFailed to properly link 'generator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("hkbStateMachineStateInfo: link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0),
      id(-1),
      startStateId(-1),
      returnToPreviousStateEventId(-1),
      randomTransitionEventId(-1),
      transitionToNextHigherStateEventId(-1),
      transitionToNextLowerStateEventId(-1),
      syncVariableIndex(-1),
      wrapAroundStateId(false),
      maxSimultaneousTransitions(32), //Max 32, min 0.
      startStateMode("START_STATE_MODE_DEFAULT"),
      selfTransitionMode("SELF_TRANSITION_MODE_NO_TRANSITION")
{
    setType(HKB_STATE_MACHINE, TYPE_GENERATOR);
    refCount++;
    name = "State Machine "+QString::number(refCount);
}

bool hkbStateMachine::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!payload.readReference(index, reader)){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        /*}*else if (text == "startStateChooser"){
            if (!generator.readReference(index, reader)){
                return false;
            }*/
        }else if (text == "startStateId"){
            startStateId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'startStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "returnToPreviousStateEventId"){
            returnToPreviousStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'returnToPreviousStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextHigherStateEventId"){
            transitionToNextHigherStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'transitionToNextHigherStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextLowerStateEventId"){
            transitionToNextLowerStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'transitionToNextLowerStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "syncVariableIndex"){
            syncVariableIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'syncVariableIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "wrapAroundStateId"){
            wrapAroundStateId = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'wrapAroundStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxSimultaneousTransitions"){
            maxSimultaneousTransitions = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'maxSimultaneousTransitions' data field!\nObject Reference: "+ref);
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
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nInvalid 'startStateMode' data!\nObject Reference: "+ref);
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
            if (!ok){
                writeToLog("hkbStateMachine: readData()!\nInvalid 'selfTransitionMode' data!\nObject Reference: "+ref);
            }
        }else if (text == "states"){
            if (!readReferences(reader.getElementValueAt(index), states)){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'states' references!\nObject Reference: "+ref);
            }
        }else if (text == "wildcardTransitions"){
            if (!wildcardTransitions.readReference(index, reader)){
                writeToLog("hkbStateMachine: readData()!\nFailed to properly read 'wildcardTransitions' reference!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbStateMachine: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //payload
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog("hkbStateMachine: linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
        }
        payload = *ptr;
    }
    for (int i = 0; i < states.size(); i++){
        //states
        ptr = getParentFile()->findGenerator(states.at(i).getReference());
        if (!ptr){
            writeToLog("hkbStateMachine: link()!\nFailed to properly link 'states' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
            writeToLog("hkbStateMachine: link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            states[i] = *ptr;
        }else{
            states[i] = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

hkbModifierGenerator::hkbModifierGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_MODIFIER_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Modifier Generator "+QString::number(refCount);
}

bool hkbModifierGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "modifier"){
            if (!modifier.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'modifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbModifierGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //enterNotifyEvents
    HkObjectExpSharedPtr *ptr;
    //modifier
    ptr = getParentFile()->findModifier(modifier.getReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            writeToLog("hkbModifierGenerator: link()!\n'modifier' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog("hkbModifierGenerator: link()!\nFailed to properly link 'generator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("hkbModifierGenerator: link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbManualSelectorGenerator
*/

uint hkbManualSelectorGenerator::refCount = 0;

hkbManualSelectorGenerator::hkbManualSelectorGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0),
      selectedGeneratorIndex(0),
      currentGeneratorIndex(0)
{
    setType(HKB_MANUAL_SELECTOR_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Manual Selector Generator "+QString::number(refCount);
}

bool hkbManualSelectorGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "generators"){
            if (!readReferences(reader.getElementValueAt(index), generators)){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'generators' references!\nObject Reference: "+ref);
            }
        }else if (text == "selectedGeneratorIndex"){
            selectedGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'selectedGeneratorIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentGeneratorIndex"){
            currentGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                writeToLog("hkbManualSelectorGenerator: readData()!\nFailed to properly read 'currentGeneratorIndex' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbManualSelectorGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //generators
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < generators.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(generators.at(i).getReference());
        if (!ptr){
            writeToLog("hkbManualSelectorGenerator: link()!\nFailed to properly link 'generators' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            writeToLog("hkbManualSelectorGenerator: link()!\n'generators' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            generators[i] = *ptr;
        }else{
            generators[i] = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      weight(0),
      worldFromModelWeight(0)
{
    setType(HKB_BLENDER_GENERATOR_CHILD, TYPE_GENERATOR);
    refCount++;
}

bool hkbBlenderGeneratorChild::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "boneWeights"){
            if (!boneWeights.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'boneWeights' reference!\nObject Reference: "+ref);
            }
        }else if (text == "weight"){
            weight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'weight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelWeight"){
            worldFromModelWeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'worldFromModelWeight' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbBlenderGeneratorChild: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //boneWeights
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(boneWeights.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog("hkbBlenderGeneratorChild: link()!\n'boneWeights' data field is linked to invalid child!");
            setDataValidity(false);
        }
        boneWeights = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog("hkbBlenderGeneratorChild: link()!\nFailed to properly link 'generator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("hkbBlenderGeneratorChild: link()!\n'generator' data field is linked to invalid child!");
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbBlenderGenerator
*/

uint hkbBlenderGenerator::refCount = 0;

QStringList hkbBlenderGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};

hkbBlenderGenerator::hkbBlenderGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0),
      referencePoseWeightThreshold(0),
      blendParameter(1),
      minCyclicBlendParameter(0),
      maxCyclicBlendParameter(0),
      indexOfSyncMasterChild(-1),
      flags(Flags.first()),
      subtractLastChild(false)
{
    setType(HKB_BLENDER_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Blender Generator "+QString::number(refCount);
}

bool hkbBlenderGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'referencePoseWeightThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'blendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'minCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'maxCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'indexOfSyncMasterChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'subtractLastChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                writeToLog("hkbBlenderGenerator: readData()!\nFailed to properly read 'children' references!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbBlenderGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //children
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(children.at(i).getReference());
        if (!ptr){
            writeToLog("hkbBlenderGenerator: link()!\nFailed to properly link 'children' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            writeToLog("hkbBlenderGenerator: link()!\n'children' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: BSBoneSwitchGeneratorBoneData
*/

uint BSBoneSwitchGeneratorBoneData::refCount = 0;

BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/)
{
    refCount++;
    setType(BS_BONE_SWITCH_GENERATOR_BONE_DATA, TYPE_GENERATOR);
}

bool BSBoneSwitchGeneratorBoneData::readData(const HkxXmlReader &reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pGenerator"){
            if (!pGenerator.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'pGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "spBoneWeight"){
            if (!spBoneWeight.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'spBoneWeight' reference!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //pGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pGenerator.getReference());
    if (!ptr){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\nFailed to properly link 'pGenerator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\n'pGenerator' data field is linked to invalid child!");
        setDataValidity(false);
        pGenerator = *ptr;
    }else{
        pGenerator = *ptr;
    }
    //spBoneWeight
    ptr = getParentFile()->findHkObject(spBoneWeight.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog("BSBoneSwitchGeneratorBoneData: link()!\n'spBoneWeight' data field is linked to invalid child!");
            setDataValidity(false);
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
    : hkbGenerator(parent/*, ref*/),\
    userData(0)
{
    refCount++;
    setType(BS_BONE_SWITCH_GENERATOR, TYPE_GENERATOR);
    name = "BS Bone Switch Generator "+QString::number(refCount);
}

bool BSBoneSwitchGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSBoneSwitchGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSBoneSwitchGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSBoneSwitchGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog("BSBoneSwitchGenerator: readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "ChildrenA"){
            if (!readReferences(reader.getElementValueAt(index), ChildrenA)){
                writeToLog("BSBoneSwitchGenerator: readData()!\nFailed to properly read 'ChildrenA' references!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSBoneSwitchGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //pDefaultGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog("BSBoneSwitchGenerator: link()!\nFailed to properly link 'pDefaultGenerator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSBoneSwitchGenerator: link()!\n'pDefaultGenerator' data field is linked to invalid child!");
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    for (int i = 0; i < ChildrenA.size(); i++){
        //ChildrenA
        ptr = getParentFile()->findGenerator(ChildrenA.at(i).getReference());
        if (!ptr){
            writeToLog("BSBoneSwitchGenerator: link()!\nFailed to properly link 'ChildrenA' data field!");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            writeToLog("BSBoneSwitchGenerator: link()!\n'ChildrenA' data field is linked to invalid child!");
            setDataValidity(false);
            ChildrenA[i] = *ptr;
        }else{
            ChildrenA[i] = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: BSCyclicBlendTransitionGenerator
*/

uint BSCyclicBlendTransitionGenerator::refCount = 0;

QStringList BSCyclicBlendTransitionGenerator::BlendCurve = {"BLEND_CURVE_SMOOTH"};

BSCyclicBlendTransitionGenerator::BSCyclicBlendTransitionGenerator(BehaviorFile *parent/*, qint16 ref*/)
    :hkbGenerator(parent/*, ref*/),
    userData(0),
    fBlendParameter(1),
    fTransitionDuration(0),
    eBlendCurve(BlendCurve.first())
{
    refCount++;
    setType(BS_CYCLIC_BLEND_TRANSITION_GENERATOR, TYPE_GENERATOR);
    name = "BS Cyclic Blend Transition Generator "+QString::number(refCount);
}

bool BSCyclicBlendTransitionGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pBlenderGenerator"){
            if (!pBlenderGenerator.readReference(index, reader)){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'pBlenderGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "EventToFreezeBlendValue"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToFreezeBlendValueId = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToFreezeBlendValueId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToFreezeBlendValuePayload.readReference(index, reader)){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToFreezeBlendValuePayload' reference!\nObject Reference: "+ref);
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
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToCrossBlendId' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToCrossBlendPayload.readReference(index, reader)){
                        writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eventToCrossBlendPayload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "fBlendParameter"){
            fBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'fBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fTransitionDuration"){
            fTransitionDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'fTransitionDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eBlendCurve"){
            eBlendCurve = reader.getElementValueAt(index);
            if (eBlendCurve == ""){
                writeToLog("BSCyclicBlendTransitionGenerator: readData()!\nFailed to properly read 'eBlendCurve' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pBlenderGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pBlenderGenerator.getReference());
    if (!ptr){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\nFailed to properly link 'pBlenderGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR){
        writeToLog("BSCyclicBlendTransitionGenerator: link()!\n'pBlenderGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pBlenderGenerator = *ptr;
    }else{
        pBlenderGenerator = *ptr;
    }
    return true;
}

/*
 * CLASS: BSiStateTaggingGenerator
*/

uint BSiStateTaggingGenerator::refCount = 0;

BSiStateTaggingGenerator::BSiStateTaggingGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(1),
      iStateToSetAs(-1),
      iPriority(0)
{
    setType(BS_I_STATE_TAGGING_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "BS iState Tagging Generator "+QString::number(refCount);
}

bool BSiStateTaggingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "iStateToSetAs"){
            iStateToSetAs = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'iStateToSetAs' data field!\nObject Reference: "+ref);
            }
        }else if (text == "iPriority"){
            iPriority = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'iPriority' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSiStateTaggingGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pDefaultGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog("BSiStateTaggingGenerator: link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSiStateTaggingGenerator: link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorReferenceGenerator
*/

uint hkbBehaviorReferenceGenerator::refCount = 0;

hkbBehaviorReferenceGenerator::hkbBehaviorReferenceGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_BEHAVIOR_REFERENCE_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Behavior Reference Generator "+QString::number(refCount);
}

bool hkbBehaviorReferenceGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "behaviorName"){
            behaviorName = reader.getElementValueAt(index);
            if (behaviorName == ""){
                writeToLog("hkbBehaviorReferenceGenerator: readData()!\nFailed to properly read 'behaviorName' data field!\nObject Reference: "+ref);
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

hkbClipGenerator::hkbClipGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0),
      cropStartAmountLocalTime(0),
      cropEndAmountLocalTime(0),
      startTime(0),
      playbackSpeed(0),
      enforcedDuration(0),
      userControlledTimeFraction(0),
      animationBindingIndex(-1),
      mode(PlaybackMode.first()),
      flags(ClipFlags.first())
{
    setType(HKB_CLIP_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Clip Generator "+QString::number(refCount);
}

bool hkbClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationName"){
            animationName = reader.getElementValueAt(index);
            if (animationName == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'animationName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "triggers"){
            if (!triggers.readReference(index, reader)){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'triggers' reference!\nObject Reference: "+ref);
            }
        }else if (text == "cropStartAmountLocalTime"){
            cropStartAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'cropStartAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "cropEndAmountLocalTime"){
            cropEndAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'cropEndAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startTime"){
            startTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'startTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "playbackSpeed"){
            playbackSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'playbackSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enforcedDuration"){
            enforcedDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'enforcedDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userControlledTimeFraction"){
            userControlledTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'userControlledTimeFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationBindingIndex"){
            animationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'animationBindingIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbClipGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //triggers
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(triggers.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
            writeToLog("hkbClipGenerator: link()!\n'triggers' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
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
    : hkbGenerator(parent/*, ref*/),
    userData(0),
    bSyncClipIgnoreMarkPlacement(false),
    fGetToMarkTime(0),
    fMarkErrorThreshold(0),
    bLeadCharacter(false),
    bReorientSupportChar(false),
    bApplyMotionFromRoot(false),
    sAnimationBindingIndex(-1)
{
    refCount++;
    setType(BS_SYNCHRONIZED_CLIP_GENERATOR, TYPE_GENERATOR);
    name = "BS Synchronized Clip Generator "+QString::number(refCount);
}

bool BSSynchronizedClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pClipGenerator"){
            if (!pClipGenerator.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'pClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "SyncAnimPrefix"){
            SyncAnimPrefix = reader.getElementValueAt(index);
            if (SyncAnimPrefix == ""){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'SyncAnimPrefix' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bSyncClipIgnoreMarkPlacement"){
            bSyncClipIgnoreMarkPlacement = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bSyncClipIgnoreMarkPlacement' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fGetToMarkTime"){
            fGetToMarkTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'fGetToMarkTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fMarkErrorThreshold"){
            fMarkErrorThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'fMarkErrorThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bLeadCharacter"){
            bLeadCharacter = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bLeadCharacter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bReorientSupportChar"){
            bReorientSupportChar = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bReorientSupportChar' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bApplyMotionFromRoot"){
            bApplyMotionFromRoot = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'bApplyMotionFromRoot' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sAnimationBindingIndex"){
            sAnimationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSSynchronizedClipGenerator: readData()!\nFailed to properly read 'sAnimationBindingIndex' data field!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSSynchronizedClipGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pClipGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pClipGenerator.getReference());
    if (!ptr){
        writeToLog("BSSynchronizedClipGenerator: link()!\nFailed to properly link 'pClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        writeToLog("BSSynchronizedClipGenerator: link()!\n'pClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pClipGenerator = *ptr;
    }else{
        pClipGenerator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbPoseMatchingGenerator
*/

uint hkbPoseMatchingGenerator::refCount = 0;

QStringList hkbPoseMatchingGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};

hkbPoseMatchingGenerator::hkbPoseMatchingGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
    userData(0),
    referencePoseWeightThreshold(0),
    blendParameter(0),
    minCyclicBlendParameter(0),
    maxCyclicBlendParameter(0),
    indexOfSyncMasterChild(-1),
    flags(Flags.first()),
    subtractLastChild(false),
    blendSpeed(0),
    minSpeedToSwitch(0),
    minSwitchTimeNoError(0),
    minSwitchTimeFullError(0),
    startPlayingEventId(-1),
    startMatchingEventId(-1),
    rootBoneIndex(-1),
    otherBoneIndex(-1),
    anotherBoneIndex(-1),
    pelvisIndex(-1)
{
    setType(HKB_POSE_MATCHING_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Pose Matching Generator "+QString::number(refCount);
}

bool hkbPoseMatchingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'referencePoseWeightThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'blendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'maxCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'indexOfSyncMasterChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'subtractLastChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'children' references!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelRotation"){
            worldFromModelRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'worldFromModelRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendSpeed"){
            blendSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'blendSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSpeedToSwitch"){
            minSpeedToSwitch = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSpeedToSwitch' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeNoError"){
            minSwitchTimeNoError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSwitchTimeNoError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeFullError"){
            minSwitchTimeFullError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSwitchTimeFullError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startPlayingEventId"){
            startPlayingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'startPlayingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startMatchingEventId"){
            startMatchingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'startMatchingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootBoneIndex"){
            rootBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'rootBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "otherBoneIndex"){
            otherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'otherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "anotherBoneIndex"){
            anotherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'anotherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pelvisIndex"){
            pelvisIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'pelvisIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbPoseMatchingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbPoseMatchingGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //children
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(children.at(i).getReference());
        if (!ptr){
            writeToLog("hkbPoseMatchingGenerator: link()!\nFailed to properly link 'children' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            writeToLog("hkbPoseMatchingGenerator: link()!\n'children' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
        }
    }
    return true;
}

/*
 * CLASS: BSOffsetAnimationGenerator
*/

uint BSOffsetAnimationGenerator::refCount = 0;

BSOffsetAnimationGenerator::BSOffsetAnimationGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0),
      fOffsetVariable(0),
      fOffsetRangeStart(0),
      fOffsetRangeEnd(0)
{
    refCount++;
    setType(BS_OFFSET_ANIMATION_GENERATOR, TYPE_GENERATOR);
    name = "BS Offset Animation Generator "+QString::number(refCount);
}

bool BSOffsetAnimationGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pOffsetClipGenerator"){
            if (!pOffsetClipGenerator.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'pOffsetClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetVariable"){
            fOffsetVariable = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetVariable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeStart"){
            fOffsetRangeStart = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetRangeStart' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeEnd"){
            fOffsetRangeEnd = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetRangeEnd' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSOffsetAnimationGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pDefaultGenerator
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSOffsetAnimationGenerator: link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    //pOffsetClipGenerator
    ptr = getParentFile()->findGenerator(pOffsetClipGenerator.getReference());
    if (!ptr){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'pOffsetClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        writeToLog("BSOffsetAnimationGenerator: link()!\n'pOffsetClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pOffsetClipGenerator = *ptr;
    }else{
        pOffsetClipGenerator = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorGraph
*/

uint hkbBehaviorGraph::refCount = 0;

QStringList hkbBehaviorGraph::VariableMode = {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"};

hkbBehaviorGraph::hkbBehaviorGraph(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_BEHAVIOR_GRAPH, TYPE_GENERATOR);
    refCount++;
    name = "Behavior Graph "+QString::number(refCount);
}

bool hkbBehaviorGraph::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "variableMode"){
            variableMode = reader.getElementValueAt(index);
            if (variableMode == ""){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'variableMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootGenerator"){
            if (!rootGenerator.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'rootGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "data"){
            if (!data.readReference(index, reader)){
                writeToLog("hkbBehaviorGraph: readData()!\nFailed to properly read 'data' reference!\nObject Reference: "+ref);
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
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkObjectExpSharedPtr *ptr = getParentFile()->findGenerator(rootGenerator.getReference());
    if (!ptr){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'rootGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE){
        writeToLog("hkbBehaviorGraph: link()!\n'rootGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        rootGenerator = *ptr;
    }else{
        rootGenerator = *ptr;
    }
    ptr = getParentFile()->findHkObject(data.getReference());
    if (!ptr){
        writeToLog("hkbBehaviorGraph: link()!\nFailed to properly link 'data' data field!\nObject Name: "+name, true);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH_DATA){
        writeToLog("hkbBehaviorGraph: link()!\n'data' data field is linked to invalid child!\nObject Name: "+name, true);
        setDataValidity(false);
        data = *ptr;
    }else{
        data = *ptr;
    }
    return true;
}
