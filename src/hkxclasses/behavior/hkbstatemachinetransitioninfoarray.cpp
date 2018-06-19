#include "hkbstatemachinetransitioninfoarray.h"
#include "generators/hkbstatemachine.h"
#include "generators/hkbstatemachinestateinfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
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

bool hkbStateMachineTransitionInfoArray::HkTransition::operator ==(const HkTransition & other) const{
    if (
            triggerInterval != other.triggerInterval ||
            initiateInterval != other.initiateInterval ||
            eventId != other.eventId ||
            toStateId != other.toStateId ||
            fromNestedStateId != other.fromNestedStateId ||
            toNestedStateId != other.toNestedStateId ||
            priority != other.priority
            )
    {
        return false;
    }
    QStringList domflags = flags.split("|");
    QStringList recflags = other.flags.split("|");
    //TO DO...
    hkbExpressionCondition *exp = static_cast<hkbExpressionCondition *>(condition.data());
    hkbExpressionCondition *otherexp = static_cast<hkbExpressionCondition *>(other.condition.data());
    hkbBlendingTransitionEffect *effect = static_cast<hkbBlendingTransitionEffect *>(transition.data());
    hkbBlendingTransitionEffect *othereffect = static_cast<hkbBlendingTransitionEffect *>(other.transition.data());
    if ((((exp && otherexp) && *exp == *otherexp) || (!exp && !otherexp)) && (((effect && othereffect) && *effect == *othereffect) || (!effect && !othereffect))){
        return true;
    }
    return false;
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
    if (transitions.size() > index && index > -1){
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
        if (parent){
            hkbStateMachine *stateMachine;
            if (parent->getSignature() == HKB_STATE_MACHINE){
                stateMachine = static_cast<hkbStateMachine *>(parent);
            }else if (parent->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
                stateMachine = static_cast<hkbStateMachineStateInfo *>(parent)->getParentStateMachine();
            }
            if (stateMachine){
                if (transitions.at(index).flags.contains("FLAG_TO_NESTED_STATE_ID_IS_VALID")){
                    return "to_"+stateMachine->getNestedStateName(transitions.at(index).toStateId, transitions.at(index).toNestedStateId)+"_Via_"+stateMachine->getStateName(transitions.at(index).toStateId);
                }
                return "to_"+stateMachine->getStateName(transitions.at(index).toStateId);
            }
        }else{
            WARNING_MESSAGE(QString("hkbStateMachineTransitionInfoArray::getTransitionNameAt: No parent!!! REF: "+getReferenceString()));
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
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enterEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitEventId"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitEventId = reader.getElementValueAt(index).toInt(&ok);
                        }
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'exitEventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enterTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.enterTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enterTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "exitTime"){
                        if (intervalType == TRIGGER){
                            transitions.last().triggerInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }else if (intervalType == INITIATE){
                            transitions.last().initiateInterval.exitTime = reader.getElementValueAt(index).toDouble(&ok);
                        }
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'exitTime' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "transition"){
                        if (!transitions.last().transition.readShdPtrReference(index, reader)){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'transition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "condition"){
                        if (!transitions.last().condition.readShdPtrReference(index, reader)){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'condition' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "eventId"){
                        transitions.last().eventId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'eventId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toStateId"){
                        transitions.last().toStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'toStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fromNestedStateId"){
                        transitions.last().fromNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fromNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toNestedStateId"){
                        transitions.last().toNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'toNestedStateId' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "priority"){
                        transitions.last().priority = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'priority' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "flags"){
                        transitions.last().flags = reader.getElementValueAt(index);
                        QStringList list = transitions.last().flags.split('|');
                        for (int k = 0; k < list.size(); k++){
                            if (!transitionFlags.contains(list.at(k))){
                                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\n'flags' data field contains an invalid string!\nObject Reference: "+ref);
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
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'transition'' at "+QString::number(i));
            }
            if (transitions.at(i).condition.data() && !transitions.at(i).condition.data()->write(writer)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'condition'' at "+QString::number(i));
            }
        }
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::isEventReferenced(int eventindex) const{
    for (auto i = 0; i < transitions.size(); i++){
        if (transitions.at(i).triggerInterval.enterEventId == eventindex ||
                transitions.at(i).triggerInterval.exitEventId == eventindex ||
                transitions.at(i).initiateInterval.enterEventId == eventindex ||
                transitions.at(i).initiateInterval.exitEventId == eventindex ||
                transitions.at(i).eventId == eventindex)
        {
            return true;
        }
    }
    return false;
}

void hkbStateMachineTransitionInfoArray::updateEventIndices(int eventindex){
    for (auto i = 0; i < transitions.size(); i++){
        if (transitions.at(i).triggerInterval.enterEventId > eventindex){
            transitions[i].triggerInterval.enterEventId--;
        }
        if (transitions.at(i).triggerInterval.exitEventId > eventindex){
            transitions[i].triggerInterval.exitEventId--;
        }
        if (transitions.at(i).initiateInterval.enterEventId > eventindex){
            transitions[i].initiateInterval.enterEventId--;
        }
        if (transitions.at(i).initiateInterval.enterEventId > eventindex){
            transitions[i].initiateInterval.enterEventId--;
        }
        if (transitions.at(i).eventId > eventindex){
            transitions[i].eventId--;
        }
    }
}

void hkbStateMachineTransitionInfoArray::mergeEventIndex(int oldindex, int newindex){
    for (auto i = 0; i < transitions.size(); i++){
        if (transitions.at(i).triggerInterval.enterEventId == oldindex){
            transitions[i].triggerInterval.enterEventId = newindex;
        }
        if (transitions.at(i).triggerInterval.exitEventId == oldindex){
            transitions[i].triggerInterval.exitEventId = newindex;
        }
        if (transitions.at(i).initiateInterval.enterEventId == oldindex){
            transitions[i].initiateInterval.enterEventId = newindex;
        }
        if (transitions.at(i).initiateInterval.enterEventId == oldindex){
            transitions[i].initiateInterval.enterEventId = newindex;
        }
        if (transitions.at(i).eventId == oldindex){
            transitions[i].eventId = newindex;
        }
    }
}

void hkbStateMachineTransitionInfoArray::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            for (auto i = 0; i < transitions.size(); i++){
                thiseventname = recdata->getEventNameAt(transitions.at(i).eventId);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                transitions[i].eventId = eventindex;
            }
            for (auto i = 0; i < transitions.size(); i++){
                thiseventname = recdata->getEventNameAt(transitions.at(i).triggerInterval.enterEventId);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                transitions[i].triggerInterval.enterEventId = eventindex;
            }
            for (auto i = 0; i < transitions.size(); i++){
                thiseventname = recdata->getEventNameAt(transitions.at(i).triggerInterval.exitEventId);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                transitions[i].triggerInterval.exitEventId = eventindex;
            }
            for (auto i = 0; i < transitions.size(); i++){
                thiseventname = recdata->getEventNameAt(transitions.at(i).initiateInterval.enterEventId);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                transitions[i].initiateInterval.enterEventId = eventindex;
            }
            for (auto i = 0; i < transitions.size(); i++){
                thiseventname = recdata->getEventNameAt(transitions.at(i).initiateInterval.exitEventId);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                transitions[i].initiateInterval.exitEventId = eventindex;
            }
        }
        setIsMerged(true);
    }
}

bool hkbStateMachineTransitionInfoArray::merge(HkxObject *recessiveObject){
    hkbStateMachine *statemachine;
    hkbStateMachineStateInfo *state;
    hkbStateMachineTransitionInfoArray *obj = nullptr;
    if (recessiveObject && recessiveObject->getSignature() == HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
        obj = static_cast<hkbStateMachineTransitionInfoArray *>(recessiveObject);
        obj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        if (parent){
            if (parent->getSignature() == HKB_STATE_MACHINE){
                statemachine = static_cast<hkbStateMachine *>(parent);
                for (auto i = 0; i < obj->transitions.size(); i++){
                    if (!transitions.contains(obj->transitions.at(i)) && statemachine->getStateName(obj->transitions.at(i).toStateId) != "" &&
                          (!obj->transitions.at(i).flags.contains(transitionFlags.at(14)) || statemachine->getNestedStateName(obj->transitions.at(i).toStateId, obj->transitions.at(i).toNestedStateId) != ""))
                    {
                        transitions.append(obj->transitions.at(i));
                    }
                }
            }else if (parent->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
                state = static_cast<hkbStateMachineStateInfo *>(parent);
                for (auto i = 0; i < obj->transitions.size(); i++){
                    if (!transitions.contains(obj->transitions.at(i)) && state->getStateName(obj->transitions.at(i).toStateId) != "" &&
                          (!obj->transitions.at(i).flags.contains(transitionFlags.at(14)) || state->getNestedStateName(obj->transitions.at(i).toStateId, obj->transitions.at(i).toNestedStateId) != ""))
                    {
                        transitions.append(obj->transitions.at(i));
                    }
                }
            }
        }
        return true;
    }else{
        return false;
    }
}

void hkbStateMachineTransitionInfoArray::updateReferences(long &ref){
    setReference(ref);
    for (auto i = 0; i < transitions.size(); i++){
        if (transitions.at(i).transition.data()){
            ref++;
            transitions[i].transition.data()->updateReferences(ref);
        }
        if (transitions.at(i).condition.data()){
            ref++;
            transitions[i].condition.data()->updateReferences(ref);
        }
    }
}

QVector<HkxObject *> hkbStateMachineTransitionInfoArray::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    for (auto i = 0; i < transitions.size(); i++){
        if (transitions.at(i).transition.data()){
            list = list + transitions.at(i).transition.data()->getChildrenOtherTypes();
        }
        if (transitions.at(i).condition.data()){
            list = list + transitions.at(i).condition.data()->getChildrenOtherTypes();
        }
    }
    return list;
}

bool hkbStateMachineTransitionInfoArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < transitions.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(transitions.at(i).transition.getShdPtrReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_BLENDING_TRANSITION_EFFECT && (*ptr)->getSignature() != HKB_GENERATOR_TRANSITION_EFFECT){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\n'transition' data field is linked to invalid child!");
            }
            transitions[i].transition = *ptr;
        }
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(transitions.at(i).condition.getShdPtrReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_EXPRESSION_CONDITION && (*ptr)->getSignature() != HKB_STRING_CONDITION){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\n'condition' data field is linked to invalid child!");
            }
            transitions[i].condition = *ptr;
        }
    }
    return true;
}

bool hkbStateMachineTransitionInfoArray::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (!parent){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Null parent!\n");
    }else if (parent->getSignature() != HKB_STATE_MACHINE && parent->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Invalid parent type! Signature: "+QString::number(parent->getSignature(), 16)+"\n");
    }else{
        if (transitions.isEmpty()){
            isvalid = false;
            errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": transitions is empty!\n");
        }else{
            for (auto i = 0; i < transitions.size(); i++){
                if (transitions.at(i).condition.data()){
                    if (transitions.at(i).condition.data()->getSignature() != HKB_EXPRESSION_CONDITION){
                        isvalid = false;
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Invalid condition type! Signature: "+QString::number(transitions.at(i).condition.data()->getSignature(), 16)+"\n");
                    }else if (transitions.at(i).condition.data()->isDataValid() && !transitions.at(i).condition.data()->evaluateDataValidity()){
                        isvalid = false;
                        //errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Invalid condition data!\n");
                    }
                }
                if (transitions.at(i).transition.data()){
                    if (transitions.at(i).transition.data()->getSignature() != HKB_BLENDING_TRANSITION_EFFECT/* || transitions.at(i).condition.data()->getSignature() != HKB_GENERATOR_TRANSITION_EFFECT*/){
                        isvalid = false;
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Invalid transition type! Signature: "+QString::number(transitions.at(i).transition.data()->getSignature(), 16)+"\n");
                    }else if (transitions.at(i).transition.data()->isDataValid() && !transitions.at(i).transition.data()->evaluateDataValidity()){
                        isvalid = false;
                        //errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": Invalid transition data!\n");
                    }
                }
                if (transitions.at(i).eventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
                    isvalid = false;
                    errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": eventId in transitions at "+QString::number(i)+" out of range!\n");
                }
                if (parent->getSignature() == HKB_STATE_MACHINE){
                    if (static_cast<hkbStateMachine *>(parent)->getStateName(transitions.at(i).toStateId) == ""){
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": toStateId in transitions at "+QString::number(i)+" does not exist!\n");
                    }
                    if (transitions.at(i).flags.contains("FLAG_TO_NESTED_STATE_ID_IS_VALID") && static_cast<hkbStateMachine *>(parent)->getNestedStateName(transitions.at(i).toStateId, transitions.at(i).toNestedStateId) == ""){
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": toNestedStateId in transitions at "+QString::number(i)+" does not exist!\n");
                    }
                }else if (parent->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
                    if (static_cast<hkbStateMachineStateInfo *>(parent)->getStateName(transitions.at(i).toStateId) == ""){
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": toStateId in transitions at "+QString::number(i)+" does not exist!\n");
                    }
                    if (transitions.at(i).flags.contains("FLAG_TO_NESTED_STATE_ID_IS_VALID") && static_cast<hkbStateMachineStateInfo *>(parent)->getNestedStateName(transitions.at(i).toStateId, transitions.at(i).toNestedStateId) == ""){
                        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": toNestedStateId in transitions at "+QString::number(i)+" does not exist!\n");
                    }
                }
            }
        }
        if (errors != ""){
            LogFile::writeToLog(errors);
        }
        setDataValidity(isvalid);
        return isvalid;
    }
    return false;
}

hkbStateMachineTransitionInfoArray::~hkbStateMachineTransitionInfoArray(){
    refCount--;
}
