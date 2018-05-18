#include "hkbkeyframebonesmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbKeyframeBonesModifier
*/

uint hkbKeyframeBonesModifier::refCount = 0;

QString hkbKeyframeBonesModifier::classname = "hkbKeyframeBonesModifier";

hkbKeyframeBonesModifier::hkbKeyframeBonesModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_KEY_FRAME_BONES_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "KeyframeBonesModifier"+QString::number(refCount);
}

QString hkbKeyframeBonesModifier::getClassname(){
    return classname;
}

QString hkbKeyframeBonesModifier::getName() const{
    return name;
}

bool hkbKeyframeBonesModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "keyframeInfo"){
            int numexp = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numexp; j++){
                keyframeInfo.append(hkKeyframeInfo());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    text = reader.getNthAttributeValueAt(index, 0);
                    if (text == "keyframedPosition"){
                        keyframeInfo.last().keyframedPosition = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'keyframedPosition' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "keyframedRotation"){
                        keyframeInfo.last().keyframedRotation = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'keyframedRotation' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "boneIndex"){
                        keyframeInfo.last().boneIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "isValid"){
                        keyframeInfo.last().isValid = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'isValid' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "keyframedBonesList"){
            if (!keyframedBonesList.readShdPtrReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'keyframedBonesList' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbKeyframeBonesModifier::write(HkxXMLWriter *writer){
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
        list1 = {writer->name, writer->numelements};
        list2 = {"keyframeInfo", QString::number(keyframeInfo.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < keyframeInfo.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keyframedPosition"), keyframeInfo[i].keyframedPosition.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keyframedRotation"), keyframeInfo[i].keyframedRotation.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneIndex"), QString::number(keyframeInfo.at(i).boneIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isValid"), getBoolAsString(keyframeInfo.at(i).isValid));
            writer->writeLine(writer->object, false);
        }
        if (keyframeInfo.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        if (keyframedBonesList.data()){
            refString = keyframedBonesList.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keyframedBonesList"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (keyframedBonesList.data() && !keyframedBonesList.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'keyframedBonesList'!!!", true);
        }
    }
    return true;
}

int hkbKeyframeBonesModifier::getNumberOfKeyframeInfos() const{
    return keyframeInfo.size();
}

bool hkbKeyframeBonesModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(keyframedBonesList.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'keyframedBonesList' is not a HKB_BONE_INDEX_ARRAY!");
            setDataValidity(false);
        }
        keyframedBonesList = *ptr;
    }
    return true;
}

void hkbKeyframeBonesModifier::unlink(){
    HkDynamicObject::unlink();
    keyframedBonesList = HkxSharedPtr();
}

bool hkbKeyframeBonesModifier::evaluateDataValidity(){    //Check if bone id is valid???
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!keyframedBonesList.data()){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbKeyframeBonesModifier::~hkbKeyframeBonesModifier(){
    refCount--;
}
