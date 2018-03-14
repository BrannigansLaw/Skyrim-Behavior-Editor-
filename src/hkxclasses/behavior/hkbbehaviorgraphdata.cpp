#include "hkbbehaviorgraphdata.h"
#include "hkbbehaviorgraphstringdata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

#include "src/hkxclasses/behavior/hkbvariablevalueset.h"

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

hkbBehaviorGraphData::hkbBehaviorGraphData(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_BEHAVIOR_GRAPH_DATA, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbBehaviorGraphData::getClassname(){
    return classname;
}

int hkbBehaviorGraphData::addVariable(hkVariableType type, const QString & name, bool isProperty){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    int index = -1;
    bool varAdded = false;
    if (name != "" /* && (isProperty || !strData->variableNames.contains(name))*/){
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
            break;
        case VARIABLE_TYPE_VECTOR4:
            varInfo.type = "VARIABLE_TYPE_VECTOR4";
            break;
        case VARIABLE_TYPE_QUATERNION:
            varInfo.type = "VARIABLE_TYPE_QUATERNION";
            break;
        default:
            return -2;
        }
        if (isProperty){
            index = strData->characterPropertyNames.indexOf(name);
            if (index == -1){
                strData->characterPropertyNames.append(name);
                characterPropertyInfos.append(varInfo);
                index = strData->characterPropertyNames.size() - 1;
                varAdded = true;
            }
        }else{
            index = strData->variableNames.indexOf(name);
            if (index == -1){
                strData->variableNames.append(name);
                variableInfos.append(varInfo);
                varData->wordVariableValues.append(0);
                index = strData->variableNames.size() - 1;
                varAdded = true;
            }
        }
        if (varAdded){
            if (type > VARIABLE_TYPE_POINTER){
                varData->quadVariableValues.append(hkQuadVariable());
            }else if (type == VARIABLE_TYPE_POINTER){
                //varData->variantVariableValues.append(HkxSharedPtr());
            }
        }
    }
    return index;
}

void hkbBehaviorGraphData::addVariable(hkVariableType type){
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
        varData->variantVariableValues.append(HkxSharedPtr());
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
    QString name = "NEW_"+varInfo.type+"_";
    int num = 0;
    int index;
    for (int i = 0; i < strData->variableNames.size(); i++){
        if (strData->variableNames.at(i) == name){
            index = name.lastIndexOf('_');
            if (index > -1){
                name.remove(index + 1, name.size());
            }
            name.append(QString::number(num));
            num++;
            if (num > 1){
                i = 0;
            }
        }
    }
    strData->variableNames.append(name);
    varData->wordVariableValues.append(0);
    variableInfos.append(varInfo);
}

