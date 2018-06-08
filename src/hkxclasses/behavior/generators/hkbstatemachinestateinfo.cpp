#include "hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

QString hkbStateMachineStateInfo::classname = "hkbStateMachineStateInfo";

hkbStateMachineStateInfo::hkbStateMachineStateInfo(HkxFile *parent, hkbStateMachine *parentsm, long ref)
    : hkbGenerator(parent, ref),
      probability(1),
      enable(true),
      parentSM(parentsm)
{
    setType(HKB_STATE_MACHINE_STATE_INFO, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    if (parentSM){
        stateId = parentsm->generateValidStateId();
    }else{
        stateId = refCount;
    }
    name = "State"+QString::number(stateId);
    refCount++;
}

QString hkbStateMachineStateInfo::getClassname(){
    return classname;
}

QString hkbStateMachineStateInfo::getStateName(int stateId) const{
    if (parentSM){
        return parentSM->getStateName(stateId);
    }
    return "";
}

QString hkbStateMachineStateInfo::getNestedStateName(int stateId, int nestedStateId) const{
    if (parentSM){
        return parentSM->getNestedStateName(stateId, nestedStateId);
    }
    return "";
}

QString hkbStateMachineStateInfo::getName() const{
    return name;
}

hkbStateMachine * hkbStateMachineStateInfo::getParentStateMachine() const{
    if (parentSM){
        return parentSM;
    }else{
        WARNING_MESSAGE(QString("The state "+getName()+" has no parent!!!"));
    }
    return nullptr;
}

bool hkbStateMachineStateInfo::setStateId(ulong id){
    hkbStateMachineStateInfo *state = nullptr;
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (parentSM){
        for (int i = 0; i < parentSM->states.size(); i++){
            state = static_cast<hkbStateMachineStateInfo *>(parentSM->states.at(i).data());
            if (state && state->stateId == id){
                return false;
            }
        }
        for (int i = 0; i < parentSM->states.size(); i++){
            state = static_cast<hkbStateMachineStateInfo *>(parentSM->states.at(i).data());
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(state->transitions.data());
            if (trans){
                for (int j = 0; j < trans->transitions.size(); j++){
                    if (trans->transitions.at(j).toStateId == stateId){
                        trans->transitions[j].toStateId = id;
                    }
                }
            }
        }
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(parentSM->wildcardTransitions.data());
        if (trans){
            for (int i = 0; i < trans->transitions.size(); i++){
                if (trans->transitions.at(i).toStateId == stateId){
                    trans->transitions[i].toStateId = id;
                }
            }
        }
    }
    stateId = id;
    return true;
}

bool hkbStateMachineStateInfo::hasChildren() const{
    if (generator.data()){
        return true;
    }
    return false;
}

bool hkbStateMachineStateInfo::isEventReferenced(int eventindex) const{
    if (enterNotifyEvents.constData() && enterNotifyEvents.constData()->isEventReferenced(eventindex)){
        return true;
    }
    if (exitNotifyEvents.constData() && exitNotifyEvents.constData()->isEventReferenced(eventindex)){
        return true;
    }
    if (transitions.constData() && transitions.constData()->isEventReferenced(eventindex)){
        return true;
    }
    return false;
}

void hkbStateMachineStateInfo::updateEventIndices(int eventindex){
    if (enterNotifyEvents.data()){
        enterNotifyEvents.data()->updateEventIndices(eventindex);
    }
    if (exitNotifyEvents.data()){
        exitNotifyEvents.data()->updateEventIndices(eventindex);
    }
    if (transitions.data()){
        transitions.data()->updateEventIndices(eventindex);
    }
}

void hkbStateMachineStateInfo::mergeEventIndex(int oldindex, int newindex){
    if (enterNotifyEvents.data()){
        enterNotifyEvents.data()->mergeEventIndex(oldindex, newindex);
    }
    if (exitNotifyEvents.data()){
        exitNotifyEvents.data()->mergeEventIndex(oldindex, newindex);
    }
    if (transitions.data()){
        transitions.data()->mergeEventIndex(oldindex, newindex);
    }
}

void hkbStateMachineStateInfo::fixMergedEventIndices(BehaviorFile *dominantfile){
    if (enterNotifyEvents.data()){
        enterNotifyEvents.data()->fixMergedEventIndices(dominantfile);
    }
    if (exitNotifyEvents.data()){
        exitNotifyEvents.data()->fixMergedEventIndices(dominantfile);
    }
    if (transitions.data()){
        transitions.data()->fixMergedEventIndices(dominantfile);
    }
}

bool hkbStateMachineStateInfo::merge(HkxObject *recessiveObject){
    hkbStateMachineStateInfo *obj = nullptr;
    if (recessiveObject && recessiveObject->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        obj = static_cast<hkbStateMachineStateInfo *>(recessiveObject);
        injectWhileMerging((obj));
        if (enterNotifyEvents.data()){
            if (obj->enterNotifyEvents.data()){
                enterNotifyEvents.data()->merge(obj->enterNotifyEvents.data());
            }
        }else if (obj->enterNotifyEvents.data()){
            enterNotifyEvents = obj->enterNotifyEvents;
            getParentFile()->addObjectToFile(obj->enterNotifyEvents.data(), 0);
        }
        if (exitNotifyEvents.data()){
            if (obj->exitNotifyEvents.data()){
                exitNotifyEvents.data()->merge(obj->exitNotifyEvents.data());
            }
        }else if (obj->exitNotifyEvents.data()){
            exitNotifyEvents = obj->exitNotifyEvents;
            getParentFile()->addObjectToFile(obj->exitNotifyEvents.data(), 0);
        }
        if (transitions.data()){
            if (obj->transitions.data()){
                transitions.data()->merge(obj->transitions.data());
            }
        }else if (obj->transitions.data()){
            transitions = obj->transitions;
            getParentFile()->addObjectToFile(obj->transitions.data(), 0);
        }
        return true;
    }else{
        return false;
    }
}

void hkbStateMachineStateInfo::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (enterNotifyEvents.data()){
        ref++;
        enterNotifyEvents.data()->updateReferences(ref);
    }
    if (exitNotifyEvents.data()){
        ref++;
        exitNotifyEvents.data()->updateReferences(ref);
    }
    if (transitions.data()){
        ref++;
        transitions.data()->updateReferences(ref);
    }
}

