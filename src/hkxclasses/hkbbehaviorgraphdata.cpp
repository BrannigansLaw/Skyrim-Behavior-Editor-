#include "hkbbehaviorgraphdata.h"
#include "hkbbehaviorgraphstringdata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbBehaviorGraphData
*/

uint hkbBehaviorGraphData::refCount = 0;

QStringList hkbBehaviorGraphData::Type = {
    "VARIABLE_TYPE_BOOL",
    "VARIABLE_TYPE_INT8",
    "VARIABLE_TYPE_INT16",
    "VARIABLE_TYPE_INT32",
    "VARIABLE_TYPE_REAL",
    "VARIABLE_TYPE_POINTER",
    "VARIABLE_TYPE_VECTOR4",
    "VARIABLE_TYPE_QUATERNION"
};

hkbBehaviorGraphData::hkbBehaviorGraphData(BehaviorFile *parent/*, qint16 ref*/)
    : HkxObject(parent/*, ref*/)
{
    setType(HKB_BEHAVIOR_GRAPH_DATA, TYPE_OTHER);
    refCount++;
}

bool hkbBehaviorGraphData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableInfos"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index += 4;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                hkVariableInfo temp;
                temp.role.role = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.role.flags = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.type = reader.getElementValueAt(index);
                variableInfos.append(temp);
                if (!Type.contains(temp.type)){
                    writeToLog("hkbBehaviorGraphData: readData()!\nInvalid variable type read!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "characterPropertyInfos"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index += 4;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                hkVariableInfo temp;
                temp.role.role = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.role.flags = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.type = reader.getElementValueAt(index);
                characterPropertyInfos.append(temp);
                if (!Type.contains(temp.type)){
                    writeToLog("hkbBehaviorGraphData: readData()!\nInvalid variable type read!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "eventInfos"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            /*index++;
            if (index >= reader.getNumElements()){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }*/
            for (int i = 0; i < numElems; i++){
                index += 2;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventInfos.append(reader.getElementValueAt(index));
                if (eventInfos.last() == ""){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "wordMinVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            if (index >= reader.getNumElements()){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordMinVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "wordMaxVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            if (index >= reader.getNumElements()){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordMaxVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "variableInitialValues"){
            if (!variableInitialValues.readReference(index, reader)){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'variableInitialValues' reference!\nObject Reference: "+ref);
            }
        }else if (text == "stringData"){
            if (!stringData.readReference(index, reader)){
                writeToLog("hkbBehaviorGraphData: readData()!\nFailed to properly read 'stringData' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraphData::link(){
    if (!getParentFile()){
        return false;
    }
    //variableInitialValues
    HkxObjectExpSharedPtr *ptr = &getParentFile()->variableValues;
    if (!ptr){
        writeToLog("hkbBehaviorGraphData: link()!\nFailed to properly link 'variableInitialValues' data field!\n");
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_VARIABLE_VALUE_SET){
        writeToLog("hkbBehaviorGraphData: link()!\n'variableInitialValues' data field is linked to invalid child!\n");
        setDataValidity(false);
        variableInitialValues = *ptr;
    }else{
        variableInitialValues = *ptr;
    }
    //stringData
    ptr = &getParentFile()->stringData;
    if (!ptr){
        writeToLog("hkbBehaviorGraphData: link()!\nFailed to properly link 'stringData' data field!\n");
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH_STRING_DATA){
        writeToLog("hkbBehaviorGraphData: link()!\n'stringData' data field is linked to invalid child!\n");
        setDataValidity(false);
        stringData = *ptr;
    }else{
        stringData = *ptr;
    }
    return true;
}

QStringList & hkbBehaviorGraphData::getVariableNames() const{
    return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->variableNames;
}

hkbBehaviorGraphData::~hkbBehaviorGraphData(){
    refCount--;
}
