#include "hkbpoweredragdollcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbPoweredRagdollControlsModifier
*/

uint hkbPoweredRagdollControlsModifier::refCount = 0;

QString hkbPoweredRagdollControlsModifier::classname = "hkbPoweredRagdollControlsModifier";

QStringList hkbPoweredRagdollControlsModifier::Mode = {"WORLD_FROM_MODEL_MODE_USE_OLD", "WORLD_FROM_MODEL_MODE_USE_INPUT", "WORLD_FROM_MODEL_MODE_COMPUTE", "WORLD_FROM_MODEL_MODE_NONE", "WORLD_FROM_MODEL_MODE_RAGDOLL"};

hkbPoweredRagdollControlsModifier::hkbPoweredRagdollControlsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      maxForce(0),
      tau(0),
      damping(0),
      proportionalRecoveryVelocity(0),
      constantRecoveryVelocity(0),
      poseMatchingBone0(-1),
      poseMatchingBone1(-1),
      poseMatchingBone2(-1),
      mode(Mode.first())
{
    setType(HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "PoweredRagdollControlsModifier"+QString::number(refCount);
}

QString hkbPoweredRagdollControlsModifier::getClassname(){
    return classname;
}

QString hkbPoweredRagdollControlsModifier::getName() const{
    return name;
}

bool hkbPoweredRagdollControlsModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "maxForce"){
            maxForce = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'maxForce' data field!\nObject Reference: "+ref);
            }
        }else if (text == "tau"){
            tau = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'tau' data field!\nObject Reference: "+ref);
            }
        }else if (text == "damping"){
            damping = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'damping' data field!\nObject Reference: "+ref);
            }
        }else if (text == "proportionalRecoveryVelocity"){
            proportionalRecoveryVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'proportionalRecoveryVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "constantRecoveryVelocity"){
            constantRecoveryVelocity = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'constantRecoveryVelocity' data field!\nObject Reference: "+ref);
            }
        }else if (text == "poseMatchingBone0"){
            poseMatchingBone0 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone0' data field!\nObject Reference: "+ref);
            }
        }else if (text == "poseMatchingBone1"){
            poseMatchingBone1 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "poseMatchingBone2"){
            poseMatchingBone2 = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'poseMatchingBone2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (!Mode.contains(mode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nInvalid 'mode' data!\nObject Reference: "+ref);
            }
        }else if (text == "boneWeights"){
            if (!boneWeights.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'boneWeights' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbPoweredRagdollControlsModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("controlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxForce"), QString::number(maxForce, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("tau"), QString::number(tau, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("damping"), QString::number(damping, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("proportionalRecoveryVelocity"), QString::number(proportionalRecoveryVelocity, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("constantRecoveryVelocity"), QString::number(constantRecoveryVelocity, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (bones.data()){
            refString = bones.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bones"), refString);

        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldFromModelModeData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone0"), QString::number(poseMatchingBone0));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone1"), QString::number(poseMatchingBone1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("poseMatchingBone2"), QString::number(poseMatchingBone2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("mode"), mode);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        if (boneWeights.data()){
            refString = boneWeights.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneWeights"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (bones.data() && !bones.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'bones'!!!");
        }
        if (boneWeights.data() && !boneWeights.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'boneWeights'!!!");
        }
    }
    return true;
}

void hkbPoweredRagdollControlsModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (bones.data()){
        ref++;
        bones.data()->updateReferences(ref);
    }
    if (boneWeights.data()){
        ref++;
        boneWeights.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> hkbPoweredRagdollControlsModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (bones.data()){
        list.append(bones.data());
    }
    if (boneWeights.data()){
        list.append(boneWeights.data());
    }
    return list;
}

bool hkbPoweredRagdollControlsModifier::merge(HkxObject *recessiveObject){
    hkbPoweredRagdollControlsModifier *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER){
        obj = static_cast<hkbPoweredRagdollControlsModifier *>(recessiveObject);
        if (bones.data()){
            if (obj->bones.data()){
                bones.data()->merge(obj->bones.data());
            }
        }else if (obj->bones.data()){
            bones = obj->bones;
            getParentFile()->addObjectToFile(obj->bones.data(), -1);
        }
        if (boneWeights.data()){
            if (obj->boneWeights.data()){
                boneWeights.data()->merge(obj->bones.data());
            }
        }else if (obj->boneWeights.data()){
            boneWeights = obj->boneWeights;
            getParentFile()->addObjectToFile(obj->boneWeights.data(), -1);
        }
        injectWhileMerging(obj);
        return true;
    }else{
        return false;
    }
}

bool hkbPoweredRagdollControlsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(bones.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_INDEX_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'bones' is not a HKB_BONE_INDEX_ARRAY!");
        }
        bones = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(boneWeights.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": linkVar()!\nThe linked object 'boneWeights' is not a HKB_BONE_WEIGHT_ARRAY!");
        }
        boneWeights = *ptr;
    }
    return true;
}

void hkbPoweredRagdollControlsModifier::unlink(){
    HkDynamicObject::unlink();
    bones = HkxSharedPtr();
    boneWeights = HkxSharedPtr();
}

QString hkbPoweredRagdollControlsModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (poseMatchingBone0 >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": poseMatchingBone0 out of range!\n");
    }
    if (poseMatchingBone1 >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": poseMatchingBone1 out of range!\n");
    }
    if (poseMatchingBone2 >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": poseMatchingBone2 out of range!\n");
    }
    if (!Mode.contains(mode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid mode!\n");
    }
    if (!bones.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null bones!\n");
    }else if (bones.data()->getSignature() != HKB_BONE_INDEX_ARRAY){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid bones type! Signature: "+QString::number(bones.data()->getSignature(), 16)+"\n");
    }
    if (!boneWeights.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null boneWeights!\n");
    }else if (boneWeights.data()->getSignature() != HKB_BONE_WEIGHT_ARRAY){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid boneWeights type! Signature: "+QString::number(boneWeights.data()->getSignature(), 16)+"\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbPoweredRagdollControlsModifier::~hkbPoweredRagdollControlsModifier(){
    refCount--;
}
