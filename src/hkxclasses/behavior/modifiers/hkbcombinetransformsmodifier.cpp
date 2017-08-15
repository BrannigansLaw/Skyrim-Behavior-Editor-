#include "hkbcombinetransformsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbCombineTransformsModifier
*/

uint hkbCombineTransformsModifier::refCount = 0;

QString hkbCombineTransformsModifier::classname = "hkbCombineTransformsModifier";

hkbCombineTransformsModifier::hkbCombineTransformsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      invertLeftTransform(false),
      invertRightTransform(false),
      invertResult(false)
{
    setType(HKB_COMBINE_TRANSFORMS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "CombineTransformsModifier"+QString::number(refCount);
}

QString hkbCombineTransformsModifier::getClassname(){
    return classname;
}

QString hkbCombineTransformsModifier::getName() const{
    return name;
}

bool hkbCombineTransformsModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "translationOut"){
            translationOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'translationOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rotationOut"){
            rotationOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'rotationOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "leftTranslation"){
            leftTranslation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'leftTranslation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "leftRotation"){
            leftRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'leftRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rightTranslation"){
            rightTranslation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'rightTranslation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rightRotation"){
            rightRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'rightRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "invertLeftTransform"){
            invertLeftTransform = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'invertLeftTransform' data field!\nObject Reference: "+ref);
            }
        }else if (text == "invertRightTransform"){
            invertRightTransform = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'invertRightTransform' data field!\nObject Reference: "+ref);
            }
        }else if (text == "invertResult"){
            invertResult = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'invertResult' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbCombineTransformsModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("translationOut"), translationOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rotationOut"), rotationOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("leftTranslation"), leftTranslation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("leftRotation"), leftRotation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rightTranslation"), rightTranslation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rightRotation"), rightRotation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("invertLeftTransform"), getBoolAsString(invertLeftTransform));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("invertRightTransform"), getBoolAsString(invertRightTransform));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("invertResult"), getBoolAsString(invertResult));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbCombineTransformsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbCombineTransformsModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbCombineTransformsModifier::evaulateDataValidity(){
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

hkbCombineTransformsModifier::~hkbCombineTransformsModifier(){
    refCount--;
}