QList<DataIconManager *> hkbStateMachineStateInfo::getChildren() const{
    QList<DataIconManager *> list;
    if (generator.data()){
        list.append(static_cast<DataIconManager*>(generator.data()));
    }
    return list;
}

int hkbStateMachineStateInfo::getIndexOfObj(DataIconManager *obj) const{
    if (generator.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool hkbStateMachineStateInfo::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        generator = HkxSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
    return true;
}

bool hkbStateMachineStateInfo::removeObjectAt(int index){
    if (index == 0 || index == -1){
        generator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbStateMachineStateInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "enterNotifyEvents"){
            if (!enterNotifyEvents.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enterNotifyEvents' reference!\nObject Reference: "+ref);
            }
        }else if (text == "exitNotifyEvents"){
            if (!exitNotifyEvents.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'exitNotifyEvents' reference!\nObject Reference: "+ref);
            }
        }else if (text == "transitions"){
            if (!transitions.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'transitions' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "stateId"){
            stateId = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'stateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "probability"){
            probability = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'probability' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbStateMachineStateInfo::write(HkxXMLWriter *writer){
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
        list1 = {writer->name, writer->numelements};
        list2 = {"listeners", "0"};
        writer->writeLine(writer->parameter, list1, list2, "");
        if (enterNotifyEvents.data()){
            refString = enterNotifyEvents.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enterNotifyEvents"), refString);
        if (exitNotifyEvents.data()){
            refString = exitNotifyEvents.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("exitNotifyEvents"), refString);
        if (transitions.data()){
            refString = transitions.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transitions"), refString);
        if (generator.data()){
            refString = generator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("generator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("stateId"), QString::number(stateId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("probability"), QString::number(probability, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (enterNotifyEvents.data() && !enterNotifyEvents.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'enterNotifyEvents'!!!");
        }
        if (exitNotifyEvents.data() && !exitNotifyEvents.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'exitNotifyEvents'!!!");
        }
        if (transitions.data() && !transitions.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'transitions'!!!");
        }
        if (generator.data() && !generator.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'generator'!!!");
        }
    }
    return true;
}

bool hkbStateMachineStateInfo::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(enterNotifyEvents.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'enterNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        enterNotifyEvents = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(exitNotifyEvents.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'exitNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        exitNotifyEvents = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(transitions.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'transitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
            setDataValidity(false);
        }
        transitions = *ptr;
        static_cast<hkbStateMachineTransitionInfoArray *>(transitions.data())->parent = parentSM;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(generator.getShdPtrReference());
    if (!ptr){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'generator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        WRITE_TO_LOG(getClassname()+": link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

void hkbStateMachineStateInfo::unlink(){
    HkDynamicObject::unlink();
    parentSM = nullptr;
    enterNotifyEvents = HkxSharedPtr();
    exitNotifyEvents = HkxSharedPtr();
    transitions = HkxSharedPtr();
    generator = HkxSharedPtr();
}

bool hkbStateMachineStateInfo::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity() || (enterNotifyEvents.data() && !enterNotifyEvents.data()->evaluateDataValidity() && enterNotifyEvents.data()->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY) ||
            (exitNotifyEvents.data() && !exitNotifyEvents.data()->evaluateDataValidity() && exitNotifyEvents.data()->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY) ||
            (transitions.data() && !transitions.data()->evaluateDataValidity() && transitions.data()->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY) ||
            (generator.data() && generator.data()->getType() != TYPE_GENERATOR) ||
            (!parentSM)
            )
    {
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbStateMachineStateInfo::~hkbStateMachineStateInfo(){
    refCount--;
}
