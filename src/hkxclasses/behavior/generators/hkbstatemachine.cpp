#include "hkbstatemachine.h"
#include "hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbStateMachine
*/

uint hkbStateMachine::refCount = 0;

QString hkbStateMachine::classname = "hkbStateMachine";

QStringList hkbStateMachine::StartStateMode = {"START_STATE_MODE_DEFAULT", "START_STATE_MODE_SYNC", "START_STATE_MODE_RANDOM", "START_STATE_MODE_CHOOSER"};
QStringList hkbStateMachine::SelfTransitionMode = {"SELF_TRANSITION_MODE_NO_TRANSITION", "SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", "SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE"};

hkbStateMachine::hkbStateMachine(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
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

int hkbStateMachine::generateValidStateId(){
    int stateId = 0;
    int prev = 0;
    hkbStateMachineStateInfo *ptr = NULL;
    for (int i = 0; i < states.size(); i++){
        ptr = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        stateId = ptr->stateId;
        if (stateId <= prev){
            prev = prev + 1;
        }else if (stateId > prev){
            prev = stateId + 1;
        }
    }
    return prev;
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

int hkbStateMachine::getStateId(const QString &statename) const{
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state->name == statename){
            return i;
        }
    }
    return -1;
}

int hkbStateMachine::getNestedStateId(const QString &statename, int stateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getStateId(statename);
    }
    return -1;
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

bool hkbStateMachine::insertObjectAt(int index, DataIconManager *obj){
    hkbStateMachineStateInfo *objChild;
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        if (index >= states.size() || index == -1){
            objChild = new hkbStateMachineStateInfo(getParentFile(), this, -1);
            states.append(HkxSharedPtr(objChild));
            objChild->generator = HkxSharedPtr((HkxObject *)obj);
        }else if (index == 0){
            objChild = static_cast<hkbStateMachineStateInfo *>(states.at(index).data());
            objChild->generator = HkxSharedPtr((HkxObject *)obj);
        }else if (index > -1){
            objChild = static_cast<hkbStateMachineStateInfo *>(states.at(index).data());
            objChild->generator = HkxSharedPtr((HkxObject *)obj);
        }else{
            return false;
        }
        return true;
    }else{
        return false;
    }
}

bool hkbStateMachine::removeObjectAt(int index){
    if (index < states.size()){
        hkbStateMachineStateInfo *state = NULL;
        hkbStateMachineTransitionInfoArray *trans = NULL;
        int stateId = 0;
        if (index > -1 && index < states.size()){
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(wildcardTransitions.data());
            state = static_cast<hkbStateMachineStateInfo *>(states.at(index).data());
            stateId = state->stateId;
            if (stateId == startStateId){
                startStateId = -1;
            }
            if (trans){
                trans->removeTransitionToState(stateId);
            }
            state->unlink();
            for (int i = 0; i < states.size(); i++){
                state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
                trans = static_cast<hkbStateMachineTransitionInfoArray *>(state->transitions.data());
                if (trans){
                    trans->removeTransitionToState(stateId);
                }
            }
            states.removeAt(index);
        }else if (index == -1){
            for (int i = 0; i < states.size(); i++){
                state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
                trans = static_cast<hkbStateMachineTransitionInfoArray *>(state->transitions.data());
                stateId = state->stateId;
                if (stateId == startStateId){
                    startStateId = -1;
                }
                if (trans){
                    trans->removeTransitionToState(stateId);
                }
                trans = static_cast<hkbStateMachineTransitionInfoArray *>(wildcardTransitions.data());
                if (trans){
                    trans->removeTransitionToState(stateId);
                }
                state->unlink();
            }
            states.clear();
        }else{
            return false;
        }
        return true;
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

int hkbStateMachine::getIndexOfObj(DataIconManager *obj) const{
    hkbStateMachineStateInfo *child;
    for (int i = 0; i < states.size(); i++){
        child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (child->generator.data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

QList<DataIconManager *> hkbStateMachine::getChildren() const{
    QList<DataIconManager *> list;
    hkbStateMachineStateInfo *child;
    for (int i = 0; i < states.size(); i++){
        child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (child->generator.data()){
            list.append(static_cast<DataIconManager*>(child->generator.data()));
        }
    }
    return list;
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
            eventToSendWhenStateOrTransitionChanges.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!eventToSendWhenStateOrTransitionChanges.payload.readReference(index, reader)){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToSendWhenStateOrTransitionChanges.id));
        if (eventToSendWhenStateOrTransitionChanges.payload.data()){
            refString = eventToSendWhenStateOrTransitionChanges.payload.data()->getReferenceString();
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
            refString = refString+states.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }else{
                refString = refString+" ";
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
        if (eventToSendWhenStateOrTransitionChanges.payload.data() && !eventToSendWhenStateOrTransitionChanges.payload.data()->write(writer)){
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
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSendWhenStateOrTransitionChanges.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
        }
        eventToSendWhenStateOrTransitionChanges.payload = *ptr;
    }
    for (int i = 0; i < states.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGeneratorChild(states.at(i).getReference());
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
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(wildcardTransitions.getReference());
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
    eventToSendWhenStateOrTransitionChanges.payload = HkxSharedPtr();
    for (int i = 0; i < states.size(); i++){
        if (states.at(i).data()){
            states[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        states[i] = HkxSharedPtr();
    }
    wildcardTransitions = HkxSharedPtr();
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
    }else if (eventToSendWhenStateOrTransitionChanges.payload.data() && eventToSendWhenStateOrTransitionChanges.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
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
