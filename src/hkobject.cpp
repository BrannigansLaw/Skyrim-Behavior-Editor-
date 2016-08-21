#include "hkobject.h"
#include "hkxxmlreader.h"
#include "generators.h"
#include "hkxfile.h"

/**
 * HkObject
 */

void HkObject::writeToLog(const QString & message, bool isError){
    if (getParentFile()){
        getParentFile()->writeToLog(message, isError);
    }
}

void HkObject::setProgressData(const QString & message, int value){
    if (getParentFile()){
        getParentFile()->setProgressData(message, value);
    }
}

QList <HkObjectExpSharedPtr> HkObject::getChildGenerators() const{
    qulonglong sig = getSignature();
    QList <HkObjectExpSharedPtr> children;
    switch (sig) {
    case HKB_STATE_MACHINE_STATE_INFO:
        //children.append(HkObjectExpSharedPtr(reinterpret_cast<hkbStateMachineStateInfo const *>(this)->generator));
        break;
    /*case HKB_STATE_MACHINE:
        if (!reinterpret_cast<hkbStateMachine *>(this)->link()){
            return false;
        }
        break;
    case HKB_MANUAL_SELECTOR_GENERATOR:
        if (!reinterpret_cast<hkbManualSelectorGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_BLENDER_GENERATOR_CHILD:
        if (!reinterpret_cast<hkbBlenderGeneratorChild *>(this)->link()){
            return false;
        }
        break;
    case HKB_BLENDER_GENERATOR:
        if (!reinterpret_cast<hkbBlenderGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_I_STATE_TAGGING_GENERATOR:
        if (!reinterpret_cast<BSiStateTaggingGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_BONE_SWITCH_GENERATOR:
        if (!reinterpret_cast<BSBoneSwitchGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_BONE_SWITCH_GENERATOR_BONE_DATA:
        if (!reinterpret_cast<BSBoneSwitchGeneratorBoneData *>(this)->link()){
            return false;
        }
        break;
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        if (!reinterpret_cast<BSCyclicBlendTransitionGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
        if (!reinterpret_cast<BSSynchronizedClipGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_MODIFIER_GENERATOR:
        if (!reinterpret_cast<hkbModifierGenerator *>(this)->link()){
            return false;
        }
        break;
    case BS_OFFSET_ANIMATION_GENERATOR:
        if (!reinterpret_cast<BSOffsetAnimationGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_POSE_MATCHING_GENERATOR:
        if (!reinterpret_cast<hkbPoseMatchingGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_CLIP_GENERATOR:
        if (!reinterpret_cast<hkbClipGenerator *>(this)->link()){
            return false;
        }
        break;
    case HKB_BEHAVIOR_GRAPH:
        if (!reinterpret_cast<hkbBehaviorGraph *>(this)->link()){
            return false;
        }
        break;*/
    default:
        break;
    }
    return children;
}

bool HkObject::readReferences(const QByteArray &line, QList <HkObjectExpSharedPtr> & children){
    qint16 size = 0;
    qint16 start;
    bool ok = false;
    for (qint16 i = 0; i < line.size(); i++){
        if (line.at(i) == '#'){
            i++;
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            children.append(HkObjectExpSharedPtr(NULL, value.toLong(&ok)));
            if (!ok){
                return false;
            }
        }else if (line.at(i) == 'n'){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            if (value == "null"){
                ok = true;
            } else {
                return false;
            }
        }
    }
    return ok;
}

bool HkObject::readIntegers(const QByteArray &line, QVector<qint16> & ints){
    qint16 size = 0;
    qint16 start;
    bool ok = true;
    for (qint16 i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            ints.append(value.toInt(&ok));
            if (!ok){
                return false;
            }
        }
    }
    return ok;
}

bool HkObject::toBool(const QByteArray &line, bool *ok){
    *ok = true;
    if (line == "true"){
        return true;
    }else if (line == "false"){
        return false;
    }else {
        *ok = false;
        return false;
    }
}

bool HkObject::readDoubles(const QByteArray &line, QVector<qreal> & doubles){
    qint16 size = 0;
    qint16 start;
    bool ok = false;
    for (qint16 i = 0; i < line.size(); i++){
        if ((line.at(i) >= '0' && line.at(i) <= '9') || (line.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < line.size() && line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = line[start];
                start++;
            }
            doubles.append(value.toDouble(&ok));
            if (!ok){
                return false;
            }
        }
    }
    return ok;
}

hkVector3 HkObject::readVector3(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3};
    qint16 size = 0;
    qint16 start;
    ushort axisVar = 0;
    hkVector3 vector3;
    for (qint16 i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = lineIn[start];
                start++;
            }
            axisVar++;
            switch (axisVar){
            case X:
                vector3.x = value.toDouble(ok);
                break;
            case Y:
                vector3.y = value.toDouble(ok);
                break;
            case Z:
                vector3.z = value.toDouble(ok);
                if (lineIn.at(i) != ')'){
                    *ok = false;
                    return hkVector3();
                }
                break;
            default:
                *ok = false;
                return hkVector3();
            }
            if (!*ok){
                return hkVector3();
            }
        }
    }
    return vector3;
}

