#include "hkbgethandleonbonemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbGetHandleOnBoneModifier
*/

uint hkbGetHandleOnBoneModifier::refCount = 0;

QString hkbGetHandleOnBoneModifier::classname = "hkbGetHandleOnBoneModifier";

hkbGetHandleOnBoneModifier::hkbGetHandleOnBoneModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      ragdollBoneIndex(-1),
      animationBoneIndex(-1)
{
    setType(HKB_GET_HANDLE_ON_BONE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "GetHandleOnBoneModifier"+QString::number(refCount);
}

QString hkbGetHandleOnBoneModifier::getClassname(){
    return classname;
}

QString hkbGetHandleOnBoneModifier::getName() const{
    return name;
}

bool hkbGetHandleOnBoneModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
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
        }else if (text == "handleOut"){
            if (!handleOut.readShdPtrReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'handleOut' reference!\nObject Reference: "+ref);
            }
        }else if (text == "localFrameName"){
            localFrameName = reader.getElementValueAt(index);
            if (localFrameName == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'localFrameName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ragdollBoneIndex"){
            ragdollBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'ragdollBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationBoneIndex"){
            animationBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'animationBoneIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbGetHandleOnBoneModifier::write(HkxXMLWriter *writer){
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
        if (handleOut.data()){
            refString = handleOut.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleOut"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localFrameName"), localFrameName, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ragdollBoneIndex"), QString::number(ragdollBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("animationBoneIndex"), QString::number(animationBoneIndex));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbGetHandleOnBoneModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbGetHandleOnBoneModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbGetHandleOnBoneModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (handleOut.data()){
    }else if (localFrameName == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbGetHandleOnBoneModifier::~hkbGetHandleOnBoneModifier(){
    refCount--;
}
