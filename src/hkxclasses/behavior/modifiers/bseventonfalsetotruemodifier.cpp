#include "bseventonfalsetotruemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSEventOnFalseToTrueModifier
*/

uint BSEventOnFalseToTrueModifier::refCount = 0;

QString BSEventOnFalseToTrueModifier::classname = "BSEventOnFalseToTrueModifier";

BSEventOnFalseToTrueModifier::BSEventOnFalseToTrueModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      bEnableEvent1(false),
      bVariableToTest1(false),
      bEnableEvent2(false),
      bVariableToTest2(false),
      bEnableEvent3(false),
      bVariableToTest3(false)
{
    setType(BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EventOnFalseToTrueModifier"+QString::number(refCount);
}

QString BSEventOnFalseToTrueModifier::getClassname(){
    return classname;
}

QString BSEventOnFalseToTrueModifier::getName() const{
    return name;
}

bool BSEventOnFalseToTrueModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "bEnableEvent1"){
            bEnableEvent1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bEnableEvent1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest1"){
            bVariableToTest1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bVariableToTest1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend1"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend1.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend1.payload.readShdPtrReference(index, reader)){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "bEnableEvent2"){
            bEnableEvent2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bEnableEvent2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest2"){
            bVariableToTest2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bVariableToTest2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend2"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend2.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend2.payload.readShdPtrReference(index, reader)){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "bEnableEvent3"){
            bEnableEvent3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bEnableEvent3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest3"){
            bVariableToTest3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'bVariableToTest3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend3"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend3.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend3.payload.readShdPtrReference(index, reader)){
                        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }
        index++;
    }
    return true;
}

bool BSEventOnFalseToTrueModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bEnableEvent1"), getBoolAsString(bEnableEvent1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bVariableToTest1"), getBoolAsString(bVariableToTest1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("EventToSend1"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToSend1.id));
        if (eventToSend1.payload.data()){
            refString = eventToSend1.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bEnableEvent2"), getBoolAsString(bEnableEvent2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bVariableToTest2"), getBoolAsString(bVariableToTest2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("EventToSend2"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToSend2.id));
        if (eventToSend2.payload.data()){
            refString = eventToSend2.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bEnableEvent3"), getBoolAsString(bEnableEvent3));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bVariableToTest3"), getBoolAsString(bVariableToTest3));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("EventToSend3"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(eventToSend3.id));
        if (eventToSend3.payload.data()){
            refString = eventToSend3.payload.data()->getReferenceString();
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
    }
    return true;
}

bool BSEventOnFalseToTrueModifier::isEventReferenced(int eventindex) const{
    if (eventToSend1.id == eventindex || eventToSend2.id == eventindex || eventToSend3.id == eventindex){
        return true;
    }
    return false;
}

void BSEventOnFalseToTrueModifier::updateEventIndices(int eventindex){
    if (eventToSend1.id > eventindex){
        eventToSend1.id--;
    }
    if (eventToSend2.id > eventindex){
        eventToSend2.id--;
    }
    if (eventToSend3.id > eventindex){
        eventToSend3.id--;
    }
}

void BSEventOnFalseToTrueModifier::mergeEventIndex(int oldindex, int newindex){
    if (eventToSend1.id == oldindex){
        eventToSend1.id = newindex;
    }
    if (eventToSend2.id == oldindex){
        eventToSend2.id = newindex;
    }
    if (eventToSend3.id == oldindex){
        eventToSend3.id = newindex;
    }
}

void BSEventOnFalseToTrueModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
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
            fixIndex(eventToSend1.id);
            fixIndex(eventToSend2.id);
            fixIndex(eventToSend3.id);
            setIsMerged(true);
        }
    }
}

void BSEventOnFalseToTrueModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (eventToSend1.payload.data()){
        ref++;
        eventToSend1.payload.data()->updateReferences(ref);
    }
    if (eventToSend2.payload.data()){
        ref++;
        eventToSend2.payload.data()->updateReferences(ref);
    }
    if (eventToSend3.payload.data()){
        ref++;
        eventToSend3.payload.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> BSEventOnFalseToTrueModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (eventToSend1.payload.data()){
        list.append(eventToSend1.payload.data());
    }
    if (eventToSend2.payload.data()){
        list.append(eventToSend2.payload.data());
    }
    if (eventToSend3.payload.data()){
        list.append(eventToSend3.payload.data());
    }
    return list;
}

bool BSEventOnFalseToTrueModifier::merge(HkxObject *recessiveObject){
    BSEventOnFalseToTrueModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER){
        recobj = static_cast<BSEventOnFalseToTrueModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!eventToSend1.payload.data() && recobj->eventToSend1.payload.data()){
            getParentFile()->addObjectToFile(recobj->eventToSend1.payload.data(), -1);
        }
        if (!eventToSend2.payload.data() && recobj->eventToSend2.payload.data()){
            getParentFile()->addObjectToFile(recobj->eventToSend2.payload.data(), -1);
        }
        if (!eventToSend3.payload.data() && recobj->eventToSend3.payload.data()){
            getParentFile()->addObjectToFile(recobj->eventToSend3.payload.data(), -1);
        }
        return true;
    }else{
        return false;
    }
}

bool BSEventOnFalseToTrueModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend1.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend1.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend2.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend2.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend3.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend3.payload = *ptr;
    }
    return true;
}

void BSEventOnFalseToTrueModifier::unlink(){
    HkDynamicObject::unlink();
    eventToSend1.payload = HkxSharedPtr();
    eventToSend2.payload = HkxSharedPtr();
    eventToSend3.payload = HkxSharedPtr();
}

QString BSEventOnFalseToTrueModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (eventToSend1.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventToSend1 event id out of range! Setting to max index in range!\n");
        eventToSend1.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (eventToSend1.payload.data() && eventToSend1.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToSend1.payload type! Signature: "+QString::number(eventToSend1.payload.data()->getSignature(), 16)+" Setting null value!\n");
        eventToSend1.payload = HkxSharedPtr();
    }
    if (eventToSend2.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventToSend2 event id out of range! Setting to max index in range!\n");
        eventToSend2.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (eventToSend2.payload.data() && eventToSend2.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToSend2.payload type! Signature: "+QString::number(eventToSend2.payload.data()->getSignature(), 16)+" Setting null value!\n");
        eventToSend2.payload = HkxSharedPtr();
    }
    if (eventToSend3.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventToSend3 event id out of range! Setting to max index in range!\n");
        eventToSend3.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (eventToSend3.payload.data() && eventToSend3.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventToSend3.payload type! Signature: "+QString::number(eventToSend3.payload.data()->getSignature(), 16)+" Setting null value!\n");
        eventToSend3.payload = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSEventOnFalseToTrueModifier::~BSEventOnFalseToTrueModifier(){
    refCount--;
}
