#include "hkbattributemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbAttributeModifier
*/

uint hkbAttributeModifier::refCount = 0;

QString hkbAttributeModifier::classname = "hkbAttributeModifier";

hkbAttributeModifier::hkbAttributeModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_ATTRIBUTE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "AttributeModifier"+QString::number(refCount);
}

QString hkbAttributeModifier::getClassname(){
    return classname;
}

QString hkbAttributeModifier::getName() const{
    return name;
}

bool hkbAttributeModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
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
        }else if (text == "assignments"){
            int numlegs = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numlegs; j++){
                assignments.append(hkAttributeMod());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "attributeIndex"){
                        assignments.last().attributeIndex = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'attributeIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "attributeValue"){
                        assignments.last().attributeValue = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'attributeValue' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool hkbAttributeModifier::write(HkxXMLWriter *writer){
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
        list1 = {writer->name, writer->numelements};
        list2 = {"assignments", QString::number(assignments.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < assignments.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attributeIndex"), QString::number(assignments.at(i).attributeIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("attributeValue"), QString::number(assignments.at(i).attributeValue, char('f'), 6));
            writer->writeLine(writer->object, false);
        }
        if (assignments.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbAttributeModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbAttributeModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbAttributeModifier::evaluateDataValidity(){  //Check for valid event id???
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbAttributeModifier::~hkbAttributeModifier(){
    refCount--;
}
