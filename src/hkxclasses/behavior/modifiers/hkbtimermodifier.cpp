#include "hkbtimermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: hkbTimerModifier
*/

uint hkbTimerModifier::refCount = 0;

QString hkbTimerModifier::classname = "hkbTimerModifier";

hkbTimerModifier::hkbTimerModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      alarmTimeSeconds(0)
{
    setType(HKB_TIMER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "TimerModifier"+QString::number(refCount);
}

QString hkbTimerModifier::getClassname(){
    return classname;
}

QString hkbTimerModifier::getName() const{
    return name;
}

bool hkbTimerModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "alarmTimeSeconds"){
            alarmTimeSeconds = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'alarmTimeSeconds' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            alarmEvent.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!alarmEvent.payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbTimerModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("alarmTimeSeconds"), QString::number(alarmTimeSeconds, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("alarmEvent"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(alarmEvent.id));
        if (alarmEvent.payload.data()){
            refString = alarmEvent.payload.data()->getReferenceString();
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
        if (alarmEvent.payload.data() && !alarmEvent.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
    }
    return true;
}

bool hkbTimerModifier::isEventReferenced(int eventindex) const{
    if (alarmEvent.id == eventindex){
        return true;
    }
    return false;
}

void hkbTimerModifier::updateEventIndices(int eventindex){
    if (alarmEvent.id > eventindex){
        alarmEvent.id--;
    }
}

void hkbTimerModifier::mergeEventIndex(int oldindex, int newindex){
    if (alarmEvent.id == oldindex){
        alarmEvent.id = newindex;
    }
}

void hkbTimerModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
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
            fixIndex(alarmEvent.id);
            setIsMerged(true);
        }
    }
}

void hkbTimerModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (alarmEvent.payload.data()){
        ref++;
        alarmEvent.payload.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbTimerModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (alarmEvent.payload.data()){
        list.append(alarmEvent.payload.data());
    }
    return list;
}

bool hkbTimerModifier::merge(HkxObject *recessiveObject){
    hkbTimerModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_TIMER_MODIFIER){
        recobj = static_cast<hkbTimerModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!alarmEvent.payload.data() && recobj->alarmEvent.payload.data()){
            getParentFile()->addObjectToFile(recobj->alarmEvent.payload.data(), -1);
        }
        return true;
    }else{
        return false;
    }
}

bool hkbTimerModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(alarmEvent.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        alarmEvent.payload = *ptr;
    }
    return true;
}

void hkbTimerModifier::unlink(){
    HkDynamicObject::unlink();
    alarmEvent.payload = HkxSharedPtr();
}

QString hkbTimerModifier::evaluateDataValidity(){
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
    if (alarmEvent.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": alarmEvent event id out of range! Setting to max index in range!\n");
        alarmEvent.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (alarmEvent.payload.data() && alarmEvent.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid alarmEvent.payload type! Signature: "+QString::number(alarmEvent.payload.data()->getSignature(), 16)+" Setting null value!\n");
        alarmEvent.payload = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

hkbTimerModifier::~hkbTimerModifier(){
    refCount--;
}

