#include "hkbpoweredragdollcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbPoweredRagdollControlsModifier::refCount = 0;

const QString hkbPoweredRagdollControlsModifier::classname = "hkbPoweredRagdollControlsModifier";

const QStringList hkbPoweredRagdollControlsModifier::Mode = {"WORLD_FROM_MODEL_MODE_USE_OLD", "WORLD_FROM_MODEL_MODE_USE_INPUT", "WORLD_FROM_MODEL_MODE_COMPUTE", "WORLD_FROM_MODEL_MODE_NONE", "WORLD_FROM_MODEL_MODE_RAGDOLL"};

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
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "PoweredRagdollControlsModifier_"+QString::number(refCount);
}

const QString hkbPoweredRagdollControlsModifier::getClassname(){
    return classname;
}

QString hkbPoweredRagdollControlsModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbPoweredRagdollControlsModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    auto ref = reader.getNthAttributeValueAt(index - 1, 0);
    auto checkvalue = [&](bool value, const QString & fieldname){
        (!value) ? LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\n'"+fieldname+"' has invalid data!\nObject Reference: "+ref) : NULL;
    };
    for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            checkvalue(getVariableBindingSet().readShdPtrReference(index, reader), "variableBindingSet");
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            checkvalue(ok, "userData");
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            checkvalue((name != ""), "name");
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enable");
        }else if (text == "maxForce"){
            maxForce = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "tau"){
            tau = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "damping"){
            damping = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "proportionalRecoveryVelocity"){
            proportionalRecoveryVelocity = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "constantRecoveryVelocity"){
            constantRecoveryVelocity = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "poseMatchingBone0"){
            poseMatchingBone0 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "enable");
        }else if (text == "poseMatchingBone1"){
            poseMatchingBone1 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "enable");
        }else if (text == "poseMatchingBone2"){
            poseMatchingBone2 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "enable");
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            checkvalue(Mode.contains(mode), "mode");
        }else if (text == "boneWeights"){
            checkvalue(boneWeights.readShdPtrReference(index, reader), "boneWeights");
        }
    }
    index--;
    return true;
}

bool hkbPoweredRagdollControlsModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    auto writeref = [&](const HkxSharedPtr & shdptr, const QString & name){
        QString refString = "null";
        (shdptr.data()) ? refString = shdptr->getReferenceString() : NULL;
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), refString);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!");
    };
    if (writer && !getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writeref(getVariableBindingSet(), "variableBindingSet");
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writedatafield("controlData", "");
        writer->writeLine(writer->object, true);
        writedatafield("maxForce", QString::number(maxForce, char('f'), 6));
        writedatafield("tau", QString::number(tau, char('f'), 6));
        writedatafield("damping", QString::number(damping, char('f'), 6));
        writedatafield("proportionalRecoveryVelocity", QString::number(proportionalRecoveryVelocity, char('f'), 6));
        writedatafield("constantRecoveryVelocity", QString::number(constantRecoveryVelocity, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writeref(bones, "bones");
        writedatafield("worldFromModelModeData", "");
        writer->writeLine(writer->object, true);
        writedatafield("poseMatchingBone0", QString::number(poseMatchingBone0));
        writedatafield("poseMatchingBone1", QString::number(poseMatchingBone1));
        writedatafield("poseMatchingBone2", QString::number(poseMatchingBone2));
        writedatafield("mode", mode);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writeref(boneWeights, "boneWeights");
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        writechild(bones, "bones");
        writechild(boneWeights, "boneWeights");
    }
    return true;
}

void hkbPoweredRagdollControlsModifier::updateReferences(long &ref){
    std::lock_guard <std::mutex> guard(mutex);
    auto updateref = [&](const HkxSharedPtr & shdptr){
        (shdptr.data()) ? shdptr->updateReferences(++ref) : NULL;
    };
    setReference(ref);
    setBindingReference(++ref);
    updateref(bones);
    updateref(boneWeights);
}

QVector<HkxObject *> hkbPoweredRagdollControlsModifier::getChildrenOtherTypes() const{
    std::lock_guard <std::mutex> guard(mutex);
    QVector<HkxObject *> list;
    auto getchildren = [&](const HkxSharedPtr & shdptr){
        if (shdptr.data()){
            list.append(shdptr.data());
        }
    };
    getchildren(bones);
    getchildren(boneWeights);
    return list;
}

bool hkbPoweredRagdollControlsModifier::merge(HkxObject *recessiveObject){ //TO DO: Make thread safe!!!
    std::lock_guard <std::mutex> guard(mutex);
    auto merge = [&](HkxSharedPtr & shdptr, const HkxSharedPtr & recshdptr){
        if (shdptr.data()){
            if (recshdptr.data()){
                shdptr->merge(recshdptr.data());
            }
        }else if (recshdptr.data()){
            shdptr = recshdptr;
            getParentFile()->addObjectToFile(recshdptr.data(), -1);
        }
    };
    hkbPoweredRagdollControlsModifier *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER){
        obj = static_cast<hkbPoweredRagdollControlsModifier *>(recessiveObject);
        merge(bones, obj->getBones());
        merge(boneWeights, obj->getBoneWeights());
        injectWhileMerging(obj);
        return true;
    }
    return false;
}

