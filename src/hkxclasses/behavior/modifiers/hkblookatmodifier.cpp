#include "hkblookatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbLookAtModifier::refCount = 0;

const QString hkbLookAtModifier::classname = "hkbLookAtModifier";

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
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "LookAtModifier_"+QString::number(refCount);
}

const QString hkbLookAtModifier::getClassname(){
    return classname;
}

QString hkbLookAtModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbLookAtModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "targetWS"){
            targetWS = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "targetWS");
        }else if (text == "headForwardLS"){
            headForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "headForwardLS");
        }else if (text == "neckForwardLS"){
            neckForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "neckForwardLS");
        }else if (text == "neckRightLS"){
            neckRightLS = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "neckRightLS");
        }else if (text == "eyePositionHS"){
            eyePositionHS = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "eyePositionHS");
        }else if (text == "newTargetGain"){
            newTargetGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "newTargetGain");
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "onGain");
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "offGain");
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleDegrees");
        }else if (text == "limitAngleLeft"){
            limitAngleLeft = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleLeft");
        }else if (text == "limitAngleRight"){
            limitAngleRight = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleRight");
        }else if (text == "limitAngleUp"){
            limitAngleUp = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleUp");
        }else if (text == "limitAngleDown"){
            limitAngleDown = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "limitAngleDown");
        }else if (text == "headIndex"){
            headIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "headIndex");
        }else if (text == "neckIndex"){
            neckIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "neckIndex");
        }else if (text == "isOn"){
            isOn = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "isOn");
        }else if (text == "individualLimitsOn"){
            individualLimitsOn = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "individualLimitsOn");
        }else if (text == "isTargetInsideLimitCone"){
            isTargetInsideLimitCone = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "isTargetInsideLimitCone");
        }
    }
    index--;
    return true;
}

bool hkbLookAtModifier::write(HkxXMLWriter *writer){
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
        writedatafield("targetWS", targetWS.getValueAsString());
        writedatafield("headForwardLS", headForwardLS.getValueAsString());
        writedatafield("neckForwardLS", neckForwardLS.getValueAsString());
        writedatafield("neckRightLS", neckRightLS.getValueAsString());
        writedatafield("eyePositionHS", eyePositionHS.getValueAsString());
        writedatafield("newTargetGain", QString::number(newTargetGain, char('f'), 6));
        writedatafield("onGain", QString::number(onGain, char('f'), 6));
        writedatafield("offGain", QString::number(offGain, char('f'), 6));
        writedatafield("limitAngleDegrees", QString::number(limitAngleDegrees, char('f'), 6));
        writedatafield("limitAngleLeft", QString::number(limitAngleLeft, char('f'), 6));
        writedatafield("limitAngleRight", QString::number(limitAngleRight, char('f'), 6));
        writedatafield("limitAngleUp", QString::number(limitAngleUp, char('f'), 6));
        writedatafield("limitAngleDown", QString::number(limitAngleDown, char('f'), 6));
        writedatafield("headIndex", QString::number(headIndex));
        writedatafield("neckIndex", QString::number(neckIndex));
        writedatafield("isOn", getBoolAsString(isOn));
        writedatafield("individualLimitsOn", getBoolAsString(individualLimitsOn));
        writedatafield("isTargetInsideLimitCone", getBoolAsString(isTargetInsideLimitCone));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbLookAtModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbLookAtModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbLookAtModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    auto temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!");
    }
    if (headIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": headIndex out of range! Setting to last bone index!");
        headIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones(true) - 1;
    }
    if (neckIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": neckIndex out of range! Setting to last bone index!");
        neckIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones(true) - 1;
    }
    setDataValidity(isvalid);
    return errors;
}

hkbLookAtModifier::~hkbLookAtModifier(){
    refCount--;
}
