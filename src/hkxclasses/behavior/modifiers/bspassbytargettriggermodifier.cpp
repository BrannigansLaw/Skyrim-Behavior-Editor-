#include "bspassbytargettriggermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSPassByTargetTriggerModifier
*/

uint BSPassByTargetTriggerModifier::refCount = 0;

QString BSPassByTargetTriggerModifier::classname = "BSPassByTargetTriggerModifier";

BSPassByTargetTriggerModifier::BSPassByTargetTriggerModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      radius(0)
{
    setType(BS_PASS_BY_TARGET_TRIGGER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "PassByTargetTriggerModifier"+QString::number(refCount);
}

QString BSPassByTargetTriggerModifier::getClassname(){
    return classname;
}

QString BSPassByTargetTriggerModifier::getName() const{
    return name;
}

bool BSPassByTargetTriggerModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetPosition"){
            targetPosition = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'targetPosition' data field!\nObject Reference: "+ref);
            }
        }else if (text == "radius"){
            radius = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'radius' data field!\nObject Reference: "+ref);
            }
        }else if (text == "movementDirection"){
            movementDirection = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'movementDirection' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            triggerEvent.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!triggerEvent.payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSPassByTargetTriggerModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetPosition"), targetPosition.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("radius"), QString::number(radius, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("movementDirection"), movementDirection.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("triggerEvent"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(triggerEvent.id));
        if (triggerEvent.payload.data()){
            refString = triggerEvent.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (triggerEvent.payload.data() && !triggerEvent.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
    }
    return true;
}

bool BSPassByTargetTriggerModifier::isEventReferenced(int eventindex) const{
    if (triggerEvent.id == eventindex){
        return true;
    }
    return false;
}

void BSPassByTargetTriggerModifier::updateEventIndices(int eventindex){
    if (triggerEvent.id > eventindex){
        triggerEvent.id--;
    }
}

void BSPassByTargetTriggerModifier::mergeEventIndex(int oldindex, int newindex){
    if (triggerEvent.id == oldindex){
        triggerEvent.id = newindex;
    }
}

void BSPassByTargetTriggerModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            auto fixIndex = [&](int & id){ if (id < 0){return;}
                thiseventname = recdata->getEventNameAt(id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                id = eventindex;
            };
            fixIndex(triggerEvent.id);
            setIsMerged(true);
        }
    }
}

void BSPassByTargetTriggerModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (triggerEvent.payload.data()){
        ref++;
        triggerEvent.payload.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> BSPassByTargetTriggerModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (triggerEvent.payload.data()){
        list.append(triggerEvent.payload.data());
    }
    return list;
}

bool BSPassByTargetTriggerModifier::merge(HkxObject *recessiveObject){
    BSPassByTargetTriggerModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == BS_PASS_BY_TARGET_TRIGGER_MODIFIER){
        recobj = static_cast<BSPassByTargetTriggerModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!triggerEvent.payload.data() && recobj->triggerEvent.payload.data()){
            getParentFile()->addObjectToFile(recobj->triggerEvent.payload.data(), -1);
        }
        return true;
    }else{
        return false;
    }
}

bool BSPassByTargetTriggerModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(triggerEvent.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        triggerEvent.payload = *ptr;
    }
    return true;
}

void BSPassByTargetTriggerModifier::unlink(){
    HkDynamicObject::unlink();
    triggerEvent.payload = HkxSharedPtr();
}

QString BSPassByTargetTriggerModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (triggerEvent.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": triggerEvent event id out of range!\n");
    }
    if (triggerEvent.payload.data() && triggerEvent.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid triggerEvent.payload type! Signature: "+QString::number(triggerEvent.payload.data()->getSignature(), 16)+"\n");
    }
    setDataValidity(isvalid);
    return errors;
}

BSPassByTargetTriggerModifier::~BSPassByTargetTriggerModifier(){
    refCount--;
}
