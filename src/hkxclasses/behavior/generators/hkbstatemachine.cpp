#include "hkbstatemachine.h"
#include "hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorreferencegenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
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
      startStateId(0),
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
    name = "StateMachine"+QString::number(refCount);
}

QString hkbStateMachine::getClassname(){
    return classname;
}

QString hkbStateMachine::getName() const{
    return name;
}

int hkbStateMachine::generateValidStateId(){
    ulong stateId = 0;
    ulong prev = 0;
    hkbStateMachineStateInfo *ptr = nullptr;
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

QString hkbStateMachine::getStateName(ulong stateId) const{
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state && state->stateId == stateId){
            return state->name;
        }
    }
    return "";
}

int hkbStateMachine::getStateId(const QString &statename) const{
    hkbStateMachineStateInfo *state;
    for (int i = 0; i < states.size(); i++){
        state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (state && state->name == statename){
            return state->stateId;
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

QString hkbStateMachine::getNestedStateName(ulong stateId, ulong nestedStateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getStateName(nestedStateId);
    }
    return "";
}

QStringList hkbStateMachine::getNestedStateNames(ulong stateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getStateNames();
    }
    return QStringList();
}

int hkbStateMachine::getNumberOfNestedStates(ulong stateId) const{
    hkbStateMachine *ptr = getNestedStateMachine(stateId);
    if (ptr){
        return ptr->getNumberOfStates();
    }
    return -1;
}

bool hkbStateMachine::insertObjectAt(int index, DataIconManager *obj){
    if (obj->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        if (index >= states.size() || index == -1){
            states.append(HkxSharedPtr(obj));
        }else if (index == 0 || !states.isEmpty()){
            states.replace(index, HkxSharedPtr(obj));
        }
        return true;
    }else{
        return false;
    }
}

/*bool hkbStateMachine::insertObjectAt(int index, DataIconManager *obj){
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
}*/

bool hkbStateMachine::removeObjectAt(int index){
    if (index < states.size()){
        hkbStateMachineStateInfo *state = nullptr;
        hkbStateMachineTransitionInfoArray *trans = nullptr;
        ulong stateId = 0;
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
                //state = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
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
    //hkbStateMachineStateInfo *child;
    for (int i = 0; i < states.size(); i++){
        /*child = static_cast<hkbStateMachineStateInfo *>(states.at(i).data());
        if (child->generator.data()){
            return true;
        }*/
        if (static_cast<hkbStateMachineStateInfo *>(states.at(i).data())){
            return true;
        }
    }
    return false;
}

bool hkbStateMachine::isEventReferenced(int eventindex) const{
    if (eventToSendWhenStateOrTransitionChanges.id == eventindex ||
            returnToPreviousStateEventId == eventindex ||
            randomTransitionEventId == eventindex ||
            transitionToNextHigherStateEventId == eventindex ||
            transitionToNextLowerStateEventId == eventindex ||
            (wildcardTransitions.constData() && wildcardTransitions.constData()->isEventReferenced(eventindex)))
    {
        return true;
    }
    return false;
}

void hkbStateMachine::updateEventIndices(int eventindex){
    if (eventToSendWhenStateOrTransitionChanges.id > eventindex){
        eventToSendWhenStateOrTransitionChanges.id--;
    }
    if (returnToPreviousStateEventId > eventindex){
        returnToPreviousStateEventId--;
    }
    if (randomTransitionEventId > eventindex){
        randomTransitionEventId--;
    }
    if (transitionToNextHigherStateEventId > eventindex){
        transitionToNextHigherStateEventId--;
    }
    if (transitionToNextLowerStateEventId > eventindex){
        transitionToNextLowerStateEventId--;
    }
    if (wildcardTransitions.data()){
        wildcardTransitions.data()->updateEventIndices(eventindex);
    }
}

void hkbStateMachine::mergeEventIndex(int oldindex, int newindex){
    if (eventToSendWhenStateOrTransitionChanges.id == oldindex){
        eventToSendWhenStateOrTransitionChanges.id = newindex;
    }
    if (returnToPreviousStateEventId == oldindex){
        returnToPreviousStateEventId = newindex;
    }
    if (randomTransitionEventId == oldindex){
        randomTransitionEventId = newindex;
    }
    if (transitionToNextHigherStateEventId == oldindex){
        transitionToNextHigherStateEventId = newindex;
    }
    if (transitionToNextLowerStateEventId == oldindex){
        transitionToNextLowerStateEventId = newindex;
    }
    if (wildcardTransitions.data()){
        wildcardTransitions.data()->mergeEventIndex(oldindex, newindex);
    }
}

void hkbStateMachine::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            auto fixIndex = [&](int & id){ if (id < 0){return;}  //TO DO: Check if event id is -1?
                thiseventname = recdata->getEventNameAt(id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                id = eventindex;
            };
            fixIndex(eventToSendWhenStateOrTransitionChanges.id);
            fixIndex(returnToPreviousStateEventId);
            fixIndex(randomTransitionEventId);
            fixIndex(transitionToNextHigherStateEventId);
            fixIndex(transitionToNextLowerStateEventId);
            if (wildcardTransitions.data()){
                wildcardTransitions.data()->fixMergedEventIndices(dominantfile);
            }
            setIsMerged(true);
        }
    }
}

