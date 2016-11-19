#include "hkbstatemachinetransitioninfoarray.h"
#include "generators/hkbstatemachine.h"
#include "generators/hkbstatemachinestateinfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/hkxobject.h"
/**
 * hkbStateMachineTransitionInfoArray
 */

uint hkbStateMachineTransitionInfoArray::refCount = 0;

QString hkbStateMachineTransitionInfoArray::classname = "hkbStateMachineTransitionInfoArray";

QStringList hkbStateMachineTransitionInfoArray::transitionFlags = {
    "0",
    "FLAG_USE_TRIGGER_INTERVAL",
    "FLAG_USE_INITIATE_INTERVAL",
    "FLAG_UNINTERRUPTIBLE_WHILE_PLAYING",
    "FLAG_UNINTERRUPTIBLE_WHILE_DELAYED",
    "FLAG_DELAY_STATE_CHANGE",
    "FLAG_DISABLED",
    "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE",
    "FLAG_DISALLOW_RANDOM_TRANSITION",
    "FLAG_DISABLE_CONDITION",
    "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE",
    "FLAG_IS_GLOBAL_WILDCARD",
    "FLAG_IS_LOCAL_WILDCARD",
    "FLAG_FROM_NESTED_STATE_ID_IS_VALID",
    "FLAG_TO_NESTED_STATE_ID_IS_VALID",
    "FLAG_ABUT_AT_END_OF_FROM_GENERATOR",
};

hkbStateMachineTransitionInfoArray::hkbStateMachineTransitionInfoArray(BehaviorFile *parent, hkbStateMachine *parentSM, long ref)
    : HkxObject(parent, ref),
      parent(reinterpret_cast<HkxObject *>(parentSM))
{
    setType(HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbStateMachineTransitionInfoArray::getClassname(){
    return classname;
}

int hkbStateMachineTransitionInfoArray::getNumTransitions() const{
    return transitions.size();
}

void hkbStateMachineTransitionInfoArray::addTransition(){
    transitions.append(HkTransition());
}

void hkbStateMachineTransitionInfoArray::removeTransition(int index){
    if (transitions.size() > index){
        transitions.removeAt(index);
    }
}

void hkbStateMachineTransitionInfoArray::removeTransitionToState(uint stateId){
    for (int i = transitions.size() - 1; i >= 0; i--){
        if (transitions.at(i).toStateId == stateId){
            transitions.removeAt(i);
        }
    }
}

QString hkbStateMachineTransitionInfoArray::getTransitionNameAt(int index){
    if (index > -1 && index < transitions.size()){
        if (parent.data()){
            hkbStateMachine *stateMachine;
            if (parent.data()->getSignature() == HKB_STATE_MACHINE){
                stateMachine = static_cast<hkbStateMachine *>(parent.data());
            }else if (parent.data()->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
                stateMachine = static_cast<hkbStateMachineStateInfo *>(parent.data())->getParentStateMachine();
            }
            if (stateMachine){
                if (transitions.at(index).flags.contains("FLAG_TO_NESTED_STATE_ID_IS_VALID")){
                    return "to_"+stateMachine->getNestedStateName(transitions.at(index).toStateId, transitions.at(index).toNestedStateId)+"_Via_"+stateMachine->getStateName(transitions.at(index).toStateId);
                }
                return "to_"+stateMachine->getStateName(transitions.at(index).toStateId);
            }
        }
    }
    return "";
}

bool hkbStateMachineTransitionInfoArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    INTERVAL_TYPE intervalType = NONE;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "transitions"){
            int numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                transitions.append(HkTransition());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    QString test = reader.getNthAttributeValueAt(index, 0);
                    if (reader.getNthAttributeValueAt(index, 0) == "triggerInterval"){
                        intervalType = TRIGGER;
                    }else if (reader.getNthAttributeValueAt(index, 0) == "initiateInterval"){
                        intervalType = INITIATE;
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enterEventId"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.enterEventId = reader.getElementValueAt(index).toInt(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.enterEventId = reader.getElementValueAt(index).toInt(&ok);
                        }
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'enterEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitEventId"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'exitEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enterTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'enterTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'exitTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "transition"){
                        if (!transitions.last().transition.readReference(index, reader)){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'transition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "condition"){
                        if (!transitions.last().condition.readReference(index, reader)){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'condition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "eventId"){
                        transitions.last().eventId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'eventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toStateId"){
                        transitions.last().toStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'toStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fromNestedStateId"){
                        transitions.last().fromNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'fromNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toNestedStateId"){
                        transitions.last().toNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'toNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "priority"){
                        transitions.last().priority = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbStateMachineTransitionInfoArray: readData()!\nFailed to properly read 'priority' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "flags"){
                        transitions.last().flags = reader.getElementValueAt(index);
                        QStringList list = transitions.last().flags.split('|');
                        for (int k = 0; k < list.size(); k++){
                            if (!transitionFlags.contains(list.at(k))){
                                writeToLog("hkbStateMachineTransitionInfoArray: readData()!\n'flags' data field contains an invalid string!\nObject Reference: "+ref);
                            }
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
            return true;
        }
        index++;
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < transitions.size(); i++){
        //transition
        ptr = getParentFile()->findHkxObject(transitions.at(i).transition.getReference());
        if (ptr){
            if ((*ptr)->getType() != HKB_BLENDING_TRANSITION_EFFECT || (*ptr)->getType() != HKB_GENERATOR_TRANSITION_EFFECT){
                writeToLog("hkbStateMachineTransitionInfoArray: link()!\n'transition' data field is linked to invalid child!");
            }
            transitions[i].transition = *ptr;
        }
        //condition
        ptr = getParentFile()->findHkxObject(transitions.at(i).condition.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_EXPRESSION_CONDITION){
                writeToLog("hkbStateMachineTransitionInfoArray: link()!\n'condition' data field is linked to invalid child!");
            }
            transitions[i].condition = *ptr;
        }
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::evaulateDataValidity(){
    for (int i = 0; i < transitions.size(); i++){
        if (transitions.at(i).condition.data() && transitions.at(i).condition.data()->getSignature() != HKB_EXPRESSION_CONDITION){
            setDataValidity(false);
            return false;
        }else if (transitions.at(i).transition.data() && (transitions.at(i).transition.data()->getSignature() != HKB_BLENDING_TRANSITION_EFFECT || transitions.at(i).transition.data()->getSignature() != HKB_GENERATOR_TRANSITION_EFFECT)){
            setDataValidity(false);
            return false;
        }else if (transitions.at(i).flags == ""){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbStateMachineTransitionInfoArray::~hkbStateMachineTransitionInfoArray(){
    refCount--;
}
