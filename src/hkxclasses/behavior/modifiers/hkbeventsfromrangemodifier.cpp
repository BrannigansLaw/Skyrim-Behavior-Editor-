#include "hkbeventsfromrangemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbeventrangedataarray.h"

/*
 * CLASS: hkbEventsFromRangeModifier
*/

uint hkbEventsFromRangeModifier::refCount = 0;

QString hkbEventsFromRangeModifier::classname = "hkbEventsFromRangeModifier";

hkbEventsFromRangeModifier::hkbEventsFromRangeModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      inputValue(0),
      lowerBound(0)
{
    setType(HKB_EVENTS_FROM_RANGE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EventsFromRangeModifier"+QString::number(refCount);
}

QString hkbEventsFromRangeModifier::getClassname(){
    return classname;
}

QString hkbEventsFromRangeModifier::getName() const{
    return name;
}

bool hkbEventsFromRangeModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "inputValue"){
            inputValue = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'inputValue' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lowerBound"){
            lowerBound = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'lowerBound' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eventRanges"){
            if (!eventRanges.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'eventRanges' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbEventsFromRangeModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("inputValue"), QString::number(inputValue, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lowerBound"), QString::number(lowerBound, char('f'), 6));
        if (eventRanges.data()){
            refString = eventRanges.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventRanges"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (eventRanges.data() && !eventRanges.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  write()!\nUnable to write 'eventRanges'!!!");
        }
    }
    return true;
}

int hkbEventsFromRangeModifier::getNumberOfRanges() const{
    if (eventRanges.data()){
        return static_cast<hkbEventRangeDataArray *>(eventRanges.data())->eventData.size();
    }
    return 0;
}

bool hkbEventsFromRangeModifier::isEventReferenced(int eventindex) const{
    if (eventRanges.constData() && eventRanges.constData()->isEventReferenced(eventindex)){
        return true;
    }
    return false;
}

void hkbEventsFromRangeModifier::updateEventIndices(int eventindex){
    if (eventRanges.data()){
        eventRanges.data()->updateEventIndices(eventindex);
    }
}

void hkbEventsFromRangeModifier::mergeEventIndex(int oldindex, int newindex){
    if (eventRanges.data()){
        eventRanges.data()->mergeEventIndex(oldindex, newindex);
    }
}

void hkbEventsFromRangeModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    if (eventRanges.data()){
        eventRanges.data()->fixMergedEventIndices(dominantfile);
    }
}

void hkbEventsFromRangeModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (eventRanges.data()){
        ref++;
        eventRanges.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbEventsFromRangeModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (eventRanges.data()){
        list.append(eventRanges.data());
    }
    return list;
}

bool hkbEventsFromRangeModifier::merge(HkxObject *recessiveObject){
    hkbEventsFromRangeModifier *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_EVENTS_FROM_RANGE_MODIFIER){
        obj = static_cast<hkbEventsFromRangeModifier *>(recessiveObject);
        if (eventRanges.data()){
            if (obj->eventRanges.data()){
                eventRanges.data()->merge(obj->eventRanges.data());
            }
        }else if (obj->eventRanges.data()){
            eventRanges = obj->eventRanges;
            getParentFile()->addObjectToFile(obj->eventRanges.data(), 0);
            obj->eventRanges.data()->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
            QVector <HkxObject *> others = obj->eventRanges.data()->getChildrenOtherTypes();
            for (auto i = 0; i < others.size(); i++){
                getParentFile()->addObjectToFile(others.at(i), -1);
            }
        }
        injectWhileMerging(obj);
        return true;
    }else{
        return false;
    }
}

bool hkbEventsFromRangeModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventRanges.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_EVENT_RANGE_DATA_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'HKB_EVENT_RANGE_DATA_ARRAY' is not a modifier!");
            setDataValidity(false);
        }
        eventRanges = *ptr;
    }
    return true;
}

void hkbEventsFromRangeModifier::unlink(){
    HkDynamicObject::unlink();
    eventRanges = HkxSharedPtr();
}

QString hkbEventsFromRangeModifier::evaluateDataValidity(){
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
    if (eventRanges.data()){
        if (eventRanges.data()->getSignature() != HKB_EVENT_RANGE_DATA_ARRAY){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventRanges type! Signature: "+QString::number(eventRanges.data()->getSignature(), 16)+" Setting default value!\n");
            eventRanges = HkxSharedPtr();
        }else if (static_cast<hkbEventRangeDataArray *>(eventRanges.data())->eventData.size() < 1){
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": eventRanges has no eventData! Setting null value!\n");
            eventRanges = HkxSharedPtr();
        }else if (eventRanges.data()->isDataValid() && eventRanges.data()->evaluateDataValidity() != ""){
            isvalid = false;
            //errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventRanges data!\n");
        }
    }else if (!eventRanges.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null eventRanges! Setting default value!\n");
        eventRanges = HkxSharedPtr();
    }
    setDataValidity(isvalid);
    return errors;
}

hkbEventsFromRangeModifier::~hkbEventsFromRangeModifier(){
    refCount--;
}