hkVector4 HkObject::readVector4(const QByteArray &lineIn, bool *ok){
    enum {X = 1, Y = 2, Z = 3, W = 4};
    qint16 size = 0;
    qint16 start;
    ushort axisVar = 0;
    hkVector4 vector;
    for (qint16 i = 0; i < lineIn.size(); i++){
        if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
            start = i;
            size = 0;
            do{
                size++;
                i++;
            }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
            QByteArray value(size, '\0');
            for (qint16 j = 0; j < size; j++){
                value[j] = lineIn[start];
                start++;
            }
            axisVar++;
            switch (axisVar){
            case X:
                vector.x = value.toDouble(ok);
                break;
            case Y:
                vector.y = value.toDouble(ok);
                break;
            case Z:
                vector.z = value.toDouble(ok);
                break;
            case W:
                vector.w = value.toDouble(ok);
                if (lineIn.at(i) != ')'){
                    *ok = false;
                    return hkVector4();
                }
                break;
            default:
                *ok = false;
                return hkVector4();
            }
            if (!*ok){
                return hkVector4();
            }
        }
    }
    return vector;
}

bool HkObject::readMultipleVector4(const QByteArray &lineIn,  QVector <hkVector4> & vectors){
    enum {X = 1, Y = 2, Z = 3, W = 4};
    qint16 size = 0;
    qint16 start;
    ushort axisVar = 0;
    hkVector4 vector;
    bool ok = false;
    bool loop = false;
    for (qint16 i = 0; i < lineIn.size(); i++){
        if (lineIn.at(i) == '('){
            loop = true;
            i++;
            while (loop && i < lineIn.size()){
                if ((lineIn.at(i) >= '0' && lineIn.at(i) <= '9') || (lineIn.at(i) == '-')){
                    start = i;
                    size = 0;
                    do{
                        size++;
                        i++;
                    }while (i < lineIn.size() && lineIn.at(i) != ' ' && lineIn.at(i) != ')');
                    QByteArray value(size, '\0');
                    for (qint16 j = 0; j < size; j++){
                        value[j] = lineIn[start];
                        start++;
                    }
                    axisVar++;
                    switch (axisVar){
                    case X:
                        vector.x = value.toDouble(&ok);
                        break;
                    case Y:
                        vector.y = value.toDouble(&ok);
                        break;
                    case Z:
                        vector.z = value.toDouble(&ok);
                        break;
                    case W:
                        vector.w = value.toDouble(&ok);
                        if (!ok){
                            return false;
                        }
                        loop = false;
                        vectors.append(vector);
                        axisVar = 0;
                        if (lineIn.at(i) != ')'){
                            return false;
                        }
                        break;
                    default:
                        return false;
                    }
                }
                i++;
            }
        }
    }
    return ok;
}

/*
 * CLASS: HkObjectExpSharedPtr
*/

bool HkObjectExpSharedPtr::readReference(long index, const HkxXmlReader & reader){
    bool ok = true;
    //need to remove the '#' from the reference string
    QByteArray temp = reader.getElementValueAt(index);
    if (temp.at(0) == '#'){
        temp.remove(0, 1);
    }
    if (temp == "null"){
        setReference(-1);
    }else{
        setReference(temp.toLong(&ok));
    }
    if (!ok){
        return false;
    }
    return true;
}

/*
 * CLASS: hkbBehaviorGraphStringData
*/

uint hkbBehaviorGraphStringData::refCount = 0;

