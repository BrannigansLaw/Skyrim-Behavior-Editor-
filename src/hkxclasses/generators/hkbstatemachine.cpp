#include "hkbstatemachine.h"
#include "hkbstatemachinestateinfo.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QString hkbStateMachine::classname = "hkbStateMachine";

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
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
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "State Machine "+QString::number(refCount);
}

QString hkbStateMachine::getClassname(){
    return classname;
}

QString hkbStateMachine::getName() const{
    return name;
}

int hkbStateMachine::getNumberOfStates() const{
    return states.size();
}

int hkbStateMachine::getIndexToInsertIcon() const{
    for (int i = 0; i < states.size(); i++){
        if (!static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator.data()){
            return i;
        }
    }
    return -1;
}

QString hkbStateMachine::getStateName(int stateId) const{
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state->stateId == stateId){
            return state->name;
        }
    }
    return "";
}

QStringList hkbStateMachine::getStateNames() const{
    QStringList list;
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        list.append(state->name);
    }
    return list;
}

QString hkbStateMachine::getNestedStateName(int stateId, int nestedStateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getStateName(nestedStateId);
    }
    return "";
}

QStringList hkbStateMachine::getNestedStateNames(int stateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getStateNames();
    }
    return QStringList();
}

int hkbStateMachine::getNumberOfNestedStates(int stateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getNumberOfStates();
    }
    return -1;
}

hkbStateMachine * hkbStateMachine::getNestedStateMachine(int stateId) const{
    hkbGenerator *gen = NULL;
    hkbStateMachineStateInfo *state = NULL;
    qlonglong sig = 0;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state && state->stateId == stateId){
            gen = static_cast<hkbGenerator *>(static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator.data());
            while (gen){
                sig = gen->getSignature();
                switch (sig){
                case HKB_STATE_MACHINE:
                    return static_cast<hkbStateMachine *>(gen);
                case HKB_MODIFIER_GENERATOR:
                    gen = static_cast<hkbGenerator *>(static_cast<hkbModifierGenerator *>(gen)->generator.data());
                    break;
                case BS_I_STATE_TAGGING_GENERATOR:
                    gen = static_cast<hkbGenerator *>(static_cast<BSiStateTaggingGenerator *>(gen)->pDefaultGenerator.data());
                    break;
                default:
                    return NULL;
                }
            }
        }
    }
    return NULL;
}

void hkbStateMachine::removeState(int index){
    if (index < states.size()){
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(wildcardTransitions.data());
        hkbStateMachineStateInfo *state = static_cast<hkbStateMachineStateInfo *>(states.at(index).data());
        int stateId = state->stateId;
        trans->removeTransitionToState(stateId);
        for (int i = 0; i < states.size(); i++){
            state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(state->transitions.data());
            if (trans){
                trans->removeTransitionToState(stateId);
            }
        }
    }
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
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(payload.getReference());
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
            static_cast<hkbStateMachineStateInfo *>(states[i].data())->parentSM = this;
        }
    }
    ptr = getParentFile()->findHkxObject(wildcardTransitions.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
                writeToLog("hkbStateMachine: linkVar()!\nThe linked object 'wildcardTransitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
            }
            wildcardTransitions = *ptr;
            static_cast<hkbStateMachineTransitionInfoArray *>(wildcardTransitions.data())->parent = this;
        }
    return true;
}

void hkbStateMachine::unlink(){
    HkDynamicObject::unlink();
    payload = HkxObjectExpSharedPtr();
    for (int i = 0; i < states.size(); i++){
        if (states.at(i).data()){
            states[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        states[i] = HkxObjectExpSharedPtr();
    }
    wildcardTransitions = HkxObjectExpSharedPtr();
}

bool hkbStateMachine::evaulateDataValidity(){
    bool valid = true;
    for (int i = 0; i < states.size(); i++){
        if (!states.at(i).data() || states.at(i).data()->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (payload.data() && payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (maxSimultaneousTransitions > 32){
    }else if (!StartStateMode.contains(startStateMode)){
    }else if (!SelfTransitionMode.contains(selfTransitionMode)){
    }else if (name == ""){
    }else if (wildcardTransitions.data() && wildcardTransitions.data()->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
    }else if (states.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbStateMachine::~hkbStateMachine(){
    refCount--;
}
