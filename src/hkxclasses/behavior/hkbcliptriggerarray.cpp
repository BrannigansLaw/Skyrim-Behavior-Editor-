#include "hkbcliptriggerarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
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
        triggers[index].id = id;
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
                        triggers.last().id = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "payload"){
                        if (!triggers.last().payload.readReference(index, reader)){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "relativeToEndOfClip"){
                        triggers.last().relativeToEndOfClip = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'relativeToEndOfClip' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "acyclic"){
                        triggers.last().acyclic = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'acyclic' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "isAnnotation"){
                        triggers.last().isAnnotation = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'isAnnotation' data field!\nObject Reference: "+ref);
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
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localTime"), QString::number(triggers.at(i).localTime));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(triggers.at(i).id));
            if (triggers.at(i).payload.data()){
                refString = triggers.at(i).payload.data()->getReferenceString();
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
            if (triggers.at(i).payload.data() && !triggers.at(i).payload.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbClipTriggerArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < triggers.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(triggers.at(i).payload.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            triggers[i].payload = *ptr;
        }
    }
    return true;
}

bool hkbClipTriggerArray::evaulateDataValidity(){
    for (int i = 0; i < triggers.size(); i++){
        if (triggers.at(i).payload.data() && triggers.at(i).payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
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