HkxSharedPtr hkbPoweredRagdollControlsModifier::getBoneWeights() const{
    std::lock_guard <std::mutex> guard(mutex);
    return boneWeights;
}

HkxSharedPtr hkbPoweredRagdollControlsModifier::getBones() const{
    std::lock_guard <std::mutex> guard(mutex);
    return bones;
}

bool hkbPoweredRagdollControlsModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    HkxSharedPtr *ptr;
    auto link = [&](HkxSharedPtr & data, const QString & fieldname, HkxSignature sig){
        if (ptr){
            if ((*ptr)->getSignature() != sig){
                LogFile::writeToLog(getParentFilename()+": "+getClassname()+": linkVar()!\nThe linked object '"+fieldname+"' is not the correct type!");
                setDataValidity(false);
            }
            data = *ptr;
        }
    };
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(bones.getShdPtrReference());
    link(bones, "bones", HKB_BONE_INDEX_ARRAY);
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(boneWeights.getShdPtrReference());
    link(boneWeights, "boneWeights", HKB_BONE_WEIGHT_ARRAY);
    return true;
}

void hkbPoweredRagdollControlsModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
    bones = HkxSharedPtr();
    boneWeights = HkxSharedPtr();
}

QString hkbPoweredRagdollControlsModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto checkbones = [&](int & boneindex, const QString & fieldname){
        if (boneindex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
            isvalid = false;
            errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": "+fieldname+" bone id out of range! Setting to max index in range!");
            boneindex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones(true) - 1;
        }
    };
    auto checktype = [&](HkxSharedPtr & shdptr, const QString & fieldname, HkxSignature sig){
        if (!shdptr.data()){
            isvalid = false;
            errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Null "+fieldname+"!");
        }else if (shdptr->getSignature() != sig){
            isvalid = false;
            errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid "+fieldname+" type! Signature: "+QString::number(shdptr->getSignature(), 16)+" Setting null value!");
            shdptr = HkxSharedPtr();
        }
    };
    auto temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!");
    }
    checkbones(poseMatchingBone0, "poseMatchingBone0");
    checkbones(poseMatchingBone1, "poseMatchingBone1");
    checkbones(poseMatchingBone2, "poseMatchingBone2");
    if (!Mode.contains(mode)){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid mode! Setting default value!");
        mode = Mode.first();
    }
    checktype(bones, "bones", HKB_BONE_INDEX_ARRAY);
    checktype(boneWeights, "boneWeights", HKB_BONE_WEIGHT_ARRAY);
    setDataValidity(isvalid);
    return errors;
}

hkbPoweredRagdollControlsModifier::~hkbPoweredRagdollControlsModifier(){
    refCount--;
}
