#include "hkbvariablevalueset.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbVariableValueSet
*/

uint hkbVariableValueSet::refCount = 0;

QString hkbVariableValueSet::classname = "hkbVariableValueSet";

hkbVariableValueSet::hkbVariableValueSet(BehaviorFile *parent/*, qint16 ref*/)
    : HkxObject(parent/*, ref*/)
{
    setType(HKB_VARIABLE_VALUE_SET, TYPE_OTHER);
    refCount++;
}

QString hkbVariableValueSet::getClassname(){
    return classname;
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
    HkxObjectExpSharedPtr *ptr = NULL;
    for (int i = 0; i < variantVariableValues.size(); i++){
        //variantVariableValues
        ptr = getParentFile()->findHkxObject(variantVariableValues.at(i).getReference());
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

bool hkbVariableValueSet::evaulateDataValidity(){
    for (int i = 0; i < variantVariableValues.size(); i++){
        if (!variantVariableValues.at(i).data() || variantVariableValues.at(i).data()->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkbVariableValueSet::~hkbVariableValueSet(){
    refCount--;
}
