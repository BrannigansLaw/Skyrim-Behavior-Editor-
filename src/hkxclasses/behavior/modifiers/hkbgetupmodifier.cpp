#include "hkbgetupmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbGetUpModifier
*/

uint hkbGetUpModifier::refCount = 0;

QString hkbGetUpModifier::classname = "hkbGetUpModifier";

hkbGetUpModifier::hkbGetUpModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      duration(0),
      alignWithGroundDuration(0),
      rootBoneIndex(-1),
      otherBoneIndex(-1),
      anotherBoneIndex(-1)
{
    setType(HKB_GET_UP_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbGetUpModifier"+QString::number(refCount);
}

QString hkbGetUpModifier::getClassname(){
    return classname;
}

QString hkbGetUpModifier::getName() const{
    return name;
}

bool hkbGetUpModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "groundNormal"){
            groundNormal = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'groundNormal' data field!\nObject Reference: "+ref);
            }
        }else if (text == "duration"){
            duration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'duration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "alignWithGroundDuration"){
            alignWithGroundDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'alignWithGroundDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootBoneIndex"){
            rootBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'rootBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "otherBoneIndex"){
            otherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'otherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "anotherBoneIndex"){
            anotherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'anotherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbGetUpModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("groundNormal"), groundNormal.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("duration"), QString::number(duration));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("alignWithGroundDuration"), QString::number(alignWithGroundDuration));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rootBoneIndex"), QString::number(rootBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("otherBoneIndex"), QString::number(otherBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("anotherBoneIndex"), QString::number(anotherBoneIndex));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbGetUpModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbGetUpModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbGetUpModifier::evaulateDataValidity(){
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

hkbGetUpModifier::~hkbGetUpModifier(){
    refCount--;
}
