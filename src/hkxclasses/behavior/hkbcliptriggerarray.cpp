#include "hkbcliptriggerarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
/**
 * hkbClipTriggerArray
 */

uint hkbClipTriggerArray::refCount = 0;

QString hkbClipTriggerArray::classname = "hkbClipTriggerArray";

hkbClipTriggerArray::hkbClipTriggerArray(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_CLIP_TRIGGER_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbClipTriggerArray::getClassname(){
    return classname;
}

void hkbClipTriggerArray::addTrigger(const HkTrigger & trigger){
    triggers.append(trigger);
}

void hkbClipTriggerArray::setTriggerId(int index, int id){
    if (triggers.size() > index){
        triggers[index].event.id = id;
    }
}

void hkbClipTriggerArray::setLocalTime(int index, qreal time){
    if (triggers.size() > index){
        triggers[index].localTime = time;
    }
}

void hkbClipTriggerArray::removeTrigger(int index){
    if (triggers.size() > index){
        triggers.removeAt(index);
    }
}

int hkbClipTriggerArray::getLastTriggerIndex() const{
    return triggers.size() - 1;
}

bool hkbClipTriggerArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "triggers"){
            int numtriggers = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtriggers; j++){
                triggers.append(HkTrigger());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "localTime"){
                        triggers.last().localTime = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "id"){
                        triggers.last().event.id = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "payload"){
                        if (!triggers.last().event.payload.readShdPtrReference(index, reader)){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "relativeToEndOfClip"){
                        triggers.last().relativeToEndOfClip = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'relativeToEndOfClip' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "acyclic"){
                        triggers.last().acyclic = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'acyclic' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "isAnnotation"){
                        triggers.last().isAnnotation = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'isAnnotation' data field!\nObject Reference: "+ref);
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

bool hkbClipTriggerArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"triggers", QString::number(triggers.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < triggers.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localTime"), QString::number(triggers.at(i).localTime, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(triggers.at(i).event.id));
            if (triggers.at(i).event.payload.data()){
                refString = triggers.at(i).event.payload.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("relativeToEndOfClip"), getBoolAsString(triggers.at(i).relativeToEndOfClip));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("acyclic"), getBoolAsString(triggers.at(i).acyclic));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isAnnotation"), getBoolAsString(triggers.at(i).isAnnotation));
            writer->writeLine(writer->object, false);
        }
        if (triggers.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < triggers.size(); i++){
            if (triggers.at(i).event.payload.data() && !triggers.at(i).event.payload.data()->write(writer)){
                WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!");
            }
        }
    }
    return true;
}

bool hkbClipTriggerArray::isEventReferenced(int eventindex) const{
    for (auto i = 0; i < triggers.size(); i++){
        if (triggers.at(i).event.id == eventindex){
            return true;
        }
    }
    return false;
}

void hkbClipTriggerArray::updateEventIndices(int eventindex){
    for (auto i = 0; i < triggers.size(); i++){
        if (triggers.at(i).event.id > eventindex){
            triggers[i].event.id--;
        }
    }
}

void hkbClipTriggerArray::mergeEventIndex(int oldindex, int newindex){
    for (auto i = 0; i < triggers.size(); i++){
        if (triggers.at(i).event.id == oldindex){
            triggers[i].event.id = newindex;
        }
    }
}

void hkbClipTriggerArray::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            for (auto i = 0; i < triggers.size(); i++){
                thiseventname = recdata->getEventNameAt(triggers.at(i).event.id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                triggers[i].event.id = eventindex;
            }
        }
        setIsMerged(true);
    }
}

bool hkbClipTriggerArray::merge(HkxObject *recessiveObject){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString othereventname;
    int eventindex;
    bool found;
    hkbClipTriggerArray *obj = nullptr;
    if (recessiveObject && recessiveObject->getSignature() == HKB_CLIP_TRIGGER_ARRAY){
        obj = static_cast<hkbClipTriggerArray *>(recessiveObject);
        //obj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(obj->getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        for (auto i = 0; i < obj->triggers.size(); i++){
            found = false;
            for (auto j = 0; j < triggers.size(); j++){
                if (triggers.at(j).event.id == obj->triggers.at(i).event.id){
                    found = true;
                }
            }
            if (!found){
                othereventname = recdata->getEventNameAt(obj->triggers.at(i).event.id);
                eventindex = domdata->getIndexOfEvent(othereventname);
                if (eventindex == -1 && othereventname != ""){
                    domdata->addEvent(othereventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                //obj->triggers[i].event.id = eventindex;
                triggers.append(obj->triggers.at(i));
            }
        }
        return true;
    }else{
        return false;
    }
}

void hkbClipTriggerArray::updateReferences(long &ref){
    setReference(ref);
    for (auto i = 0; i < triggers.size(); i++){
        if (triggers.at(i).event.payload.data()){
            ref++;
            triggers[i].event.payload.data()->updateReferences(ref);
        }
    }
}

bool hkbClipTriggerArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < triggers.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(triggers.at(i).event.payload.getShdPtrReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            triggers[i].event.payload = *ptr;
        }
    }
    return true;
}

bool hkbClipTriggerArray::evaluateDataValidity(){
    for (int i = 0; i < triggers.size(); i++){
        if (triggers.at(i).event.payload.data() && triggers.at(i).event.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbClipTriggerArray::~hkbClipTriggerArray(){
    refCount--;
}
