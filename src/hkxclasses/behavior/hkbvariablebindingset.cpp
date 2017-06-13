#include "hkbvariablebindingset.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbVariableBindingSet
*/

uint hkbVariableBindingSet::refCount = 0;

QString hkbVariableBindingSet::classname = "hkbVariableBindingSet";

hkbVariableBindingSet::hkbVariableBindingSet(HkxFile *parent, long ref)
    : HkxObject(parent, ref),
      indexOfBindingToEnable(-1)
{
    setType(HKB_VARIABLE_BINDING_SET, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbVariableBindingSet::getClassname(){
    return classname;
}

int hkbVariableBindingSet::getNumberOfBindings() const{
    return bindings.size();
}

bool hkbVariableBindingSet::addBinding(const QString & path, const QString & name, int varIndex, hkBinding::BindingType type){
    int index = -1;
    bool exists = false;
    for (int i = 0; i < bindings.size(); i++){//Do this for below but remove the binding if the paths are the same...
        if (bindings.at(i).memberPath == path){
            if (type == hkBinding::BINDING_TYPE_VARIABLE){
                bindings[i].variableIndex = varIndex;
            }else if (type == hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                index = static_cast<BehaviorFile *>(getParentFile())->addCharacterProperty(varIndex);
                if (index > -1){
                    bindings[i].variableIndex = index;
                }else{
                    return false;
                }
            }
            bindings[i].bindingType = type;
            if (path == "enable"){
                indexOfBindingToEnable = i;
            }
            exists = true;
        }
    }
    if (!exists){
        if (type == hkBinding::BINDING_TYPE_VARIABLE){
            bindings.append(hkBinding(path, varIndex, -1, type));
        }else if (type == hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
            index = static_cast<BehaviorFile *>(getParentFile())->addCharacterProperty(varIndex);
            if (index > -1){
                bindings.append(hkBinding(path, index, -1, type));
            }else{
                return false;
            }
        }
        if (path == "enable"){
            indexOfBindingToEnable = bindings.size() - 1;
        }
        return true;
    }
    return true;
}

void hkbVariableBindingSet::removeBinding(const QString & path){
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).memberPath == path){
            bindings.removeAt(i);
        }
    }
}

void hkbVariableBindingSet::removeBinding(int varIndex){
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).variableIndex == varIndex){
            bindings.removeAt(i);
        }
    }
}

QString hkbVariableBindingSet::getPathOfBindingAt(int index){
    QString path;
    if (index < bindings.size() && index >= 0){
        path = bindings.at(index).memberPath;
    }
    return path;
}

int hkbVariableBindingSet::getVariableIndexOfBinding(const QString & path) const{
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).memberPath == path){
            if (bindings.at(i).bindingType == hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                return static_cast<BehaviorFile *>(getParentFile())->findCharacterPropertyIndexFromCharacter(bindings.at(i).variableIndex);
            }else{
                return bindings.at(i).variableIndex;
            }
        }
    }
    return -1;
}

hkbVariableBindingSet::hkBinding::BindingType hkbVariableBindingSet::getBindingType(int index) const{
    if (index < bindings.size() && index >= 0){
        return bindings.at(index).bindingType;
    }
    return hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE;
}

hkbVariableBindingSet::hkBinding::BindingType hkbVariableBindingSet::getBindingType(const QString & path) const{
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).memberPath == path){
            return bindings.at(i).bindingType;
        }
    }
    return hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE;
}

bool hkbVariableBindingSet::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    int numtrans = 0;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "bindings"){
            numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                bindings.append(hkBinding());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "memberPath"){
                        bindings.last().memberPath = reader.getElementValueAt(index);
                        if (bindings.last().memberPath == ""){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'memberPath' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "variableIndex"){
                        bindings.last().variableIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "bitIndex"){
                        bindings.last().bitIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'bitIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if(reader.getNthAttributeValueAt(index, 0) == "bindingType"){
                        if (reader.getElementValueAt(index) == "BINDING_TYPE_VARIABLE"){
                            bindings.last().bindingType = hkBinding::BINDING_TYPE_VARIABLE;
                        }else if (reader.getElementValueAt(index) == "BINDING_TYPE_CHARACTER_PROPERTY"){
                            bindings.last().bindingType = hkBinding::BINDING_TYPE_CHARACTER_PROPERTY;
                        }else{
                            writeToLog(getClassname()+": readData()!\n'bindingType' data field contains an invalid string!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "indexOfBindingToEnable"){
            indexOfBindingToEnable = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'indexOfBindingToEnable' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbVariableBindingSet::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString string;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"bindings", QString::number(bindings.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < bindings.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("memberPath"), bindings.at(i).memberPath);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableIndex"), QString::number(bindings.at(i).variableIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bitIndex"), QString::number(bindings.at(i).bitIndex));
            if (bindings.at(i).bindingType == hkBinding::BINDING_TYPE_VARIABLE){
                string = "BINDING_TYPE_VARIABLE";
            }else{
                string = "BINDING_TYPE_CHARACTER_PROPERTY";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("bindingType"), string);
            writer->writeLine(writer->object, false);
        }
        if (bindings.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("indexOfBindingToEnable"), QString::number(indexOfBindingToEnable));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbVariableBindingSet::link(){
    return true;
}

bool hkbVariableBindingSet::evaulateDataValidity(){
    //Do later...
    return true;
}

hkbVariableBindingSet::~hkbVariableBindingSet(){
    refCount--;
}
