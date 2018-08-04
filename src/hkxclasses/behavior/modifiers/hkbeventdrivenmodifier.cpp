#include "hkbeventdrivenmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

uint hkbEventDrivenModifier::refCount = 0;

const QString hkbEventDrivenModifier::classname = "hkbEventDrivenModifier";

hkbEventDrivenModifier::hkbEventDrivenModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(1),
      enable(true),
      activateEventId(-1),
      deactivateEventId(-1)
{
    setType(HKB_EVENT_DRIVEN_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "EventDrivenModifier_"+QString::number(refCount);
}

const QString hkbEventDrivenModifier::getClassname(){
    return classname;
}

QString hkbEventDrivenModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbEventDrivenModifier::insertObjectAt(int , DataIconManager *obj){
    std::lock_guard <std::mutex> guard(mutex);
    if (obj && obj->getType() == TYPE_MODIFIER){
        modifier = HkxSharedPtr(obj);
        return true;
    }
    return false;
}

bool hkbEventDrivenModifier::removeObjectAt(int index){
    std::lock_guard <std::mutex> guard(mutex);
    if (index == 0 || index == -1){
        modifier = HkxSharedPtr();
        return true;
    }
    return false;
}

bool hkbEventDrivenModifier::hasChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    if (modifier.data()){
        return true;
    }
    return false;
}

bool hkbEventDrivenModifier::isEventReferenced(int eventindex) const{
    std::lock_guard <std::mutex> guard(mutex);
    if (activateEventId == eventindex || deactivateEventId == eventindex){
        return true;
    }
    return false;
}

void hkbEventDrivenModifier::updateEventIndices(int eventindex){
    std::lock_guard <std::mutex> guard(mutex);
    auto updateind = [&](int & index){
        (index > eventindex) ? index-- : NULL;
    };
    updateind(activateEventId);
    updateind(deactivateEventId);
}

void hkbEventDrivenModifier::mergeEventIndex(int oldindex, int newindex){
    std::lock_guard <std::mutex> guard(mutex);
    auto mergeind = [&](int & index){
        (index == oldindex) ? index = newindex : NULL;
    };
    mergeind(activateEventId);
    mergeind(deactivateEventId);
}

void hkbEventDrivenModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    std::lock_guard <std::mutex> guard(mutex);
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

QVector<DataIconManager *> hkbEventDrivenModifier::getChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    QVector<DataIconManager *> list;
    (modifier.data()) ? list.append(static_cast<DataIconManager*>(modifier.data())) : NULL;
    return list;
}

int hkbEventDrivenModifier::getIndexOfObj(DataIconManager *obj) const{
    std::lock_guard <std::mutex> guard(mutex);
    if (modifier.data() == obj){
        return 0;
    }
    return -1;
}

bool hkbEventDrivenModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    auto checkvalue = [&](bool value, const QString & fieldname){
        (!value) ? LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\n'"+fieldname+"' has invalid data!\nObject Reference: "+ref) : NULL;
    };
    for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            checkvalue(getVariableBindingSet().readShdPtrReference(index, reader), "variableBindingSet");
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            checkvalue(ok, "userData");
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            checkvalue((name != ""), "name");
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enable");
        }else if (text == "modifier"){
            checkvalue(modifier.readShdPtrReference(index, reader), "modifier");
        }else if (text == "activateEventId"){
            activateEventId = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "activateEventId");
        }else if (text == "deactivateEventId"){
            deactivateEventId = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "deactivateEventId");
        }else if (text == "activeByDefault"){
            activeByDefault = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "activeByDefault");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbEventDrivenModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    auto writeref = [&](const HkxSharedPtr & shdptr, const QString & name){
        QString refString = "null";
        (shdptr.data()) ? refString = shdptr->getReferenceString() : NULL;
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), refString);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!\n");
    };
    if (writer && !getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writeref(getVariableBindingSet(), "variableBindingSet");
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writeref(modifier, "modifier");
        writedatafield("activateEventId", QString::number(activateEventId));
        writedatafield("deactivateEventId", QString::number(deactivateEventId));
        writedatafield("activeByDefault", getBoolAsString(activeByDefault));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        writechild(modifier, "modifier");
    }
    return true;
}

bool hkbEventDrivenModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifier.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": linkVar()!\nThe linked object 'modifier' is not a modifier!");
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    return true;
}

void hkbEventDrivenModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
    modifier = HkxSharedPtr();
}

QString hkbEventDrivenModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!\n");
    }
    if (activateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": activateEventId event id out of range! Setting to max index in range!\n");
        activateEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (deactivateEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": deactivateEventId event id out of range! Setting to max index in range!\n");
        deactivateEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (!modifier.data()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Null modifier!\n");
    }else if (modifier->getType() != HkxObject::TYPE_MODIFIER){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid modifier type! Signature: "+QString::number(modifier->getSignature(), 16)+" Setting null value!\n");
        modifier = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

hkbEventDrivenModifier::~hkbEventDrivenModifier(){
    refCount--;
}
