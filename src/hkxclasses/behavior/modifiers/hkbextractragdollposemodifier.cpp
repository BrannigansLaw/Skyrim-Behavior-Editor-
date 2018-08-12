#include "hkbextractragdollposemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbExtractRagdollPoseModifier::refCount = 0;

const QString hkbExtractRagdollPoseModifier::classname = "hkbExtractRagdollPoseModifier";

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
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "ExtractRagdollPoseModifier_"+QString::number(refCount);
}

const QString hkbExtractRagdollPoseModifier::getClassname(){
    return classname;
}

QString hkbExtractRagdollPoseModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbExtractRagdollPoseModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    bool ok;
    QByteArray text;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
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
        }else if (text == "poseMatchingBone0"){
            poseMatchingBone0 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "poseMatchingBone0");
        }else if (text == "poseMatchingBone1"){
            poseMatchingBone1 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "poseMatchingBone1");
        }else if (text == "poseMatchingBone2"){
            poseMatchingBone2 = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "poseMatchingBone2");
        }else if (text == "enableComputeWorldFromModel"){
            enableComputeWorldFromModel = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enableComputeWorldFromModel");
        }
    }
    index--;
    return true;
}

bool hkbExtractRagdollPoseModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    if (writer && !getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (getVariableBindingSetData()){
            refString = getVariableBindingSet()->getReferenceString();
        }
        writedatafield("variableBindingSet", refString);
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writedatafield("poseMatchingBone0", QString::number(poseMatchingBone0));
        writedatafield("poseMatchingBone1", QString::number(poseMatchingBone1));
        writedatafield("poseMatchingBone2", QString::number(poseMatchingBone2));
        writedatafield("enableComputeWorldFromModel", getBoolAsString(enableComputeWorldFromModel));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbExtractRagdollPoseModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbExtractRagdollPoseModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbExtractRagdollPoseModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbExtractRagdollPoseModifier::~hkbExtractRagdollPoseModifier(){
    refCount--;
}
