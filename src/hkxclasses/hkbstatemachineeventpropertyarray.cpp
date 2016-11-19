#include "hkbstatemachineeventpropertyarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/**
 * hkbStateMachineEventPropertyArray
 */

uint hkbStateMachineEventPropertyArray::refCount = 0;

QString hkbStateMachineEventPropertyArray::classname = "hkbStateMachineEventPropertyArray";

hkbStateMachineEventPropertyArray::hkbStateMachineEventPropertyArray(BehaviorFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbStateMachineEventPropertyArray::getClassname(){
    return classname;
}

void hkbStateMachineEventPropertyArray::addEvent(const HkEvent & event){
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
                events.append(HkEvent());
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

bool hkbStateMachineEventPropertyArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < events.size(); i++){
        //payload
        ptr = getParentFile()->findHkxObject(events.at(i).payload.getReference());
        if (ptr){
            if ((*ptr)->getType() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            events[i].payload = *ptr;
        }
    }
    return true;
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