hkbBehaviorGraphStringData::hkbBehaviorGraphStringData(BehaviorFile *parent/*, qint16 ref*/)
    : HkObject(parent/*, ref*/)
{
    setType(HKB_BEHAVIOR_GRAPH_STRING_DATA, TYPE_OTHER);
    refCount++;
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
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "attributeNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'attributeNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                attributeNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "variableNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'variableNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                variableNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "characterPropertyNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog("hkbBehaviorGraphStringData: readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref, true);
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

/*
 * CLASS: hkbVariableValueSet
*/

uint hkbVariableValueSet::refCount = 0;

hkbVariableValueSet::hkbVariableValueSet(BehaviorFile *parent/*, qint16 ref*/)
    : HkObject(parent/*, ref*/)
{
    setType(HKB_VARIABLE_VALUE_SET, TYPE_OTHER);
    refCount++;
}

bool hkbVariableValueSet::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "wordVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index += 2;
            numElems = numElems*2 + index;
            for (; index < numElems; index += 2){
                if (index >= reader.getNumElements()){
                    writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
            index--;
            continue;
        }else if (text == "quadVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'quadVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readMultipleVector4(reader.getElementValueAt(index), quadVariableValues)){
                writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'quadVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
        }else if (text == "variantVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'variantVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readReferences(reader.getElementValueAt(index), variantVariableValues)){
                writeToLog("hkbVariableValueSet: readData()!\nFailed to properly read 'variantVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbVariableValueSet::link(){
    if (!getParentFile()){
        return false;
    }
    //variantVariableValues
    HkObjectExpSharedPtr *ptr = NULL;
    for (int i = 0; i < variantVariableValues.size(); i++){
        //variantVariableValues
        ptr = getParentFile()->findHkObject(variantVariableValues.at(i).getReference());
        if (!ptr){
            writeToLog("hkbVariableValueSet: link()!\nFailed to properly link 'variantVariableValues' data field!\n");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog("hkbVariableValueSet: link()!\n'variantVariableValues' data field is linked to invalid child!\n");
            setDataValidity(false);
            variantVariableValues[i] = *ptr;
        }else{
            variantVariableValues[i] = *ptr;
        }
    }
    return true;
}

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
    : HkObject(parent/*, ref*/)
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
    HkObjectExpSharedPtr *ptr = &getParentFile()->variableValues;
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

/**
 * hkbStateMachineEventPropertyArray
 */
/*
uint hkbStateMachineEventPropertyArray::refCount = 0;

bool hkbStateMachineEventPropertyArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "events"){
            int numEvents = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numEvents; j++){
                events.append(HkEvent());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "id"){
                        events.last().id = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "payload"){
                        if (!events.last().payload.readReference(index, reader)){
                            return false;
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

bool hkbStateMachineEventPropertyArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < events.size(); i++){
        //payload
        ptr = getParentFile()->findHkObject(events.at(i).payload.getReference());
        if (ptr){
            if ((*ptr)->getType() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            events[i].payload = *ptr;
        }
    }
    return true;
}*/

/**
 * hkbStateMachineTransitionInfoArray
 */
/*
uint hkbStateMachineTransitionInfoArray::refCount = 0;

bool hkbStateMachineTransitionInfoArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "transitions"){
            int numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                transitions.append(HkTransition());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "transition"){
                        if (!transitions.last().transition.readReference(index, reader)){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "condition"){
                        if (!transitions.last().condition.readReference(index, reader)){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "eventId"){
                        transitions.last().eventId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toStateId"){
                        transitions.last().toStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fromNestedStateId"){
                        transitions.last().fromNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "toNestedStateId"){
                        transitions.last().toNestedStateId = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "priority"){
                        transitions.last().priority = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if(reader.getNthAttributeValueAt(index, 0) == "flags"){
                        transitions.last().flags = reader.getElementValueAt(index);
                        if (transitions.last().flags == ""){
                            return false;
                        }
                        //index++;
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

bool hkbStateMachineTransitionInfoArray::link(){
    if (!getParentFile()){
        return false;
    }
    HkObjectExpSharedPtr *ptr;
    for (int i = 0; i < transitions.size(); i++){
        //transition
        ptr = getParentFile()->findHkObject(transitions.at(i).transition.getReference());
        if (ptr){
            if ((*ptr)->getType() != HKB_BLENDING_TRANSITION_EFFECT || (*ptr)->getType() != HKB_GENERATOR_TRANSITION_EFFECT){
                return false;
            }
            transitions[i].transition = *ptr;
        }
        //condition
        ptr = getParentFile()->findHkObject(transitions.at(i).condition.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_EXPRESSION_CONDITION){
                return false;
            }
            transitions[i].condition = *ptr;
        }
    }
    return true;
}*/

/**
 * hkbStringEventPayload
 */

uint hkbStringEventPayload::refCount = 0;

/**
 * HkDynamicObject
 */

bool HkDynamicObject::linkVar(){
    if (!getParentFile()){
        return false;
    }
    HkObjectExpSharedPtr *ptr = getParentFile()->findHkObject(variableBindingSet.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_VARIABLE_BINDING_SET){
            getParentFile()->writeToLog("HkDynamicObject: linkVar()!\nThe linked object is not a HKB_VARIABLE_BINDING_SET!\nRemoving the link to the invalid object!");
            variableBindingSet = NULL;
            return false;
        }
        variableBindingSet = *ptr;
    }
    return true;
}

/*
 * CLASS: hkbBoneWeightArray
*/

uint hkbBoneWeightArray::refCount = 0;

hkbBoneWeightArray::hkbBoneWeightArray(BehaviorFile *parent/*, qint16 ref*/)
    : HkDynamicObject(parent/*, ref*/)
{
    setType(HKB_BONE_WEIGHT_ARRAY, TYPE_OTHER);
    refCount++;
}

bool hkbBoneWeightArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBoneWeightArray: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "boneWeights"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog("hkbBoneWeightArray: readData()!\nFailed to properly read 'boneWeights' data!\nObject Reference: "+ref, true);
                return false;
            }
            //index++;
            if (numElems > 0 && !readDoubles(reader.getElementValueAt(index), boneWeights)){
                writeToLog("hkbBoneWeightArray: readData()!\nFailed to properly read 'boneWeights' data!\nObject Reference: "+ref, true);
                return false;
            }
            //continue;
        }
        index++;
    }
    return true;
}

bool hkbBoneWeightArray::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!this->linkVar()){
        writeToLog("hkbBoneWeightArray: link()!\nFailed to properly link 'variableBindingSet' data field!\n");
    }
    return true;
}
