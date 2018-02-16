#include "hkbstatemachineeventpropertyarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/**
 * hkbStateMachineEventPropertyArray
 */

uint hkbStateMachineEventPropertyArray::refCount = 0;

QString hkbStateMachineEventPropertyArray::classname = "hkbStateMachineEventPropertyArray";

hkbStateMachineEventPropertyArray::hkbStateMachineEventPropertyArray(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbStateMachineEventPropertyArray::getClassname(){
    return classname;
}

void hkbStateMachineEventPropertyArray::addEvent(const hkEventPayload &event){
    events.append(event);
}

void hkbStateMachineEventPropertyArray::setEventId(int index, int id){
    if (events.size() > index){
        events[index].id = id;
    }
}

void hkbStateMachineEventPropertyArray::removeEvent(int index){
    if (events.size() > index){
        events.removeAt(index);
    }
}

int hkbStateMachineEventPropertyArray::getLastEventIndex() const{
    return events.size() - 1;
}

bool hkbStateMachineEventPropertyArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "events"){
            int numEvents = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numEvents; j++){
                events.append(hkEventPayload());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "id"){
                        events.last().id = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "payload"){
                        if (!events.last().payload.readReference(index, reader)){
                            return false;
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

bool hkbStateMachineEventPropertyArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"events", QString::number(events.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < events.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(events.at(i).id));
            if (events.at(i).payload.data()){
                refString = events.at(i).payload.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
            writer->writeLine(writer->object, false);
        }
        if (events.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < events.size(); i++){
            if (events.at(i).payload.data() && !events.at(i).payload.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbStateMachineEventPropertyArray::isEventReferenced(int eventindex) const{
    for (auto i = 0; i < events.size(); i++){
        if (events.at(i).id == eventindex){
            return true;
        }
    }
    return false;
}

void hkbStateMachineEventPropertyArray::updateEventIndices(int eventindex){
    for (auto i = 0; i < events.size(); i++){
        if (events.at(i).id > eventindex){
            events[i].id--;
        }
    }
}

bool hkbStateMachineEventPropertyArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < events.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(events.at(i).payload.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            events[i].payload = *ptr;
        }
    }
    return true;
}

void hkbStateMachineEventPropertyArray::unlink(){
    for (int i = 0; i < events.size(); i++){
        events[i].payload = HkxSharedPtr();
    }
}

bool hkbStateMachineEventPropertyArray::evaulateDataValidity(){
    for (int i = 0; i < events.size(); i++){
        if (events.at(i).payload.data() && events.at(i).payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbStateMachineEventPropertyArray::~hkbStateMachineEventPropertyArray(){
    refCount--;
}
