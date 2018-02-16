#include "hkbattachmentmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbAttachmentModifier
*/

uint hkbAttachmentModifier::refCount = 0;

QString hkbAttachmentModifier::classname = "hkbAttachmentModifier";

hkbAttachmentModifier::hkbAttachmentModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      attacheeLayer(0)
{
    setType(HKB_ATTACHMENT_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "AttachmentModifier"+QString::number(refCount);
}

QString hkbAttachmentModifier::getClassname(){
    return classname;
}

QString hkbAttachmentModifier::getName() const{
    return name;
}

/*
bool hkbAttachmentModifier::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR && index == 0){
        pDefaultGenerator = HkxObjectExpSharedPtr((HkxObject *)obj);
    }else if (((HkxObject *)obj)->getSignature() == HKB_CLIP_GENERATOR && index == 1){
        pOffsetClipGenerator = HkxObjectExpSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
}

bool hkbAttachmentModifier::removeObjectAt(int index){
    if (index == 0){
        pDefaultGenerator = HkxObjectExpSharedPtr();
    }else if (index == 1){
        pOffsetClipGenerator = HkxObjectExpSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbAttachmentModifier::hasChildren() const{
    if (pDefaultGenerator.data() || pOffsetClipGenerator.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> hkbAttachmentModifier::getChildren() const{
    QList<DataIconManager *> list;
    if (pDefaultGenerator.data()){
        list.append(static_cast<DataIconManager*>(pDefaultGenerator.data()));
    }
    if (pOffsetClipGenerator.data()){
        list.append(static_cast<DataIconManager*>(pOffsetClipGenerator.data()));
    }
    return list;
}

int hkbAttachmentModifier::getIndexOfObj(DataIconManager *obj) const{
    if (pDefaultGenerator.data() == (HkxObject *)obj){
        return 0;
    }else if (pOffsetClipGenerator.data() == (HkxObject *)obj){
        return 1;
    }
    return -1;
}*/

bool hkbAttachmentModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "sendToAttacherOnAttach"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    sendToAttacherOnAttach.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!sendToAttacherOnAttach.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "sendToAttacheeOnAttach"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    sendToAttacheeOnAttach.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!sendToAttacheeOnAttach.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "sendToAttacherOnDetach"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    sendToAttacherOnDetach.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!sendToAttacherOnDetach.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "sendToAttacheeOnDetach"){
            index++;
            while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                text = reader.getNthAttributeValueAt(index, 0);
                if (text == "id"){
                    sendToAttacheeOnDetach.id = reader.getElementValueAt(index).toInt(&ok);
                    if (!ok){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                    }
                }else if (text == "payload"){
                    if (!sendToAttacheeOnDetach.payload.readReference(index, reader)){
                        writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                    }
                    break;
                }
                index++;
            }
        }else if (text == "attachmentSetup"){
            if (!attachmentSetup.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'attachmentSetup' reference!\nObject Reference: "+ref);
            }
        }else if (text == "attacherHandle"){
            if (!attacherHandle.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'attacherHandle' reference!\nObject Reference: "+ref);
            }
        }else if (text == "attacheeHandle"){
            if (!attacheeHandle.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'attacheeHandle' reference!\nObject Reference: "+ref);
            }
        }else if (text == "attacheeLayer"){
            attacheeLayer = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'attacheeLayer' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbAttachmentModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sendToAttacherOnAttach"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(sendToAttacherOnAttach.id));
        if (sendToAttacherOnAttach.payload.data()){
            refString = sendToAttacherOnAttach.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sendToAttacheeOnAttach"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(sendToAttacheeOnAttach.id));
        if (sendToAttacheeOnAttach.payload.data()){
            refString = sendToAttacheeOnAttach.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sendToAttacherOnDetach"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(sendToAttacherOnDetach.id));
        if (sendToAttacherOnDetach.payload.data()){
            refString = sendToAttacherOnDetach.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sendToAttacheeOnDetach"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(sendToAttacheeOnDetach.id));
        if (sendToAttacheeOnDetach.payload.data()){
            refString = sendToAttacheeOnDetach.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (attachmentSetup.data()){
            refString = attachmentSetup.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attachmentSetup"), refString);
        if (attacherHandle.data()){
            refString = attacherHandle.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attacherHandle"), refString);
        if (attacheeHandle.data()){
            refString = attacheeHandle.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attacheeHandle"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attacheeLayer"), QString::number(attacheeLayer));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbAttachmentModifier::isEventReferenced(int eventindex) const{
    if (sendToAttacherOnAttach.id == eventindex || sendToAttacheeOnAttach.id == eventindex || sendToAttacherOnDetach.id == eventindex || sendToAttacheeOnDetach.id == eventindex ){
        return true;
    }
    return false;
}

void hkbAttachmentModifier::updateEventIndices(int eventindex){
    if (sendToAttacherOnAttach.id > eventindex){
        sendToAttacherOnAttach.id--;
    }
    if (sendToAttacheeOnAttach.id > eventindex){
        sendToAttacheeOnAttach.id--;
    }
    if (sendToAttacherOnDetach.id > eventindex){
        sendToAttacherOnDetach.id--;
    }
    if (sendToAttacheeOnDetach.id > eventindex){
        sendToAttacheeOnDetach.id--;
    }
}

bool hkbAttachmentModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(sendToAttacherOnAttach.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        sendToAttacherOnAttach.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(sendToAttacheeOnAttach.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        sendToAttacheeOnAttach.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(sendToAttacherOnDetach.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        sendToAttacherOnDetach.payload = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(sendToAttacheeOnDetach.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        sendToAttacheeOnDetach.payload = *ptr;
    }
    //Link handles???
    return true;
}

void hkbAttachmentModifier::unlink(){
    HkDynamicObject::unlink();
    sendToAttacherOnAttach.payload = HkxSharedPtr();
    sendToAttacheeOnAttach.payload = HkxSharedPtr();
    sendToAttacherOnDetach.payload = HkxSharedPtr();
    sendToAttacheeOnDetach.payload = HkxSharedPtr();
    attachmentSetup = HkxSharedPtr();
    attacherHandle = HkxSharedPtr();
    attacheeHandle = HkxSharedPtr();
}

bool hkbAttachmentModifier::evaulateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (sendToAttacherOnAttach.payload.data() && sendToAttacherOnAttach.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (sendToAttacheeOnAttach.payload.data() && sendToAttacheeOnAttach.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (sendToAttacherOnDetach.payload.data() && sendToAttacherOnDetach.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else if (sendToAttacheeOnDetach.payload.data() && sendToAttacheeOnDetach.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbAttachmentModifier::~hkbAttachmentModifier(){
    refCount--;
}
