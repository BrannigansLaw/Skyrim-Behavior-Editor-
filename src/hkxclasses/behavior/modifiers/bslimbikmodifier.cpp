#include "bslimbikmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSLimbIKModifier
*/

uint BSLimbIKModifier::refCount = 0;

QString BSLimbIKModifier::classname = "BSLimbIKModifier";

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
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "LimbIKModifier"+QString::number(refCount);
}

QString BSLimbIKModifier::getClassname(){
    return classname;
}

QString BSLimbIKModifier::getName() const{
    return name;
}

bool BSLimbIKModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'startBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'endBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "gain"){
            gain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'gain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "boneRadius"){
            boneRadius = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'boneRadius' data field!\nObject Reference: "+ref);
            }
        }else if (text == "castOffset"){
            castOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'castOffset' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSLimbIKModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(limitAngleDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startBoneIndex"), QString::number(startBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("endBoneIndex"), QString::number(endBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("gain"), QString::number(gain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneRadius"), QString::number(boneRadius, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("castOffset"), QString::number(castOffset, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSLimbIKModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSLimbIKModifier::unlink(){
    HkDynamicObject::unlink();
}

QString BSLimbIKModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (startBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": startBoneIndex out of range! Setting to last bone index!\n");
        startBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    if (endBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": endBoneIndex out of range! Setting to last bone index!\n");
        endBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    setDataValidity(isvalid);
    return errors;
}

BSLimbIKModifier::~BSLimbIKModifier(){
    refCount--;
}
