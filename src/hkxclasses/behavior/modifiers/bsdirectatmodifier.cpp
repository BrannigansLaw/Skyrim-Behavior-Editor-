#include "bsdirectatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSDirectAtModifier
*/

uint BSDirectAtModifier::refCount = 0;

QString BSDirectAtModifier::classname = "BSDirectAtModifier";

BSDirectAtModifier::BSDirectAtModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(2),
      enable(true),
      directAtTarget(true),
      sourceBoneIndex(-1),
      startBoneIndex(-1),
      endBoneIndex(-1),
      limitHeadingDegrees(0),
      limitPitchDegrees(0),
      offsetHeadingDegrees(0),
      offsetPitchDegrees(0),
      onGain(0),
      offGain(0),
      userInfo(0),
      directAtCamera(false),
      directAtCameraX(0),
      directAtCameraY(0),
      directAtCameraZ(0),
      active(false),
      currentHeadingOffset(0),
      currentPitchOffset(0)
{
    setType(BS_DIRECT_AT_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "DirectAtModifier"+QString::number(refCount);
}

QString BSDirectAtModifier::getClassname(){
    return classname;
}

QString BSDirectAtModifier::getName() const{
    return name;
}

bool BSDirectAtModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtTarget"){
            directAtTarget = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'directAtTarget' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sourceBoneIndex"){
            sourceBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'sourceBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'startBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'endBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitHeadingDegrees"){
            limitHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitHeadingDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitPitchDegrees"){
            limitPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitPitchDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offsetHeadingDegrees"){
            offsetHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offsetHeadingDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offsetPitchDegrees"){
            offsetPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offsetPitchDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetLocation"){
            targetLocation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'targetLocation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userInfo"){
            userInfo = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'userInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCamera"){
            directAtCamera = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'directAtCamera' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraX"){
            directAtCameraX = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'directAtCameraX' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraY"){
            directAtCameraY = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'directAtCameraY' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraZ"){
            directAtCameraZ = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'directAtCameraZ' data field!\nObject Reference: "+ref);
            }
        }else if (text == "active"){
            active = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'active' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentHeadingOffset"){
            currentHeadingOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'currentHeadingOffset' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentPitchOffset"){
            currentPitchOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'currentPitchOffset' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSDirectAtModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtTarget"), getBoolAsString(directAtTarget));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sourceBoneIndex"), QString::number(sourceBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startBoneIndex"), QString::number(startBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("endBoneIndex"), QString::number(endBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitHeadingDegrees"), QString::number(limitHeadingDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitPitchDegrees"), QString::number(limitPitchDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offsetHeadingDegrees"), QString::number(offsetHeadingDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offsetPitchDegrees"), QString::number(offsetPitchDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(onGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(offGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("axisOfRotation"), targetLocation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userInfo"), QString::number(userInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCamera"), getBoolAsString(directAtCamera));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraX"), QString::number(directAtCameraX, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraY"), QString::number(directAtCameraY, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraZ"), QString::number(directAtCameraZ, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("active"), getBoolAsString(active));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentHeadingOffset"), QString::number(currentHeadingOffset, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentPitchOffset"), QString::number(currentPitchOffset, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSDirectAtModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSDirectAtModifier::unlink(){
    HkDynamicObject::unlink();
}

bool BSDirectAtModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (!HkDynamicObject::evaluateDataValidity()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (sourceBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": sourceBoneIndex out of range!\n");
    }
    if (startBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": startBoneIndex out of range!\n");
    }
    if (endBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": endBoneIndex out of range!\n");
    }
    if (errors != ""){
        LogFile::writeToLog(errors);
    }
    setDataValidity(isvalid);
    return isvalid;
}

BSDirectAtModifier::~BSDirectAtModifier(){
    refCount--;
}
