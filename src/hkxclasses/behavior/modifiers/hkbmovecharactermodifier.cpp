#include "hkbmovecharactermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbMoveCharacterModifier
*/

uint hkbMoveCharacterModifier::refCount = 0;

QString hkbMoveCharacterModifier::classname = "hkbMoveCharacterModifier";

hkbMoveCharacterModifier::hkbMoveCharacterModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_MOVE_CHARACTER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "MoveCharacterModifier"+QString::number(refCount);
}

QString hkbMoveCharacterModifier::getClassname(){
    return classname;
}

QString hkbMoveCharacterModifier::getName() const{
    return name;
}

bool hkbMoveCharacterModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "offsetPerSecondMS"){
            offsetPerSecondMS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'offsetPerSecondMS' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbMoveCharacterModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offsetPerSecondMS"), offsetPerSecondMS.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbMoveCharacterModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbMoveCharacterModifier::unlink(){
    HkDynamicObject::unlink();
}

QString hkbMoveCharacterModifier::evaluateDataValidity(){
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

hkbMoveCharacterModifier::~hkbMoveCharacterModifier(){
    refCount--;
}
