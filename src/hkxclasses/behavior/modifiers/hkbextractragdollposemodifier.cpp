#include "hkbextractragdollposemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbExtractRagdollPoseModifier
*/

uint hkbExtractRagdollPoseModifier::refCount = 0;

QString hkbExtractRagdollPoseModifier::classname = "hkbExtractRagdollPoseModifier";

hkbExtractRagdollPoseModifier::hkbExtractRagdollPoseModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      poseMatchingBone0(0),
      poseMatchingBone1(0),
      poseMatchingBone2(0),
      enableComputeWorldFromModel(true)
{
    setType(HKB_EXTRACT_RAGDOLL_POSE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ExtractRagdollPoseModifier"+QString::number(refCount);
}

QString hkbExtractRagdollPoseModifier::getClassname(){
    return classname;
}

QString hkbExtractRagdollPoseModifier::getName() const{
    return name;
}

bool hkbExtractRagdollPoseModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "poseMatchingBone0"){
            poseMatchingBone0 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone0' data field!\nObject Reference: "+ref);
            }
        }else if (text == "poseMatchingBone1"){
            poseMatchingBone1 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "poseMatchingBone2"){
            poseMatchingBone2 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enableComputeWorldFromModel"){
            enableComputeWorldFromModel = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enableComputeWorldFromModel' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbExtractRagdollPoseModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone0"), QString::number(poseMatchingBone0));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone1"), QString::number(poseMatchingBone1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone2"), QString::number(poseMatchingBone2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enableComputeWorldFromModel"), getBoolAsString(enableComputeWorldFromModel));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbExtractRagdollPoseModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbExtractRagdollPoseModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbExtractRagdollPoseModifier::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbExtractRagdollPoseModifier::~hkbExtractRagdollPoseModifier(){
    refCount--;
}
