#include "bslimbikmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint BSLimbIKModifier::refCount = 0;

const QString BSLimbIKModifier::classname = "BSLimbIKModifier";

BSLimbIKModifier::BSLimbIKModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      limitAngleDegrees(0),
      startBoneIndex(-1),
      endBoneIndex(-1),
      gain(0),
      boneRadius(0),
      castOffset(0)
{
    setType(BS_LIMB_IK_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "LimbIKModifier_"+QString::number(refCount);
}

const QString BSLimbIKModifier::getClassname(){
    return classname;
}

QString BSLimbIKModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool BSLimbIKModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleDegrees");
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "startBoneIndex");
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "endBoneIndex");
        }else if (text == "gain"){
            gain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "gain");
        }else if (text == "boneRadius"){
            boneRadius = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "boneRadius");
        }else if (text == "castOffset"){
            castOffset = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "castOffset");
        }
    }
    index--;
    return true;
}

bool BSLimbIKModifier::write(HkxXMLWriter *writer){
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
        writedatafield("limitAngleDegrees", QString::number(limitAngleDegrees, char('f'), 6), false);
        writedatafield("startBoneIndex", QString::number(startBoneIndex), false);
        writedatafield("endBoneIndex", QString::number(endBoneIndex), false);
        writedatafield("gain", QString::number(gain, char('f'), 6), false);
        writedatafield("boneRadius", QString::number(boneRadius, char('f'), 6), false);
        writedatafield("castOffset", QString::number(castOffset, char('f'), 6), false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSLimbIKModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSLimbIKModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString BSLimbIKModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto setinvalid = [&](const QString & message){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": "+message+"!");
    };
    QString temp = HkDynamicObject::evaluateDataValidity();
    (temp != "") ? errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n"): NULL;
    (name == "") ? setinvalid("Invalid name") : NULL;
    if (startBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        setinvalid("startBoneIndex out of range! Setting to max index in range!");
        startBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    if (endBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        setinvalid("endBoneIndex out of range! Setting to max index in range!");
        endBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    setDataValidity(isvalid);
    return errors;
}

BSLimbIKModifier::~BSLimbIKModifier(){
    refCount--;
}
