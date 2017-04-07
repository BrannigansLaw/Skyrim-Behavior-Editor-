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
      userData(0),
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
    name = "BSDirectAtModifier"+QString::number(refCount);
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
            if (!variableBindingSet.readReference(index, reader)){
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
        }else if (text == "directAtTarget"){
            directAtTarget = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'directAtTarget' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sourceBoneIndex"){
            sourceBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sourceBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'startBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'endBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitHeadingDegrees"){
            limitHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitHeadingDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitPitchDegrees"){
            limitPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitPitchDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offsetHeadingDegrees"){
            offsetHeadingDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'offsetHeadingDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offsetPitchDegrees"){
            offsetPitchDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'offsetPitchDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetLocation"){
            targetLocation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetLocation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userInfo"){
            userInfo = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCamera"){
            directAtCamera = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'directAtCamera' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraX"){
            directAtCameraX = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'directAtCameraX' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraY"){
            directAtCameraY = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'directAtCameraY' data field!\nObject Reference: "+ref);
            }
        }else if (text == "directAtCameraZ"){
            directAtCameraZ = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'directAtCameraZ' data field!\nObject Reference: "+ref);
            }
        }else if (text == "active"){
            active = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'active' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentHeadingOffset"){
            currentHeadingOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'currentHeadingOffset' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentPitchOffset"){
            currentPitchOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'currentPitchOffset' data field!\nObject Reference: "+ref);
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitHeadingDegrees"), QString::number(limitHeadingDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitPitchDegrees"), QString::number(limitPitchDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offsetHeadingDegrees"), QString::number(offsetHeadingDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offsetPitchDegrees"), QString::number(offsetPitchDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(onGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(offGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("axisOfRotation"), targetLocation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userInfo"), QString::number(userInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCamera"), getBoolAsString(directAtCamera));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraX"), QString::number(directAtCameraX));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraY"), QString::number(directAtCameraY));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("directAtCameraZ"), QString::number(directAtCameraZ));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("active"), getBoolAsString(active));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentHeadingOffset"), QString::number(currentHeadingOffset));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentPitchOffset"), QString::number(currentPitchOffset));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool BSDirectAtModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSDirectAtModifier::unlink(){
    HkDynamicObject::unlink();
}

bool BSDirectAtModifier::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSDirectAtModifier::~BSDirectAtModifier(){
    refCount--;
}
