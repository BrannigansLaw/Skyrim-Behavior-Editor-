#include "bseventondeactivatemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSEventOnDeactivateModifier
*/

uint BSEventOnDeactivateModifier::refCount = 0;

QString BSEventOnDeactivateModifier::classname = "BSEventOnDeactivateModifier";

BSEventOnDeactivateModifier::BSEventOnDeactivateModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(BS_EVENT_ON_DEACTIVATE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EventOnDeactivateModifier"+QString::number(refCount);
}

QString BSEventOnDeactivateModifier::getClassname(){
    return classname;
}

QString BSEventOnDeactivateModifier::getName() const{
    return name;
}

bool BSEventOnDeactivateModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "id"){
            event.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!event.payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSEventOnDeactivateModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(event.id));
        if (event.payload.data()){
            refString = event.payload.data()->getReferenceString();
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
        if (event.payload.data() && !event.payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
    }
    return true;
}

bool BSEventOnDeactivateModifier::isEventReferenced(int eventindex) const{
    if (event.id == eventindex){
        return true;
    }
    return false;
}

void BSEventOnDeactivateModifier::updateEventIndices(int eventindex){
    if (event.id > eventindex){
        event.id--;
    }
}

void BSEventOnDeactivateModifier::mergeEventIndex(int oldindex, int newindex){
    if (event.id == oldindex){
        event.id = newindex;
    }
}

void BSEventOnDeactivateModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
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
            fixIndex(event.id);
            setIsMerged(true);
        }
    }
}

void BSEventOnDeactivateModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (event.payload.data()){
        ref++;
        event.payload.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> BSEventOnDeactivateModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (event.payload.data()){
        list.append(event.payload.data());
    }
    return list;
}

bool BSEventOnDeactivateModifier::merge(HkxObject *recessiveObject){
    BSEventOnDeactivateModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == BS_EVENT_ON_DEACTIVATE_MODIFIER){
        recobj = static_cast<BSEventOnDeactivateModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        if (!event.payload.data() && recobj->event.payload.data()){
            getParentFile()->addObjectToFile(recobj->event.payload.data(), -1);
        }
        return true;
    }else{
        return false;
    }
}

bool BSEventOnDeactivateModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(event.payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        event.payload = *ptr;
    }
    return true;
}

void BSEventOnDeactivateModifier::unlink(){
    HkDynamicObject::unlink();
    event.payload = HkxSharedPtr();
}

QString BSEventOnDeactivateModifier::evaluateDataValidity(){
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
    if (event.id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": event event id out of range! Setting to max index in range!\n");
        event.id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (event.payload.data() && event.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid event.payload type! Signature: "+QString::number(event.payload.data()->getSignature(), 16)+" Setting null value!\n");
        event.payload = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

BSEventOnDeactivateModifier::~BSEventOnDeactivateModifier(){
    refCount--;
}
