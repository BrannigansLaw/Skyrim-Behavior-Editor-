#include "bsistatetagginggenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSiStateTaggingGenerator
*/

uint BSiStateTaggingGenerator::refCount = 0;

QString BSiStateTaggingGenerator::classname = "BSiStateTaggingGenerator";

BSiStateTaggingGenerator::BSiStateTaggingGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(1),
      iStateToSetAs(-1),
      iPriority(0)
{
    setType(BS_I_STATE_TAGGING_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "BS iState Tagging Generator "+QString::number(refCount);
}

QString BSiStateTaggingGenerator::getClassname(){
    return classname;
}

QString BSiStateTaggingGenerator::getName() const{
    return name;
}

bool BSiStateTaggingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "iStateToSetAs"){
            iStateToSetAs = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'iStateToSetAs' data field!\nObject Reference: "+ref);
            }
        }else if (text == "iPriority"){
            iPriority = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("BSiStateTaggingGenerator: readData()!\nFailed to properly read 'iPriority' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSiStateTaggingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSiStateTaggingGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pDefaultGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog("BSiStateTaggingGenerator: link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSiStateTaggingGenerator: link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    return true;
}

void BSiStateTaggingGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxObjectExpSharedPtr();
}

bool BSiStateTaggingGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!pDefaultGenerator.data() || pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSiStateTaggingGenerator::~BSiStateTaggingGenerator(){
    refCount--;
}
