#include "hkbstatemachinestateinfo.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbStateMachineStateInfo
*/

uint hkbStateMachineStateInfo::refCount = 0;

QString hkbStateMachineStateInfo::classname = "hkbStateMachineStateInfo";

hkbStateMachineStateInfo::hkbStateMachineStateInfo(BehaviorFile *parent, hkbStateMachine *parentSM, long ref)
    : hkbGenerator(parent, ref),
      name("State"),
      stateId(0),
      probability(1),
      enable(true),
      parentSM(reinterpret_cast<HkxObject *>(parentSM))
{
    setType(HKB_STATE_MACHINE_STATE_INFO, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbStateMachineStateInfo::getClassname(){
    return classname;
}

QString hkbStateMachineStateInfo::getName() const{
    return name;
}

hkbStateMachine * hkbStateMachineStateInfo::getParentStateMachine() const{
    if (parentSM.data() && parentSM.data()->getSignature() == HKB_STATE_MACHINE){
        return reinterpret_cast<hkbStateMachine *>(parentSM.data());
    }
    return NULL;
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
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(enterNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'enterNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        enterNotifyEvents = *ptr;
    }
    //exitNotifyEvents
    ptr = getParentFile()->findHkxObject(exitNotifyEvents.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'exitNotifyEvents' is not a HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY!");
            setDataValidity(false);
        }
        exitNotifyEvents = *ptr;
    }
    //transitions
    ptr = getParentFile()->findHkxObject(transitions.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            writeToLog("hkbStateMachineStateInfo: linkVar()!\nThe linked object 'transitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
            setDataValidity(false);
        }
        transitions = *ptr;
        static_cast<hkbStateMachineTransitionInfoArray *>(transitions.data())->parent = this;
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

void hkbStateMachineStateInfo::unlink(){
    HkDynamicObject::unlink();
    enterNotifyEvents = HkxObjectExpSharedPtr();
    exitNotifyEvents = HkxObjectExpSharedPtr();
    transitions = HkxObjectExpSharedPtr();
    generator = HkxObjectExpSharedPtr();
}

bool hkbStateMachineStateInfo::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (enterNotifyEvents.data() && enterNotifyEvents.data()->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
    }else if (exitNotifyEvents.data() && exitNotifyEvents.data()->getSignature() != HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
    }else if (transitions.data() && transitions.data()->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
    }else if (!generator.data() || generator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbStateMachineStateInfo::~hkbStateMachineStateInfo(){
    refCount--;
}
