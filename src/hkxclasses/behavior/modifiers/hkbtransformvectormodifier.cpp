#include "hkbtransformvectormodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbTransformVectorModifier
*/

uint hkbTransformVectorModifier::refCount = 0;

QString hkbTransformVectorModifier::classname = "hkbTransformVectorModifier";

hkbTransformVectorModifier::hkbTransformVectorModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      rotateOnly(true),
      inverse(true),
      computeOnActivate(true),
      computeOnModify(true)
{
    setType(HKB_TRANSFORM_VECTOR_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "TransformVectorModifier"+QString::number(refCount);
}

QString hkbTransformVectorModifier::getClassname(){
    return classname;
}

QString hkbTransformVectorModifier::getName() const{
    return name;
}

bool hkbTransformVectorModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "rotation"){
            rotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "translation"){
            translation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'translation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "vectorIn"){
            vectorIn = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'vectorIn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "vectorOut"){
            vectorOut = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'vectorOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rotateOnly"){
            rotateOnly = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rotateOnly' data field!\nObject Reference: "+ref);
            }
        }else if (text == "inverse"){
            inverse = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'inverse' data field!\nObject Reference: "+ref);
            }
        }else if (text == "computeOnActivate"){
            computeOnActivate = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'computeOnActivate' data field!\nObject Reference: "+ref);
            }
        }else if (text == "computeOnModify"){
            computeOnModify = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'computeOnModify' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbTransformVectorModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rotation"), rotation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("translation"), translation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("vectorIn"), vectorIn.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("vectorOut"), vectorOut.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rotateOnly"), getBoolAsString(rotateOnly));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("inverse"), getBoolAsString(inverse));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("computeOnActivate"), getBoolAsString(computeOnActivate));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("computeOnModify"), getBoolAsString(computeOnModify));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbTransformVectorModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbTransformVectorModifier::unlink(){
    HkDynamicObject::unlink();
}

QString hkbTransformVectorModifier::evaluateDataValidity(){
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

hkbTransformVectorModifier::~hkbTransformVectorModifier(){
    refCount--;
}
