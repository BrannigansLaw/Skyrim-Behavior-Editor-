#include "hkblookatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbLookAtModifier
*/

uint hkbLookAtModifier::refCount = 0;

QString hkbLookAtModifier::classname = "hkbLookAtModifier";

hkbLookAtModifier::hkbLookAtModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      newTargetGain(0),
      onGain(0),
      offGain(0),
      limitAngleDegrees(0),
      limitAngleLeft(0),
      limitAngleRight(0),
      limitAngleUp(0),
      limitAngleDown(0),
      headIndex(-1),
      neckIndex(-1),
      isOn(false),
      individualLimitsOn(false),
      isTargetInsideLimitCone(false)
{
    setType(HKB_LOOK_AT_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "LookAtModifier"+QString::number(refCount);
}

QString hkbLookAtModifier::getClassname(){
    return classname;
}

QString hkbLookAtModifier::getName() const{
    return name;
}

bool hkbLookAtModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetWS"){
            targetWS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'targetWS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "headForwardLS"){
            headForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'headForwardLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckForwardLS"){
            neckForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'neckForwardLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckRightLS"){
            neckRightLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'neckRightLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eyePositionHS"){
            eyePositionHS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'eyePositionHS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "newTargetGain"){
            newTargetGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'newTargetGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleLeft"){
            limitAngleLeft = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitAngleLeft' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleRight"){
            limitAngleRight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitAngleRight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleUp"){
            limitAngleUp = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitAngleUp' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleDown"){
            limitAngleDown = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'limitAngleDown' data field!\nObject Reference: "+ref);
            }
        }else if (text == "headIndex"){
            headIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'headIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckIndex"){
            neckIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'neckIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isOn"){
            isOn = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'isOn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "individualLimitsOn"){
            individualLimitsOn = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'individualLimitsOn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isTargetInsideLimitCone"){
            isTargetInsideLimitCone = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'isTargetInsideLimitCone' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbLookAtModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetWS"), targetWS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("headForwardLS"), headForwardLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckForwardLS"), neckForwardLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckRightLS"), neckRightLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eyePositionHS"), eyePositionHS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("newTargetGain"), QString::number(newTargetGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(onGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(offGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(limitAngleDegrees, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleLeft"), QString::number(limitAngleLeft, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleRight"), QString::number(limitAngleRight, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleUp"), QString::number(limitAngleUp, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDown"), QString::number(limitAngleDown, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("headIndex"), QString::number(headIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckIndex"), QString::number(neckIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isOn"), getBoolAsString(isOn));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("individualLimitsOn"), getBoolAsString(individualLimitsOn));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isTargetInsideLimitCone"), getBoolAsString(isTargetInsideLimitCone));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbLookAtModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbLookAtModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbLookAtModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbLookAtModifier::~hkbLookAtModifier(){
    refCount--;
}
