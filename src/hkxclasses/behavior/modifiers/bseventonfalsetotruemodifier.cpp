#include "bseventonfalsetotruemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

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
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bEnableEvent1"){
            bEnableEvent1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bEnableEvent1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest1"){
            bVariableToTest1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bVariableToTest1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend1"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend1.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend1.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "bEnableEvent2"){
            bEnableEvent2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bEnableEvent2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest2"){
            bVariableToTest2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bVariableToTest2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend2"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend2.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend2.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "bEnableEvent3"){
            bEnableEvent3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bEnableEvent3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bVariableToTest3"){
            bVariableToTest3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bVariableToTest3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "EventToSend3"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    eventToSend3.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!eventToSend3.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
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
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
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

bool BSEventOnFalseToTrueModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend1.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend1.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend2.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        eventToSend2.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(eventToSend3.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
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

bool BSEventOnFalseToTrueModifier::evaulateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (eventToSend1.payload.data() && eventToSend1.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (eventToSend2.payload.data() && eventToSend2.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (eventToSend3.payload.data() && eventToSend3.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSEventOnFalseToTrueModifier::~BSEventOnFalseToTrueModifier(){
    refCount--;
}
