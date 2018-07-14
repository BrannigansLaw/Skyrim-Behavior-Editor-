#include "hkbeventdrivenmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: hkbEventDrivenModifier
*/

uint hkbEventDrivenModifier::refCount = 0;

QString hkbEventDrivenModifier::classname = "hkbEventDrivenModifier";

hkbEventDrivenModifier::hkbEventDrivenModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(1),
      enable(true),
      activateEventId(-1),
      deactivateEventId(-1)
{
    setType(HKB_EVENT_DRIVEN_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EventDrivenModifier"+QString::number(refCount);
}

QString hkbEventDrivenModifier::getClassname(){
    return classname;
}

QString hkbEventDrivenModifier::getName() const{
    return name;
}

bool hkbEventDrivenModifier::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_MODIFIER){
        modifier = HkxSharedPtr((HkxObject *)obj);
        return true;
    }else{
        return false;
    }
}

bool hkbEventDrivenModifier::removeObjectAt(int index){
    if (index == 0 || index == -1){
        modifier = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbEventDrivenModifier::hasChildren() const{
    if (modifier.data()){
        return true;
    }
    return false;
}

bool hkbEventDrivenModifier::isEventReferenced(int eventindex) const{
    if (activateEventId == eventindex || deactivateEventId == eventindex){
        return true;
    }
    return false;
}

void hkbEventDrivenModifier::updateEventIndices(int eventindex){
    if (activateEventId > eventindex){
        activateEventId--;
    }
    if (deactivateEventId > eventindex){
        deactivateEventId--;
    }
}

void hkbEventDrivenModifier::mergeEventIndex(int oldindex, int newindex){
    if (activateEventId == oldindex){
        activateEventId = newindex;
    }
    if (deactivateEventId == oldindex){
        deactivateEventId = newindex;
    }
}

void hkbEventDrivenModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
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
            fixIndex(activateEventId);
            fixIndex(deactivateEventId);
            setIsMerged(true);
        }
    }
}

QList<DataIconManager *> hkbEventDrivenModifier::getChildren() const{
    QList<DataIconManager *> list;
    if (modifier.data()){
        list.append(static_cast<DataIconManager*>(modifier.data()));
    }
    return list;
}

int hkbEventDrivenModifier::getIndexOfObj(DataIconManager *obj) const{
    if (modifier.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool hkbEventDrivenModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "modifier"){
            if (!modifier.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'modifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "activateEventId"){
            activateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'activateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "deactivateEventId"){
            deactivateEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'deactivateEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "activeByDefault"){
            activeByDefault = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'activeByDefault' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbEventDrivenModifier::write(HkxXMLWriter *writer){
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
        if (modifier.data()){
            refString = modifier.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("modifier"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("activateEventId"), QString::number(activateEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("deactivateEventId"), QString::number(deactivateEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("activeByDefault"), getBoolAsString(activeByDefault));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (modifier.data() && !modifier.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'modifier'!!!");
        }
    }
    return true;
}

bool hkbEventDrivenModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifier.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'modifier' is not a modifier!");
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    return true;
}

void hkbEventDrivenModifier::unlink(){
    HkDynamicObject::unlink();
    modifier = HkxSharedPtr();
}

QString hkbEventDrivenModifier::evaluateDataValidity(){
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
    if (activateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": activateEventId event id out of range! Setting to max index in range!\n");
        activateEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (deactivateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": deactivateEventId event id out of range! Setting to max index in range!\n");
        deactivateEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (!modifier.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null modifier!\n");
    }else if (modifier.data()->getType() != HkxObject::TYPE_MODIFIER){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid modifier type! Signature: "+QString::number(modifier.data()->getSignature(), 16)+" Setting null value!\n");
        modifier = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

hkbEventDrivenModifier::~hkbEventDrivenModifier(){
    refCount--;
}
