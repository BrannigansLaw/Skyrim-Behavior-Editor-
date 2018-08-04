#include "bscomputeaddboneanimmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSComputeAddBoneAnimModifier::refCount = 0;

const QString BSComputeAddBoneAnimModifier::classname = "BSComputeAddBoneAnimModifier";

BSComputeAddBoneAnimModifier::BSComputeAddBoneAnimModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      boneIndex(-1)
{
    setType(BS_COMPUTE_ADD_BONE_ANIM_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "ComputeAddBoneAnimModifier_"+QString::number(refCount);
}

const QString BSComputeAddBoneAnimModifier::getClassname(){
    return classname;
}

QString BSComputeAddBoneAnimModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSComputeAddBoneAnimModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "boneIndex"){
            boneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "boneIndex");
        }else if (text == "translationLSOut"){
            translationLSOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "translationLSOut");
        }else if (text == "rotationLSOut"){
            rotationLSOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rotationLSOut");
        }else if (text == "scaleLSOut"){
            scaleLSOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "scaleLSOut");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool BSComputeAddBoneAnimModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value, bool allownull){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value, allownull);
    };
    if (writer && !getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (getVariableBindingSetData()){
            refString = getVariableBindingSet()->getReferenceString();
        }
        writedatafield("variableBindingSet", refString, false);
        writedatafield("userData", QString::number(userData), false);
        writedatafield("name", name, false);
        writedatafield("enable", getBoolAsString(enable), false);
        writedatafield("boneIndex", QString::number(boneIndex), false);
        writedatafield("translationLSOut", translationLSOut.getValueAsString(), false);
        writedatafield("rotationLSOut", rotationLSOut.getValueAsString(), false);
        writedatafield("scaleLSOut", scaleLSOut.getValueAsString(), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSComputeAddBoneAnimModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSComputeAddBoneAnimModifier::unlink(){
    HkDynamicObject::unlink();
}

QString BSComputeAddBoneAnimModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!\n");
    }
    if (boneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": boneIndex out of range! Setting to last bone index!\n");
        boneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    setDataValidity(isvalid);
    return errors;
}

BSComputeAddBoneAnimModifier::~BSComputeAddBoneAnimModifier(){
    refCount--;
}
