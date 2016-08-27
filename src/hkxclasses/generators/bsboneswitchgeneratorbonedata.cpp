#include "bsboneswitchgeneratorbonedata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSBoneSwitchGeneratorBoneData
*/

uint BSBoneSwitchGeneratorBoneData::refCount = 0;

BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/)
{
    refCount++;
    setType(BS_BONE_SWITCH_GENERATOR_BONE_DATA, TYPE_GENERATOR);
}

bool BSBoneSwitchGeneratorBoneData::readData(const HkxXmlReader &reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pGenerator"){
            if (!pGenerator.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'pGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "spBoneWeight"){
            if (!spBoneWeight.readReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'spBoneWeight' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //pGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pGenerator.getReference());
    if (!ptr){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\nFailed to properly link 'pGenerator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\n'pGenerator' data field is linked to invalid child!");
        setDataValidity(false);
        pGenerator = *ptr;
    }else{
        pGenerator = *ptr;
    }
    //spBoneWeight
    ptr = getParentFile()->findHkxObject(spBoneWeight.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog("BSBoneSwitchGeneratorBoneData: link()!\n'spBoneWeight' data field is linked to invalid child!");
            setDataValidity(false);
        }
        spBoneWeight = *ptr;
    }
    return true;
}

void BSBoneSwitchGeneratorBoneData::unlink(){
    HkDynamicObject::unlink();
    pGenerator = HkxObjectExpSharedPtr();
    spBoneWeight = HkxObjectExpSharedPtr();
}

BSBoneSwitchGeneratorBoneData::~BSBoneSwitchGeneratorBoneData(){
    refCount--;
}
