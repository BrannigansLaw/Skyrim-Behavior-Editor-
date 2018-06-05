#include "hkbtwistmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbTwistModifier
*/

uint hkbTwistModifier::refCount = 0;

QString hkbTwistModifier::classname = "hkbTwistModifier";

QStringList hkbTwistModifier::SetAngleMethod = {"LINEAR", "RAMPED"};
QStringList hkbTwistModifier::RotationAxisCoordinates = {"ROTATION_AXIS_IN_MODEL_COORDINATES", "ROTATION_AXIS_IN_LOCAL_COORDINATES"};

hkbTwistModifier::hkbTwistModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(1),
      enable(true),
      twistAngle(0),
      startBoneIndex(-1),
      endBoneIndex(-1),
      setAngleMethod(SetAngleMethod.first()),
      rotationAxisCoordinates(RotationAxisCoordinates.first()),
      isAdditive(true)
{
    setType(HKB_TWIST_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "TwistModifier"+QString::number(refCount);
}

QString hkbTwistModifier::getClassname(){
    return classname;
}

QString hkbTwistModifier::getName() const{
    return name;
}

bool hkbTwistModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "axisOfRotation"){
            axisOfRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'axisOfRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "twistAngle"){
            twistAngle = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'twistAngle' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startBoneIndex"){
            startBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'startBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endBoneIndex"){
            endBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'endBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "setAngleMethod"){
            setAngleMethod = reader.getElementValueAt(index);
            if (!SetAngleMethod.contains(setAngleMethod)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'setAngleMethod' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rotationAxisCoordinates"){
            rotationAxisCoordinates = reader.getElementValueAt(index);
            if (!RotationAxisCoordinates.contains(rotationAxisCoordinates)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'rotationAxisCoordinates' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isAdditive"){
            isAdditive = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'isAdditive' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbTwistModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("axisOfRotation"), axisOfRotation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("twistAngle"), QString::number(twistAngle, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startBoneIndex"), QString::number(startBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("endBoneIndex"), QString::number(endBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("setAngleMethod"), setAngleMethod);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rotationAxisCoordinates"), rotationAxisCoordinates);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isAdditive"), getBoolAsString(isAdditive));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbTwistModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbTwistModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbTwistModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!SetAngleMethod.contains(setAngleMethod)){
    }else if (!RotationAxisCoordinates.contains(rotationAxisCoordinates)){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbTwistModifier::~hkbTwistModifier(){
    refCount--;
}
