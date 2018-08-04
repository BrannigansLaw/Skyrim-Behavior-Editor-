#include "hkbcomputerotationtotargetmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbComputeRotationToTargetModifier::refCount = 0;

const QString hkbComputeRotationToTargetModifier::classname = "hkbComputeRotationToTargetModifier";

hkbComputeRotationToTargetModifier::hkbComputeRotationToTargetModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      resultIsDelta(true)
{
    setType(HKB_COMPUTE_ROTATION_TO_TARGET_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "ComputeRotationToTargetModifier_"+QString::number(refCount);
}

const QString hkbComputeRotationToTargetModifier::getClassname(){
    return classname;
}

QString hkbComputeRotationToTargetModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbComputeRotationToTargetModifier::readData(const HkxXmlReader &reader, long & index){
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
        }else if (text == "rotationOut"){
            rotationOut = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "rotationOut");
        }else if (text == "targetPosition"){
            targetPosition = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "targetPosition");
        }else if (text == "currentPosition"){
            currentPosition = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "currentPosition");
        }else if (text == "currentRotation"){
            currentRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "currentRotation");
        }else if (text == "localAxisOfRotation"){
            localAxisOfRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "localAxisOfRotation");
        }else if (text == "localFacingDirection"){
            localFacingDirection = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "localFacingDirection");
        }else if (text == "resultIsDelta"){
            resultIsDelta = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "resultIsDelta");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbComputeRotationToTargetModifier::write(HkxXMLWriter *writer){
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
        writedatafield("rotationOut", rotationOut.getValueAsString());
        writedatafield("targetPosition", targetPosition.getValueAsString());
        writedatafield("currentPosition", currentPosition.getValueAsString());
        writedatafield("currentRotation", currentRotation.getValueAsString());
        writedatafield("localAxisOfRotation", localAxisOfRotation.getValueAsString());
        writedatafield("localFacingDirection", localFacingDirection.getValueAsString());
        writedatafield("resultIsDelta", getBoolAsString(resultIsDelta));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (getVariableBindingSetData() && !getVariableBindingSet()->write(writer)){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbComputeRotationToTargetModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbComputeRotationToTargetModifier::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
}

QString hkbComputeRotationToTargetModifier::evaluateDataValidity(){
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
    setDataValidity(isvalid);
    return errors;
}

hkbComputeRotationToTargetModifier::~hkbComputeRotationToTargetModifier(){
    refCount--;
}
