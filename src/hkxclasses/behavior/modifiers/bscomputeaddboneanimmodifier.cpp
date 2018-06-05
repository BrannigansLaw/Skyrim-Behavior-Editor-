#include "bscomputeaddboneanimmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSComputeAddBoneAnimModifier
*/

uint BSComputeAddBoneAnimModifier::refCount = 0;

QString BSComputeAddBoneAnimModifier::classname = "BSComputeAddBoneAnimModifier";

BSComputeAddBoneAnimModifier::BSComputeAddBoneAnimModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      boneIndex(-1)
{
    setType(BS_COMPUTE_ADD_BONE_ANIM_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ComputeAddBoneAnimModifier"+QString::number(refCount);
}

QString BSComputeAddBoneAnimModifier::getClassname(){
    return classname;
}

QString BSComputeAddBoneAnimModifier::getName() const{
    return name;
}

bool BSComputeAddBoneAnimModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "boneIndex"){
            boneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'boneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "translationLSOut"){
            translationLSOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'translationLSOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rotationLSOut"){
            rotationLSOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'rotationLSOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "scaleLSOut"){
            scaleLSOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'scaleLSOut' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSComputeAddBoneAnimModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneIndex"), QString::number(boneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("translationLSOut"), translationLSOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rotationLSOut"), rotationLSOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("scaleLSOut"), scaleLSOut.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSComputeAddBoneAnimModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSComputeAddBoneAnimModifier::unlink(){
    HkDynamicObject::unlink();
}

bool BSComputeAddBoneAnimModifier::evaluateDataValidity(){  //Check bone index???
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSComputeAddBoneAnimModifier::~BSComputeAddBoneAnimModifier(){
    refCount--;
}
