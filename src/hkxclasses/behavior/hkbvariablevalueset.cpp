#include "hkbvariablevalueset.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbVariableValueSet
*/

uint hkbVariableValueSet::refCount = 0;

QString hkbVariableValueSet::classname = "hkbVariableValueSet";

hkbVariableValueSet::hkbVariableValueSet(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_VARIABLE_VALUE_SET, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
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
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            index += 2;
            numElems = numElems*2 + index;
            for (; index < numElems; index += 2){
                if (index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
                wordVariableValues.append(reader.getElementValueAt(index).toInt(&ok));
                if (!ok){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'wordVariableValues' data!\nObject Reference: "+ref, true);
                    return false;
                }
            }
            index--;
            continue;
        }else if (text == "quadVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'quadVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readMultipleVector4(reader.getElementValueAt(index), quadVariableValues)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'quadVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
        }else if (text == "variantVariableValues"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variantVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readReferences(reader.getElementValueAt(index), variantVariableValues)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variantVariableValues' data!\nObject Reference: "+ref, true);
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbVariableValueSet::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"wordVariableValues", QString::number(wordVariableValues.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < wordVariableValues.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("value"), QString::number(wordVariableValues.at(i)));
            writer->writeLine(writer->object, false);
        }
        if (wordVariableValues.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"quadVariableValues", QString::number(quadVariableValues.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < quadVariableValues.size(); i++){
            writer->writeLine(quadVariableValues[i].getValueAsString());
        }
        if (quadVariableValues.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"variantVariableValues", QString::number(variantVariableValues.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        QString refs;
        for (int i = 0; i < variantVariableValues.size(); i++){
            if (variantVariableValues.at(i).data()){
                refs = refs + variantVariableValues.at(i).data()->getReferenceString()+" ";
            }
        }
        writer->writeLine(refs);
        if (variantVariableValues.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbVariableValueSet::link(){
    if (!getParentFile()){
        return false;
    }
    HkxObjectExpSharedPtr *ptr = NULL;
    for (int i = 0; i < variantVariableValues.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(variantVariableValues.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'variantVariableValues' data field!\n");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog(getClassname()+": link()!\n'variantVariableValues' data field is linked to invalid child!\n");
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
