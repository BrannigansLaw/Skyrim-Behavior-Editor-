#include "hkbdampingmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbDampingModifier
*/

uint hkbDampingModifier::refCount = 0;

QString hkbDampingModifier::classname = "hkbDampingModifier";

hkbDampingModifier::hkbDampingModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(1),
      enable(true),
      kP(0),
      kI(0),
      kD(0),
      enableScalarDamping(true),
      enableVectorDamping(false),
      rawValue(0),
      dampedValue(0),
      errorSum(0),
      previousError(0)
{
    setType(HKB_DAMPING_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "DampingModifier"+QString::number(refCount);
}

QString hkbDampingModifier::getClassname(){
    return classname;
}

QString hkbDampingModifier::getName() const{
    return name;
}

bool hkbDampingModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "kP"){
            kP = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'kP' data field!\nObject Reference: "+ref);
            }
        }else if (text == "kI"){
            kI = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'kI' data field!\nObject Reference: "+ref);
            }
        }else if (text == "kD"){
            kD = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'kD' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enableScalarDamping"){
            enableScalarDamping = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enableScalarDamping' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enableVectorDamping"){
            enableVectorDamping = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enableVectorDamping' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rawValue"){
            rawValue = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rawValue' data field!\nObject Reference: "+ref);
            }
        }else if (text == "dampedValue"){
            dampedValue = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'dampedValue' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rawVector"){
            rawVector = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rawVector' data field!\nObject Reference: "+ref);
            }
        }else if (text == "dampedVector"){
            dampedVector = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'dampedVector' data field!\nObject Reference: "+ref);
            }
        }else if (text == "vecErrorSum"){
            vecErrorSum = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'vecErrorSum' data field!\nObject Reference: "+ref);
            }
        }else if (text == "vecPreviousError"){
            vecPreviousError = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'vecPreviousError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "errorSum"){
            errorSum = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'errorSum' data field!\nObject Reference: "+ref);
            }
        }else if (text == "previousError"){
            previousError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'previousError' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbDampingModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("kP"), QString::number(kP, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("kI"), QString::number(kI, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("kD"), QString::number(kD, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enableScalarDamping"), getBoolAsString(enableScalarDamping));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enableVectorDamping"), getBoolAsString(enableVectorDamping));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rawValue"), QString::number(rawValue, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("dampedValue"), QString::number(dampedValue, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rawVector"), rawVector.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("dampedVector"), dampedVector.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("vecErrorSum"), vecErrorSum.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("vecPreviousError"), vecPreviousError.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("errorSum"), QString::number(errorSum, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("previousError"), QString::number(previousError, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbDampingModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbDampingModifier::unlink(){
    HkDynamicObject::unlink();
}

QString hkbDampingModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbDampingModifier::~hkbDampingModifier(){
    refCount--;
}
