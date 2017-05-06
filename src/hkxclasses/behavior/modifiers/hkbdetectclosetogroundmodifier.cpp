#include "hkbdetectclosetogroundmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbDetectCloseToGroundModifier
*/

uint hkbDetectCloseToGroundModifier::refCount = 0;

QString hkbDetectCloseToGroundModifier::classname = "hkbDetectCloseToGroundModifier";

hkbDetectCloseToGroundModifier::hkbDetectCloseToGroundModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      closeToGroundHeight(0),
      raycastDistanceDown(0),
      collisionFilterInfo(0),
      boneIndex(-1),
      animBoneIndex(-1)
{
    setType(HKB_EVENT_DRIVEN_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbDetectCloseToGroundModifier"+QString::number(refCount);
}

QString hkbDetectCloseToGroundModifier::getClassname(){
    return classname;
}

QString hkbDetectCloseToGroundModifier::getName() const{
    return name;
}

bool hkbDetectCloseToGroundModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "id"){
            closeToGroundEvent.id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!closeToGroundEvent.payload.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }else if (text == "closeToGroundHeight"){
            closeToGroundHeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'closeToGroundHeight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "raycastDistanceDown"){
            raycastDistanceDown = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'raycastDistanceDown' data field!\nObject Reference: "+ref);
            }
        }else if (text == "collisionFilterInfo"){
            collisionFilterInfo = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'collisionFilterInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "boneIndex"){
            boneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animBoneIndex"){
            animBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'animBoneIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbDetectCloseToGroundModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("closeToGroundEvent"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(closeToGroundEvent.id));
        if (closeToGroundEvent.payload.data()){
            refString = closeToGroundEvent.payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("closeToGroundHeight"), QString::number(closeToGroundHeight));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("raycastDistanceDown"), QString::number(raycastDistanceDown));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("collisionFilterInfo"), QString::number(collisionFilterInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneIndex"), QString::number(boneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("animBoneIndex"), QString::number(animBoneIndex));
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (closeToGroundEvent.payload.data() && !closeToGroundEvent.payload.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload'!!!", true);
        }
    }
    return true;
}

bool hkbDetectCloseToGroundModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(closeToGroundEvent.payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        closeToGroundEvent.payload = *ptr;
    }
    return true;
}

void hkbDetectCloseToGroundModifier::unlink(){
    HkDynamicObject::unlink();
    closeToGroundEvent.payload = HkxSharedPtr();
}

bool hkbDetectCloseToGroundModifier::evaulateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (closeToGroundEvent.payload.data() && closeToGroundEvent.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbDetectCloseToGroundModifier::~hkbDetectCloseToGroundModifier(){
    refCount--;
}