bool hkbStateMachine::merge(HkxObject *recessiveObject){
    hkbStateMachine *recobj = nullptr;
    hkbStateMachineStateInfo *thisobjstate = nullptr;
    hkbStateMachineStateInfo *otherobjstate = nullptr;
    ulong thisstateid = 0;
    ulong otherstateid = 0;
    ulong freeid = 0;
    bool add;
    QList <DataIconManager *> objects;
    QList <DataIconManager *> children;
    DataIconManager * obj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_STATE_MACHINE){
        recobj = static_cast<hkbStateMachine *>(recessiveObject);
        recobj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        if (variableBindingSet.data()){
            variableBindingSet.data()->merge(recobj->variableBindingSet.data());
        }else if (recobj->variableBindingSet.data()){
            variableBindingSet = HkxSharedPtr(recobj->variableBindingSet.data());
            recobj->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
            getParentFile()->addObjectToFile(recobj->variableBindingSet.data(), -1);
        }
        for (auto i = 0; i < recobj->states.size(); i++){
            add = true;
            otherobjstate = static_cast<hkbStateMachineStateInfo *>(recobj->states.at(i).data());
            otherstateid = otherobjstate->stateId;
            for (auto j = 0; j < states.size(); j++){
                thisobjstate = static_cast<hkbStateMachineStateInfo *>(states.at(j).data());
                thisstateid = thisobjstate->stateId;
                if (thisstateid >= freeid){
                    freeid = thisstateid + 1;
                }
                if (otherstateid == thisstateid){
                    if (thisobjstate->getName() != otherobjstate->getName()){
                        for (auto k = j + 1; k < states.size(); k++){
                            thisobjstate = static_cast<hkbStateMachineStateInfo *>(states.at(k).data());
                            thisstateid = thisobjstate->stateId;
                            if (thisstateid >= freeid){
                                freeid = thisstateid + 1;
                            }
                        }
                        otherobjstate->parentSM = this;
                        otherobjstate->setStateId(freeid);
                        i = recobj->states.size();
                        j = states.size();
                    }else{
                        add = false;
                    }
                }
            }
            if (add && (/* For FNIS*/otherobjstate->getName().contains("TKDodge") || static_cast<BehaviorFile *>(getParentFile())->isNameUniqueInProject(otherobjstate))){
                insertObjectAt(i, otherobjstate);
                if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(otherobjstate)){
                    otherobjstate->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                    otherobjstate->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
                    getParentFile()->addObjectToFile(otherobjstate, -1);
                    getParentFile()->addObjectToFile(otherobjstate->variableBindingSet.data(), -1);
                }
                objects = otherobjstate->getChildren();
                while (!objects.isEmpty()){
                    obj = objects.last();
                    if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(obj)){
                        obj->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                        obj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
                        getParentFile()->addObjectToFile(obj, -1);
                        getParentFile()->addObjectToFile(obj->variableBindingSet.data(), -1);
                        children = obj->getChildren();
                    }
                    objects.removeLast();
                    objects = objects + children;
                    children.clear();
                }
            }
        }
        if (wildcardTransitions.data()){
            if (recobj->wildcardTransitions.data()){
                wildcardTransitions.data()->merge(recobj->wildcardTransitions.data());
            }
        }else if (recobj->wildcardTransitions.data()){
            wildcardTransitions = recobj->wildcardTransitions;
            getParentFile()->addObjectToFile(recobj->wildcardTransitions.data(), -1);
            //obj->wildcardTransitions.data()->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        }
        setIsMerged(true);
        return true;
    }else{
        return false;
    }
}

