#include "hkbrotatecharactermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbRotateCharacterModifier
*/

uint hkbRotateCharacterModifier::refCount = 0;

QString hkbRotateCharacterModifier::classname = "hkbRotateCharacterModifier";

hkbRotateCharacterModifier::hkbRotateCharacterModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      degreesPerSecond(0),
      speedMultiplier(0)
{
    setType(HKB_ROTATE_CHARACTER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "RotateCharacterModifier"+QString::number(refCount);
}

QString hkbRotateCharacterModifier::getClassname(){
    return classname;
}

QString hkbRotateCharacterModifier::getName() const{
    return name;
}

bool hkbRotateCharacterModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "degreesPerSecond"){
            degreesPerSecond = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'degreesPerSecond' data field!\nObject Reference: "+ref);
            }
        }else if (text == "speedMultiplier"){
            speedMultiplier = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'speedMultiplier' data field!\nObject Reference: "+ref);
            }
        }else if (text == "axisOfRotation"){
            axisOfRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'axisOfRotation' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbRotateCharacterModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("degreesPerSecond"), QString::number(degreesPerSecond, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("speedMultiplier"), QString::number(speedMultiplier, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("axisOfRotation"), axisOfRotation.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbRotateCharacterModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbRotateCharacterModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbRotateCharacterModifier::evaluateDataValidity(){
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
    if (errors != ""){
        LogFile::writeToLog(errors);
    }
    setDataValidity(isvalid);
    return isvalid;
}

hkbRotateCharacterModifier::~hkbRotateCharacterModifier(){
    refCount--;
}
