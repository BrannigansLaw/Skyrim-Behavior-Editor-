#include "hkbstatemachinetransitioninfoarray.h"
#include "generators/hkbstatemachine.h"
#include "generators/hkbstatemachinestateinfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
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

hkbStateMachineTransitionInfoArray::hkbStateMachineTransitionInfoArray(HkxFile *parent, hkbStateMachine *parentSM, long ref)
    : HkxObject(parent, ref),
      parent(reinterpret_cast<HkxObject *>(parentSM))
{
    setType(HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
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
    int numtrans = 0;
    INTERVAL_TYPE intervalType = NONE;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "transitions"){
            numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
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
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'enterEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitEventId"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'exitEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enterTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'enterTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'exitTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "transition"){
                        if (!transitions.last().transition.readReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'transition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "condition"){
                        if (!transitions.last().condition.readReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'condition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "eventId"){
                        transitions.last().eventId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toStateId"){
                        transitions.last().toStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'toStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fromNestedStateId"){
                        transitions.last().fromNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'fromNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toNestedStateId"){
                        transitions.last().toNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'toNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "priority"){
                        transitions.last().priority = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'priority' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "flags"){
                        transitions.last().flags = reader.getElementValueAt(index);
                        QStringList list = transitions.last().flags.split('|');
                        for (int k = 0; k < list.size(); k++){
                            if (!transitionFlags.contains(list.at(k))){
                                writeToLog(getClassname()+": readData()!\n'flags' data field contains an invalid string!\nObject Reference: "+ref);
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

bool hkbStateMachineTransitionInfoArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"transitions", QString::number(transitions.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < transitions.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("triggerInterval"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enterEventId"), QString::number(transitions.at(i).triggerInterval.enterEventId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("exitEventId"), QString::number(transitions.at(i).triggerInterval.exitEventId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enterTime"), QString::number(transitions.at(i).triggerInterval.enterTime, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("exitTime"), QString::number(transitions.at(i).triggerInterval.exitTime, char('f'), 6));
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("initiateInterval"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enterEventId"), QString::number(transitions.at(i).initiateInterval.enterEventId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("exitEventId"), QString::number(transitions.at(i).initiateInterval.exitEventId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enterTime"), QString::number(transitions.at(i).initiateInterval.enterTime, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("exitTime"), QString::number(transitions.at(i).initiateInterval.exitTime, char('f'), 6));
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            if (transitions.at(i).transition.data()){
                refString = transitions.at(i).transition.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transition"), refString);
            if (transitions.at(i).condition.data()){
                refString = transitions.at(i).condition.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("condition"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventId"), QString::number(transitions.at(i).eventId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("toStateId"), QString::number(transitions.at(i).toStateId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fromNestedStateId"), QString::number(transitions.at(i).fromNestedStateId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("toNestedStateId"), QString::number(transitions.at(i).toNestedStateId));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("priority"), QString::number(transitions.at(i).priority));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), transitions.at(i).flags);
            writer->writeLine(writer->object, false);
        }
        if (transitions.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < transitions.size(); i++){
            if (transitions.at(i).transition.data() && !transitions.at(i).transition.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'transition'' at "+QString::number(i), true);
            }
            if (transitions.at(i).condition.data() && !transitions.at(i).condition.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'condition'' at "+QString::number(i), true);
            }
        }
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < transitions.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(transitions.at(i).transition.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_BLENDING_TRANSITION_EFFECT && (*ptr)->getSignature() != HKB_GENERATOR_TRANSITION_EFFECT){
                writeToLog(getClassname()+": link()!\n'transition' data field is linked to invalid child!");
            }
            transitions[i].transition = *ptr;
        }
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(transitions.at(i).condition.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_EXPRESSION_CONDITION && (*ptr)->getSignature() != HKB_STRING_CONDITION){
                writeToLog(getClassname()+": link()!\n'condition' data field is linked to invalid child!");
            }
            transitions[i].condition = *ptr;
        }
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::evaulateDataValidity(){
    for (int i = 0; i < transitions.size(); i++){
        if (transitions.at(i).condition.data() && transitions.at(i).condition.data()->getSignature() != HKB_EXPRESSION_CONDITION && transitions.at(i).condition.data()->getSignature() != HKB_STRING_CONDITION){
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
