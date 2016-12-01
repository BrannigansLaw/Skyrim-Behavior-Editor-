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
      startStateMode(StartStateMode.first()),
      selfTransitionMode(SelfTransitionMode.first())
{
    setType(HKB_STATE_MACHINE, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbStateMachine"+QString::number(refCount);
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

bool hkbStateMachine::setChildAt(HkxObject *newChild, ushort index){
    if (newChild && newChild->getType() != TYPE_GENERATOR){
        return false;
    }
    hkbStateMachineStateInfo *state = NULL;
    if (!states.isEmpty()){
        if (index < states.size()){
            state = static_cast<hkbStateMachineStateInfo *>(states.at(index).data());
            if (state){
                state->generator = HkxObjectExpSharedPtr(newChild);
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        state = new hkbStateMachineStateInfo(getParentFile(), this, 0);
        state->generator = HkxObjectExpSharedPtr(newChild);
        states.append(HkxObjectExpSharedPtr(state));
        return true;
    }
}

bool hkbStateMachine::wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace){
    bool wasReplaced = false;
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state->generator.data() == childToReplace){
            if (!objToInject->setChildAt(state->generator.data())){
                return false;
            }
            state->generator = HkxObjectExpSharedPtr(objToInject);
            wasReplaced = true;
        }
    }
    return wasReplaced;
}

bool hkbStateMachine::appendObject(DataIconManager *objToAppend){
    hkbStateMachineStateInfo *objChild = new hkbStateMachineStateInfo(getParentFile(), this, -1);
    states.append(HkxObjectExpSharedPtr(objChild));
    objChild->generator = HkxObjectExpSharedPtr(objToAppend);
    return true;
}

bool hkbStateMachine::removeObject(DataIconManager *objToRemove, bool removeAll){
    bool b = false;
    if (removeAll){
        hkbStateMachineStateInfo *child;
        for (int i = 0; i < states.size(); i++){
            child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
            if (child->generator.data() == objToRemove){
                child->generator = HkxObjectExpSharedPtr();
                b = true;
            }
        }
        return b;
    }else{
        hkbStateMachineStateInfo *child;
        for (int i = 0; i < states.size(); i++){
            child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
            if (child->generator.data() == objToRemove){
                child->generator = HkxObjectExpSharedPtr();
                return true;
            }
        }
    }
    return false;
}

bool hkbStateMachine::hasChildren() const{
    hkbStateMachineStateInfo *child;
    for (int i = 0; i < states.size(); i++){
        child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (child->generator.data()){
            return true;
        }
    }
    return false;
}

int hkbStateMachine::addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder){
    int objectChildCount = 0;
    if (reverseOrder){
        for (int i = states.size() - 1; i >= 0; i--){
            if (states.at(i).data() && static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator.data()){
                list.append(static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator);
                objectChildCount++;
            }
        }
    }else{
        for (int i = 0; i < states.size(); i++){
            if (states.at(i).data() && static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator.data()){
                list.append(static_cast<hkbStateMachineStateInfo *>(states.at(i).data())->generator);
                objectChildCount++;
            }
        }
    }
    return objectChildCount;
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
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!payload.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        /*}*else if (text == "startStateChooser"){
            if (!generator.readReference(index, reader)){
                return false;
            }*/
        }else if (text == "startStateId"){
            startStateId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'startStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "returnToPreviousStateEventId"){
            returnToPreviousStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'returnToPreviousStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextHigherStateEventId"){
            transitionToNextHigherStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'transitionToNextHigherStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextLowerStateEventId"){
            transitionToNextLowerStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'transitionToNextLowerStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "syncVariableIndex"){
            syncVariableIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'syncVariableIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "wrapAroundStateId"){
            wrapAroundStateId = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'wrapAroundStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxSimultaneousTransitions"){
            maxSimultaneousTransitions = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxSimultaneousTransitions' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startStateMode"){
            startStateMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                writeToLog(getClassname()+": readData()!\nInvalid 'startStateMode' data!\nObject Reference: "+ref);
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                writeToLog(getClassname()+": readData()!\nInvalid 'selfTransitionMode' data!\nObject Reference: "+ref);
            }
        }else if (text == "states"){
            if (!readReferences(reader.getElementValueAt(index), states)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'states' references!\nObject Reference: "+ref);
            }
        }else if (text == "wildcardTransitions"){
            if (!wildcardTransitions.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'wildcardTransitions' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbStateMachine::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userData"), QString::number(userData));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventToSendWhenStateOrTransitionChanges"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(id));
        if (payload.data()){
            refString = payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startStateChooser"), "null");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startStateId"), QString::number(startStateId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("returnToPreviousStateEventId"), QString::number(returnToPreviousStateEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("randomTransitionEventId"), QString::number(randomTransitionEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transitionToNextHigherStateEventId"), QString::number(transitionToNextHigherStateEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transitionToNextLowerStateEventId"), QString::number(transitionToNextLowerStateEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("syncVariableIndex"), QString::number(syncVariableIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("wrapAroundStateId"), getBoolAsString(wrapAroundStateId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxSimultaneousTransitions"), QString::number(maxSimultaneousTransitions));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startStateMode"), startStateMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("selfTransitionMode"), selfTransitionMode);
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"states", QString::number(states.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < states.size(); i++){
            refString = refString+" "+states.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }
        }
        if (states.size() > 0){
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        if (wildcardTransitions.data()){
            refString = wildcardTransitions.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("wildcardTransitions"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (payload.data() && !payload.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload'!!!", true);
        }
        if (wildcardTransitions.data() && !wildcardTransitions.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'wildcardTransitions'!!!", true);
        }
        for (int i = 0; i < states.size(); i++){
            if (states.at(i).data() && !states.at(i).data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'states' at: "+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbStateMachine::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
        }
        payload = *ptr;
    }
    for (int i = 0; i < states.size(); i++){
        ptr = getParentFile()->findGeneratorChild(states.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'states' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
            writeToLog(getClassname()+": link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
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
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'wildcardTransitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
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