void hkbStateMachine::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (eventToSendWhenStateOrTransitionChanges.payload.data()){
        ref++;
        eventToSendWhenStateOrTransitionChanges.payload.data()->setReference(ref);
    }
    if (wildcardTransitions.data()){
        ref++;
        wildcardTransitions.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbStateMachine::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (eventToSendWhenStateOrTransitionChanges.payload.data()){
        list.append(eventToSendWhenStateOrTransitionChanges.payload.data());
    }
    if (wildcardTransitions.data()){
        list.append(wildcardTransitions.data());
    }
    return list;
}

int hkbStateMachine::getIndexOfObj(DataIconManager *obj) const{
    for (int i = 0; i < states.size(); i++){
        if (states.at(i).data() && (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

QList<DataIconManager *> hkbStateMachine::getChildren() const{
    QList<DataIconManager *> list;
    for (int i = 0; i < states.size(); i++){
        if (states.at(i).data()){
            list.append(static_cast<DataIconManager*>(states.at(i).data()));
        }
    }
    return list;
}

hkbStateMachine * hkbStateMachine::getNestedStateMachine(ulong stateId) const{
    hkbGenerator *gen = nullptr;
    hkbStateMachineStateInfo *state = nullptr;
    qlonglong sig = 0;
    QString behaviorname;
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
                case HKB_BEHAVIOR_REFERENCE_GENERATOR:
                    behaviorname = reinterpret_cast<hkbBehaviorReferenceGenerator *>(gen)->getBehaviorName();
                    return static_cast<BehaviorFile *>(getParentFile())->findRootStateMachineFromBehavior(behaviorname);
                default:
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

bool hkbStateMachine::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            eventToSendWhenStateOrTransitionChanges.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!eventToSendWhenStateOrTransitionChanges.payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        /*}*else if (text == "startStateChooser"){
            if (!generator.readReference(index, reader)){
                return false;
            }*/
        }else if (text == "startStateId"){
            startStateId = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'startStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "returnToPreviousStateEventId"){
            returnToPreviousStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'returnToPreviousStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextHigherStateEventId"){
            transitionToNextHigherStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'transitionToNextHigherStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "transitionToNextLowerStateEventId"){
            transitionToNextLowerStateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'transitionToNextLowerStateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "syncVariableIndex"){
            syncVariableIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'syncVariableIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "wrapAroundStateId"){
            wrapAroundStateId = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'wrapAroundStateId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxSimultaneousTransitions"){
            maxSimultaneousTransitions = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'maxSimultaneousTransitions' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startStateMode"){
            startStateMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nInvalid 'startStateMode' data!\nObject Reference: "+ref);
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nInvalid 'selfTransitionMode' data!\nObject Reference: "+ref);
            }
        }else if (text == "states"){
            if (!readReferences(reader.getElementValueAt(index), states)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'states' references!\nObject Reference: "+ref);
            }
        }else if (text == "wildcardTransitions"){
            if (!wildcardTransitions.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'wildcardTransitions' reference!\nObject Reference: "+ref);
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
        for (int i = 0, j = 1; i < states.size(); i++, j++){
            refString.append(states.at(i).data()->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (states.size() > 0){
            if (refString.endsWith(" \0")){
                refString.remove(refString.lastIndexOf(" "), 1);
            }
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
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (eventToSendWhenStateOrTransitionChanges.payload.data() && !eventToSendWhenStateOrTransitionChanges.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
        if (wildcardTransitions.data() && !wildcardTransitions.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'wildcardTransitions'!!!");
        }
        for (int i = 0; i < states.size(); i++){
            if (states.at(i).data() && !states.at(i).data()->write(writer)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'states' at: "+QString::number(i)+"!!!");
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
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSendWhenStateOrTransitionChanges.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
        }
        eventToSendWhenStateOrTransitionChanges.payload = *ptr;
    }
    for (int i = 0; i < states.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(states.at(i).getShdPtrReference());
        if (!ptr){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'states' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            states[i] = *ptr;
        }else{
            states[i] = *ptr;
            static_cast<hkbStateMachineStateInfo *>(states[i].data())->parentSM = this;
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(static_cast<hkbStateMachineStateInfo *>(states[i].data())->transitions.data());
            if (trans){
                trans->parent = this;
            }
        }
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(wildcardTransitions.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'wildcardTransitions' is not a HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY!");
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

QString hkbStateMachine::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (states.isEmpty()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": states is empty!\n");
    }else{
        for (auto i = states.size() - 1; i >= 0; i--){
            if (!states.at(i).data()){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": states at index '"+QString::number(i)+"' is null! Removing child!\n");
                states.removeAt(i);
            }else if (states.at(i).data()->getSignature() != HKB_STATE_MACHINE_STATE_INFO){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid state! Signature: "+QString::number(states.at(i).data()->getSignature(), 16)+" Removing child!\n");
                states.removeAt(i);
            }
        }
    }
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (getStateName(startStateId) == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid startStateId! Setting default value!\n");
        if (!states.isEmpty()){
            startStateId = static_cast<hkbStateMachineStateInfo *>(states.first().data())->stateId;
        }else{
            startStateId = -1;
        }
    }
    if (eventToSendWhenStateOrTransitionChanges.payload.data() && eventToSendWhenStateOrTransitionChanges.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToSendWhenStateOrTransitionChanges.payload type! Signature: "+QString::number(eventToSendWhenStateOrTransitionChanges.payload.data()->getSignature(), 16)+"\n");
    }
    if (returnToPreviousStateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": returnToPreviousStateEventId event id out of range!\n");
    }
    if (randomTransitionEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": randomTransitionEventId event id out of range!\n");
    }
    if (transitionToNextHigherStateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": transitionToNextHigherStateEventId event id out of range!\n");
    }
    if (transitionToNextLowerStateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": transitionToNextLowerStateEventId event id out of range!\n");
    }
    if (syncVariableIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfVariables()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": syncVariableIndex out of range!\n");
    }
    if (startStateMode == "START_STATE_MODE_SYNC" && syncVariableIndex < 0){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Using START_STATE_MODE_SYNC but syncVariableIndex is not set!\n");
    }
    if (!StartStateMode.contains(startStateMode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid startStateMode!\n");
    }
    if (!SelfTransitionMode.contains(selfTransitionMode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid selfTransitionMode!\n");
    }
    if (maxSimultaneousTransitions > 32 || maxSimultaneousTransitions < 0){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid maxSimultaneousTransitions!\n");
    }
    if (wildcardTransitions.data()){
        if (wildcardTransitions.data()->getSignature() != HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid wildcardTransitions type! Signature: "+QString::number(wildcardTransitions.data()->getSignature(), 16)+"\n");
        }else if (wildcardTransitions.data()->isDataValid() && wildcardTransitions.data()->evaluateDataValidity() != ""){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid wildcardTransitions data!\n");
        }
    }
    setDataValidity(isvalid);
    return errors;
}

hkbStateMachine::~hkbStateMachine(){
    refCount--;
}