void hkbBehaviorGraphData::removeVariable(int index){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    hkbVariableValueSet *varData = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    int count = -1;
    if (index < strData->variableNames.size() && index < varData->wordVariableValues.size() && index < variableInfos.size() && index > -1){
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
                if (variableInfos.at(i).type == "VARIABLE_TYPE_VECTOR4" || variableInfos.at(i).type == "VARIABLE_TYPE_QUATERNION"){
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

void hkbBehaviorGraphData::addEvent(const QString &name){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    strData->eventNames.append(name);
    eventInfos.append("0");
}

void hkbBehaviorGraphData::addEvent(){
    QString name = "NEW_Event_";
    int num = 1;
    int index;
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    for (int i = 0; i < strData->eventNames.size(); i++){
        if (strData->eventNames.at(i) == name){
            index = name.lastIndexOf('_');
            if (index > -1){
                name.remove(index + 1, name.size());
            }
            name.append(QString::number(num));
            num++;
            if (num > 1){
                i = 0;
            }
        }
    }
    strData->eventNames.append(name);
    eventInfos.append("0");
}

void hkbBehaviorGraphData::removeEvent(int index){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (index < strData->eventNames.size() && index < eventInfos.size() && index > -1){
        strData->eventNames.removeAt(index);
        eventInfos.removeAt(index);
    }
}

void hkbBehaviorGraphData::setEventNameAt(int index, const QString & name){
    hkbBehaviorGraphStringData *strData = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (strData->eventNames.size() > index && index > -1){
        strData->eventNames.replace(index, name);
    }
}

void hkbBehaviorGraphData::setEventFlagAt(int index, bool state){
    if (eventInfos.size() > index && index > -1){
        if (state){
            eventInfos.replace(index, "FLAG_SYNC_POINT");
        }else{
            eventInfos.replace(index, "0");
        }
    }
}

hkVariableType hkbBehaviorGraphData::getVariableTypeAt(int index) const{
    if (variableInfos.size() > index && index > -1){
        QString type = variableInfos.at(index).type;
        if (type == "VARIABLE_TYPE_BOOL"){
            return VARIABLE_TYPE_BOOL;
        }else if (type == "VARIABLE_TYPE_INT8"){
            return VARIABLE_TYPE_INT8;
        }else if (type == "VARIABLE_TYPE_INT16"){
            return VARIABLE_TYPE_INT16;
        }else if (type == "VARIABLE_TYPE_INT32"){
            return VARIABLE_TYPE_INT32;
        }else if (type == "VARIABLE_TYPE_REAL"){
            return VARIABLE_TYPE_REAL;
        }else if (type == "VARIABLE_TYPE_POINTER"){
            return VARIABLE_TYPE_POINTER;
        }else if (type == "VARIABLE_TYPE_VECTOR4"){
            return VARIABLE_TYPE_VECTOR4;
        }else if (type == "VARIABLE_TYPE_QUATERNION"){
            return VARIABLE_TYPE_QUATERNION;
        }
    }
    return VARIABLE_TYPE_INT8;
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

hkQuadVariable hkbBehaviorGraphData::getQuadVariable(int index, bool *ok) const{
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

HkxObject * hkbBehaviorGraphData::getVariantVariable(int index) const{
    hkbVariableValueSet *variableValues = static_cast<hkbVariableValueSet *>(variableInitialValues.data());
    if (variableValues->wordVariableValues.size() > index && variableInfos.size() > index){
        int count = -1;
        for (int i = 0; i <= index; i++){
            if (variableInfos.at(i).type == "VARIABLE_TYPE_POINTER"){
                count++;
            }
        }
        if (count < variableValues->variantVariableValues.size() && count > -1){
            return variableValues->variantVariableValues.at(count).data();
        }
    }
    return nullptr;
}

hkVariableType hkbBehaviorGraphData::getCharacterPropertyTypeAt(int index) const{
    if (characterPropertyInfos.size() > index && index > -1){
        QString type = characterPropertyInfos.at(index).type;
        if (type == "VARIABLE_TYPE_BOOL"){
            return VARIABLE_TYPE_BOOL;
        }else if (type == "VARIABLE_TYPE_INT8"){
            return VARIABLE_TYPE_INT8;
        }else if (type == "VARIABLE_TYPE_INT16"){
            return VARIABLE_TYPE_INT16;
        }else if (type == "VARIABLE_TYPE_INT32"){
            return VARIABLE_TYPE_INT32;
        }else if (type == "VARIABLE_TYPE_REAL"){
            return VARIABLE_TYPE_REAL;
        }else if (type == "VARIABLE_TYPE_POINTER"){
            return VARIABLE_TYPE_POINTER;
        }else if (type == "VARIABLE_TYPE_VECTOR4"){
            return VARIABLE_TYPE_VECTOR4;
        }else if (type == "VARIABLE_TYPE_QUATERNION"){
            return VARIABLE_TYPE_QUATERNION;
        }
    }
    return VARIABLE_TYPE_INT8;
}

QString hkbBehaviorGraphData::getCharacterPropertyNameAt(int index) const{
    hkbBehaviorGraphStringData *strdata = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (strdata){
        if (strdata->characterPropertyNames.size() > index && index > -1){
            return strdata->characterPropertyNames.at(index);
        }
    }
    return "";
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
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index += 4;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                hkVariableInfo temp;
                temp.role.role = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.role.flags = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.type = reader.getElementValueAt(index);
                variableInfos.append(temp);
                if (!Type.contains(temp.type)){
                    writeToLog(getClassname()+": readData()!\nInvalid variable type read!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "characterPropertyInfos"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index += 4;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                hkVariableInfo temp;
                temp.role.role = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.role.flags = reader.getElementValueAt(index);
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPropertyInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                temp.type = reader.getElementValueAt(index);
                characterPropertyInfos.append(temp);
                if (!Type.contains(temp.type)){
                    writeToLog(getClassname()+": readData()!\nInvalid variable type read!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "eventInfos"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            /*index++;
            if (index >= reader.getNumElements()){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }*/
            for (int i = 0; i < numElems; i++){
                index += 2;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventInfos.append(reader.getElementValueAt(index));
                if (eventInfos.last() == ""){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "wordMinVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            if (index >= reader.getNumElements()){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordMinVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMinVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "wordMaxVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            if (index >= reader.getNumElements()){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventInfos' data!\nObject Reference: "+ref, true);
                return false;
            }
            for (int i = 0; i < numElems; i++){
                index++;
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordMaxVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordMaxVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
        }else if (text == "variableInitialValues"){
            if (!variableInitialValues.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableInitialValues' reference!\nObject Reference: "+ref);
            }
        }else if (text == "stringData"){
            if (!stringData.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'stringData' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBehaviorGraphData::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"attributeDefaults", QString::number(/*attributeDefaults.size()*/0)};
        writer->writeLine(writer->object, list1, list2, "");
        /*for (int i = 0; i < attributeDefaults.size(); i++){
            //???
        }
        if (attributeDefaults.size() > 0){
            writer->writeLine(writer->object, false);
        }
        writer->writeLine(writer->object, false);*/
        list1 = {writer->name, writer->numelements};
        list2 = {"variableInfos", QString::number(variableInfos.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < variableInfos.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("role"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("role"), variableInfos.at(i).role.role);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), variableInfos.at(i).role.flags);
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("type"), variableInfos.at(i).type);
            writer->writeLine(writer->object, false);
        }
        if (variableInfos.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"characterPropertyInfos", QString::number(characterPropertyInfos.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < characterPropertyInfos.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("role"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("role"), characterPropertyInfos.at(i).role.role);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), characterPropertyInfos.at(i).role.flags);
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("type"), characterPropertyInfos.at(i).type);
            writer->writeLine(writer->object, false);
        }
        if (characterPropertyInfos.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"eventInfos", QString::number(eventInfos.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < eventInfos.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), eventInfos.at(i));
            writer->writeLine(writer->object, false);
        }
        if (eventInfos.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        if (variableInitialValues.data()){
            refString = variableInitialValues.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableInitialValues"), refString);
        if (stringData.data()){
            refString = stringData.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("stringData"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (!variableInitialValues.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableInitialValues'!!!", true);
        }
        if (!stringData.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'stringData'!!!", true);
        }
    }
    return true;
}

QStringList hkbBehaviorGraphData::getVariableTypeNames() const{
    QStringList list;
    for (int i = 0; i < variableInfos.size(); i++){
        list.append(variableInfos.at(i).type);
    }
    return list;
}

bool hkbBehaviorGraphData::merge(HkxObject *recessiveobj){ //Make sure to update event and variable indices when merging!!!
    //
    //
    //
    //
    return true;
}

bool hkbBehaviorGraphData::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr = &static_cast<BehaviorFile *>(getParentFile())->variableValues;
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableInitialValues' data field!\n");
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_VARIABLE_VALUE_SET){
        writeToLog(getClassname()+": link()!\n'variableInitialValues' data field is linked to invalid child!\n");
        setDataValidity(false);
        variableInitialValues = *ptr;
    }else{
        variableInitialValues = *ptr;
    }
    ptr = &static_cast<BehaviorFile *>(getParentFile())->stringData;
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'stringData' data field!\n");
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_BEHAVIOR_GRAPH_STRING_DATA){
        writeToLog(getClassname()+": link()!\n'stringData' data field is linked to invalid child!\n");
        setDataValidity(false);
        stringData = *ptr;
    }else{
        stringData = *ptr;
    }
    return true;
}

QStringList hkbBehaviorGraphData::getVariableNames() const{
    return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->variableNames;
}

QStringList hkbBehaviorGraphData::getEventNames() const{
    return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->eventNames;
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
