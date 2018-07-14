#include "hkbevaluatehandlemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbEvaluateHandleModifier
*/

uint hkbEvaluateHandleModifier::refCount = 0;

QString hkbEvaluateHandleModifier::classname = "hkbEvaluateHandleModifier";

QStringList hkbEvaluateHandleModifier::HandleChangeMode = {"HANDLE_CHANGE_MODE_ABRUPT", "HANDLE_CHANGE_MODE_CONSTANT_VELOCITY"};

hkbEvaluateHandleModifier::hkbEvaluateHandleModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      isValidOut(false),
      extrapolationTimeStep(0),
      handleChangeSpeed(0),
      handleChangeMode(HandleChangeMode.first())
{
    setType(HKB_EVALUATE_HANDLE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "EvaluateHandleModifier"+QString::number(refCount);
}

QString hkbEvaluateHandleModifier::getClassname(){
    return classname;
}

QString hkbEvaluateHandleModifier::getName() const{
    return name;
}

bool hkbEvaluateHandleModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "handle"){
            if (!handle.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'handle' reference!\nObject Reference: "+ref);
            }
        }else if (text == "handlePositionOut"){
            handlePositionOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'handlePositionOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "handleRotationOut"){
            handleRotationOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'handleRotationOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isValidOut"){
            isValidOut = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'isValidOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "extrapolationTimeStep"){
            extrapolationTimeStep = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'extrapolationTimeStep' data field!\nObject Reference: "+ref);
            }
        }else if (text == "handleChangeSpeed"){
            handleChangeSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'handleChangeSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "handleChangeMode"){
            handleChangeMode = reader.getElementValueAt(index);
            if (!HandleChangeMode.contains(handleChangeMode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nInvalid 'handleChangeMode' data!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbEvaluateHandleModifier::write(HkxXMLWriter *writer){
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
        if (handle.data()){
            refString = handle.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handle"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handlePositionOut"), handlePositionOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleRotationOut"), handleRotationOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isValidOut"), getBoolAsString(isValidOut));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("extrapolationTimeStep"), QString::number(extrapolationTimeStep, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleChangeSpeed"), QString::number(handleChangeSpeed, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleChangeMode"), handleChangeMode);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbEvaluateHandleModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbEvaluateHandleModifier::unlink(){
    HkDynamicObject::unlink();
}

QString hkbEvaluateHandleModifier::evaluateDataValidity(){
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
    setDataValidity(isvalid);
    return errors;
}

hkbEvaluateHandleModifier::~hkbEvaluateHandleModifier(){
    refCount--;
}
