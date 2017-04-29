#include "hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBehaviorGraphStringData
*/

uint hkbBehaviorGraphStringData::refCount = 0;

QString hkbBehaviorGraphStringData::classname = "hkbBehaviorGraphStringData";

hkbBehaviorGraphStringData::hkbBehaviorGraphStringData(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_BEHAVIOR_GRAPH_STRING_DATA, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbBehaviorGraphStringData::getClassname(){
    return classname;
}

bool hkbBehaviorGraphStringData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "eventNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "attributeNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                attributeNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "variableNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                variableNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "characterPropertyNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                characterPropertyNames.append(reader.getElementValueAt(index));
            }
            continue;
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraphStringData::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        QStringList list3 = {writer->name, writer->numelements};
        QStringList list4 = {"eventNames", QString::number(eventNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < eventNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), eventNames.at(i));
        }
        if (eventNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"attributeNames", QString::number(attributeNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < attributeNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), attributeNames.at(i));
        }
        if (attributeNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"variableNames", QString::number(variableNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < variableNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), variableNames.at(i));
        }
        if (variableNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"characterPropertyNames", QString::number(characterPropertyNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < characterPropertyNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), characterPropertyNames.at(i));
        }
        if (characterPropertyNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

QString hkbBehaviorGraphStringData::getVariableNameAt(int index) const{
    if (index >= 0 && index < variableNames.size()){
        return variableNames.at(index);
    }
    return "";
}

QString hkbBehaviorGraphStringData::getEventNameAt(int index) const{
    if (index >= 0 && index < eventNames.size()){
        return eventNames.at(index);
    }
    return "";
}

bool hkbBehaviorGraphStringData::link(){
    return true;
}

bool hkbBehaviorGraphStringData::evaulateDataValidity(){
    for (int i = 0; i < eventNames.size(); i++){
        if (eventNames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < attributeNames.size(); i++){
        if (attributeNames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < variableNames.size(); i++){
        if (variableNames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < characterPropertyNames.size(); i++){
        if (characterPropertyNames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbBehaviorGraphStringData::~hkbBehaviorGraphStringData(){
    refCount--;
}
