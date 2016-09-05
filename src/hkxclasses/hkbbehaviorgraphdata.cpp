#include "hkbbehaviorgraphdata.h"
#include "hkbbehaviorgraphstringdata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

#include "src/hkxclasses/hkbvariablevalueset.h"

/*
 * CLASS: hkbBehaviorGraphData
*/

uint hkbBehaviorGraphData::refCount = 0;

QString hkbBehaviorGraphData::classname = "hkbBehaviorGraphData";

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

QString hkbBehaviorGraphData::getClassname(){
    return classname;
}

void hkbBehaviorGraphData::addVariable(hkVariableType type, const QString & name){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    hkVariableInfo varInfo;
    switch (type){
    case VARIABLE_TYPE_BOOL:
        varInfo.type = "VARIABLE_TYPE_BOOL";
        break;
    case VARIABLE_TYPE_INT8:
        varInfo.type = "VARIABLE_TYPE_INT8";
        break;
    case VARIABLE_TYPE_INT16:
        varInfo.type = "VARIABLE_TYPE_INT16";
        break;
    case VARIABLE_TYPE_INT32:
        varInfo.type = "VARIABLE_TYPE_INT32";
        break;
    case VARIABLE_TYPE_REAL:
        varInfo.type = "VARIABLE_TYPE_REAL";
        break;
    case VARIABLE_TYPE_POINTER:
        varInfo.type = "VARIABLE_TYPE_POINTER";
        varData->variantVariableValues.append(HkxObjectExpSharedPtr());
        break;
    case VARIABLE_TYPE_VECTOR4:
        varInfo.type = "VARIABLE_TYPE_VECTOR4";
        varData->quadVariableValues.append(hkQuadVariable());
        break;
    case VARIABLE_TYPE_QUATERNION:
        varInfo.type = "VARIABLE_TYPE_QUATERNION";
        varData->quadVariableValues.append(hkQuadVariable());
        break;
    default:
        return;
    }
    strData->variableNames.append(name);
    varData->wordVariableValues.append(0);
    variableInfos.append(varInfo);
}

void hkbBehaviorGraphData::removeVariable(int index){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    int count = -1;
    if (index < strData->variableNames.size() && index < varData->wordVariableValues.size() && index < variableInfos.size()){
        if (variableInfos.at(index).type == "VARIABLE_TYPE_POINTER"){
            for (int i = 0; i <= index; i++){
                if (variableInfos.at(i).type == "VARIABLE_TYPE_POINTER"){
                    count++;
                }
            }
            if (count < varData->variantVariableValues.size() && count > -1){
                varData->variantVariableValues.removeAt(count);
            }
        }else if (variableInfos.at(index).type == "VARIABLE_TYPE_VECTOR4" || variableInfos.at(index).type == "VARIABLE_TYPE_QUATERNION"){
            for (int i = 0; i <= index; i++){
                if (variableInfos.at(i).type == "VARIABLE_TYPE_POINTER" || variableInfos.at(i).type == "VARIABLE_TYPE_QUATERNION"){
                    count++;
                }
            }
            if (count < varData->quadVariableValues.size() && count > -1){
                varData->quadVariableValues.removeAt(count);
            }
        }
        strData->variableNames.removeAt(index);
        varData->wordVariableValues.removeAt(index);
        variableInfos.removeAt(index);
    }
}

void hkbBehaviorGraphData::setVariableNameAt(int index, const QString & name){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (strData->variableNames.size() > index && index > -1){
        strData->variableNames.replace(index, name);
    }
}

void hkbBehaviorGraphData::setWordVariableValueAt(int index, int value){
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    if (varData->wordVariableValues.size() > index && index > -1){
        varData->wordVariableValues.replace(index, value);
    }
}

void hkbBehaviorGraphData::setQuadVariableValueAt(int index, hkQuadVariable value){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    int count = -1;
    if (index < strData->variableNames.size() && index < varData->wordVariableValues.size() && index < variableInfos.size()){
        if (variableInfos.at(index).type == "VARIABLE_TYPE_VECTOR4" || variableInfos.at(index).type == "VARIABLE_TYPE_QUATERNION"){
            for (int i = 0; i <= index; i++){
                if (variableInfos.at(i).type == "VARIABLE_TYPE_VECTOR4" || variableInfos.at(i).type == "VARIABLE_TYPE_QUATERNION"){
                    count++;
                }
            }
            if (count < varData->quadVariableValues.size() && count > -1){
                varData->quadVariableValues.replace(count, value);
            }
        }else{
            return;
        }
    }
}

hkQuadVariable hkbBehaviorGraphData::getQuadVariable(int index, bool *ok){
    *ok = false;
    hkbVariableValueSet *variableValues = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    if (variableValues->wordVariableValues.size() > index && variableInfos.size() > index){
        int count = -1;
        for (int i = 0; i <= index; i++){
            if (variableInfos.at(i).type == "VARIABLE_TYPE_VECTOR4" || variableInfos.at(i).type == "VARIABLE_TYPE_QUATERNION"){
                count++;
            }
        }
        if (count < variableValues->quadVariableValues.size() && count > -1){
            *ok = true;
            return variableValues->quadVariableValues.at(count);
        }
    }
    return hkQuadVariable();
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

bool hkbBehaviorGraphData::evaulateDataValidity(){
    if (!variableInitialValues.data() || variableInitialValues.data()->getSignature() != HKB_VARIABLE_VALUE_SET){
        setDataValidity(false);
        return false;
    }else if (!stringData.data() || stringData.data()->getSignature() != HKB_BEHAVIOR_GRAPH_STRING_DATA){
        setDataValidity(false);
        return false;
    }
    //Check other data...
    setDataValidity(true);
    return true;
}

hkbBehaviorGraphData::~hkbBehaviorGraphData(){
    refCount--;
}
