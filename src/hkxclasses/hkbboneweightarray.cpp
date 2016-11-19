#include "hkbboneweightarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBoneWeightArray
*/

uint hkbBoneWeightArray::refCount = 0;

QString hkbBoneWeightArray::classname = "hkbBoneWeightArray";

hkbBoneWeightArray::hkbBoneWeightArray(BehaviorFile *parent, long ref)
    : HkDynamicObject(parent, ref)
{
    setType(HKB_BONE_WEIGHT_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbBoneWeightArray::getClassname(){
    return classname;
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

hkbBoneWeightArray::~hkbBoneWeightArray(){
    refCount--;
}
