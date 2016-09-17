#include "hkbvariablebindingset.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbVariableBindingSet
*/

uint hkbVariableBindingSet::refCount = 0;

QString hkbVariableBindingSet::classname = "hkbVariableBindingSet";

hkbVariableBindingSet::hkbVariableBindingSet(BehaviorFile *parent)
    : HkxObject(parent),
      indexOfBindingToEnable(-1)
{
    setType(HKB_VARIABLE_BINDING_SET, TYPE_OTHER);
    refCount++;
}

QString hkbVariableBindingSet::getClassname(){
    return classname;
}

void hkbVariableBindingSet::addBinding(const QString & path, int varIndex, hkBinding::BindingType type){
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).memberPath == path){
            bindings[i].variableIndex = varIndex;
            bindings[i].bindingType = type;
            if (path == "enable"){
                indexOfBindingToEnable = i;
            }
            return;
        }
    }
    bindings.append(hkBinding(path, varIndex, -1, type));
    if (path == "enable"){
        indexOfBindingToEnable = bindings.size() - 1;
    }
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

int hkbVariableBindingSet::getVariableIndexOfBinding(const QString & path) const{
    for (int i = 0; i < bindings.size(); i++){
        if (bindings.at(i).memberPath == path){
            return bindings.at(i).variableIndex;
        }
    }
    return -1;
}

bool hkbVariableBindingSet::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "bindings"){
            int numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                bindings.append(hkBinding());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "memberPath"){
                        bindings.last().memberPath = reader.getElementValueAt(index);
                        if (bindings.last().memberPath == ""){
                            writeToLog("hkbVariableBindingSet: readData()!\nFailed to properly read 'memberPath' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "variableIndex"){
                        bindings.last().variableIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbVariableBindingSet: readData()!\nFailed to properly read 'variableIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "bitIndex"){
                        bindings.last().bitIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog("hkbVariableBindingSet: readData()!\nFailed to properly read 'bitIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if(reader.getNthAttributeValueAt(index, 0) == "bindingType"){
                        if (reader.getElementValueAt(index) == "BINDING_TYPE_VARIABLE"){
                            bindings.last().bindingType = hkBinding::BINDING_TYPE_VARIABLE;
                        }else if (reader.getElementValueAt(index) == "BINDING_TYPE_CHARACTER_PROPERTY"){
                            bindings.last().bindingType = hkBinding::BINDING_TYPE_CHARACTER_PROPERTY;
                        }else{
                            writeToLog("hkbVariableBindingSet: readData()!\n'bindingType' data field contains an invalid string!\nObject Reference: "+ref);
                        }
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "indexOfBindingToEnable"){
            indexOfBindingToEnable = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbVariableBindingSet: readData()!\nFailed to properly read 'indexOfBindingToEnable' data field!\nObject Reference: "+ref);
            }
        }
        index++;
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
