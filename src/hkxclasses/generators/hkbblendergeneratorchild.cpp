#include "hkbblendergeneratorchild.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

QString hkbBlenderGeneratorChild::classname = "hkbBlenderGeneratorChild";

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
      weight(0),
      worldFromModelWeight(0)
{
    setType(HKB_BLENDER_GENERATOR_CHILD, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbBlenderGeneratorChild::getClassname(){
    return classname;
}

bool hkbBlenderGeneratorChild::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "boneWeights"){
            if (!boneWeights.readReference(index, reader)){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'boneWeights' reference!\nObject Reference: "+ref);
            }
        }else if (text == "weight"){
            weight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'weight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelWeight"){
            worldFromModelWeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbBlenderGeneratorChild: readData()!\nFailed to properly read 'worldFromModelWeight' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBlenderGeneratorChild::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbBlenderGeneratorChild: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //boneWeights
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(boneWeights.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog("hkbBlenderGeneratorChild: link()!\n'boneWeights' data field is linked to invalid child!");
            setDataValidity(false);
        }
        boneWeights = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog("hkbBlenderGeneratorChild: link()!\nFailed to properly link 'generator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("hkbBlenderGeneratorChild: link()!\n'generator' data field is linked to invalid child!");
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

void hkbBlenderGeneratorChild::unlink(){
    HkDynamicObject::unlink();
    generator = HkxObjectExpSharedPtr();
    boneWeights = HkxObjectExpSharedPtr();
}

bool hkbBlenderGeneratorChild::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (boneWeights.data() && boneWeights.data()->getSignature() != HKB_BONE_WEIGHT_ARRAY){
    }else if (!generator.data() || generator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbBlenderGeneratorChild::~hkbBlenderGeneratorChild(){
    refCount--;
}
