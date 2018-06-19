#include "bsisactivemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSIsActiveModifier
*/

uint BSIsActiveModifier::refCount = 0;

QString BSIsActiveModifier::classname = "BSIsActiveModifier";

BSIsActiveModifier::BSIsActiveModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(2),
      enable(true),
      bIsActive0(false),
      bInvertActive0(false),
      bIsActive1(false),
      bInvertActive1(false),
      bIsActive2(false),
      bInvertActive2(false),
      bIsActive3(false),
      bInvertActive3(false),
      bIsActive4(false),
      bInvertActive4(false)
{
    setType(BS_IS_ACTIVE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "IsActiveModifier"+QString::number(refCount);
}

QString BSIsActiveModifier::getClassname(){
    return classname;
}

QString BSIsActiveModifier::getName() const{
    return name;
}

bool BSIsActiveModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "bIsActive0"){
            bIsActive0 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bIsActive0' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bInvertActive0"){
            bInvertActive0 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bInvertActive0' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bIsActive1"){
            bIsActive1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bIsActive1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bInvertActive1"){
            bInvertActive1 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bInvertActive1' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bIsActive2"){
            bIsActive2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bIsActive2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bInvertActive2"){
            bInvertActive2 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bInvertActive2' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bIsActive3"){
            bIsActive3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bIsActive3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bInvertActive3"){
            bInvertActive3 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bInvertActive3' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bIsActive4"){
            bIsActive4 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bIsActive4' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bInvertActive4"){
            bInvertActive4 = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'bInvertActive4' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSIsActiveModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bIsActive0"), getBoolAsString(bIsActive0));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bInvertActive0"), getBoolAsString(bInvertActive0));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bIsActive1"), getBoolAsString(bIsActive1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bInvertActive1"), getBoolAsString(bInvertActive1));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bIsActive2"), getBoolAsString(bIsActive2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bInvertActive2"), getBoolAsString(bInvertActive2));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bIsActive3"), getBoolAsString(bIsActive3));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bInvertActive3"), getBoolAsString(bInvertActive3));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bIsActive4"), getBoolAsString(bIsActive4));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bInvertActive4"), getBoolAsString(bInvertActive4));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSIsActiveModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSIsActiveModifier::unlink(){
    HkDynamicObject::unlink();
}

bool BSIsActiveModifier::evaluateDataValidity(){
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

BSIsActiveModifier::~BSIsActiveModifier(){
    refCount--;
}
