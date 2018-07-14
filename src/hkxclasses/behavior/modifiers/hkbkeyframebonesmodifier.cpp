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
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
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
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'keyframedPosition' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "keyframedRotation"){
                        keyframeInfo.last().keyframedRotation = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'keyframedRotation' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "boneIndex"){
                        keyframeInfo.last().boneIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'boneIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "isValid"){
                        keyframeInfo.last().isValid = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'isValid' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "keyframedBonesList"){
            if (!keyframedBonesList.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'keyframedBonesList' reference!\nObject Reference: "+ref);
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
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (keyframedBonesList.data() && !keyframedBonesList.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'keyframedBonesList'!!!");
        }
    }
    return true;
}

int hkbKeyframeBonesModifier::getNumberOfKeyframeInfos() const{
    return keyframeInfo.size();
}

void hkbKeyframeBonesModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (keyframedBonesList.data()){
        ref++;
        keyframedBonesList.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbKeyframeBonesModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (keyframedBonesList.data()){
        list.append(keyframedBonesList.data());
    }
    return list;
}

bool hkbKeyframeBonesModifier::merge(HkxObject *recessiveObject){
    hkbKeyframeBonesModifier *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_KEY_FRAME_BONES_MODIFIER){
        obj = static_cast<hkbKeyframeBonesModifier *>(recessiveObject);
        if (keyframedBonesList.data()){
            if (obj->keyframedBonesList.data()){
                keyframedBonesList.data()->merge(obj->keyframedBonesList.data());
            }
        }else if (obj->keyframedBonesList.data()){
            keyframedBonesList = obj->keyframedBonesList;
            getParentFile()->addObjectToFile(obj->keyframedBonesList.data(), 0);
        }
        injectWhileMerging(obj);
        return true;
    }else{
        return false;
    }
}

bool hkbKeyframeBonesModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(keyframedBonesList.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'keyframedBonesList' is not a HKB_BONE_INDEX_ARRAY!");
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

QString hkbKeyframeBonesModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (keyframeInfo.isEmpty()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": keyframeInfo is empty!\n");
    }else{
        for (auto i = 0; i < keyframeInfo.size(); i++){
            if (keyframeInfo.at(i).boneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": boneIndex at "+QString::number(i)+" out of range! Setting to last bone index!\n");
                keyframeInfo[i].boneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
            }
        }
    }
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (keyframedBonesList.data()){
        if (keyframedBonesList.data()->getSignature() != HKB_BONE_INDEX_ARRAY){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid keyframedBonesList type! Signature: "+QString::number(keyframedBonesList.data()->getSignature(), 16)+" Setting null value!\n");
            keyframedBonesList = HkxSharedPtr();    //TO DO: add here?
        }else if (keyframedBonesList.data()->isDataValid() && keyframedBonesList.data()->evaluateDataValidity() != ""){
            isvalid = false;
            //errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid keyframedBonesList data!\n");
        }
    }else if (!keyframedBonesList.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null keyframedBonesList!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbKeyframeBonesModifier::~hkbKeyframeBonesModifier(){
    refCount--;
}
