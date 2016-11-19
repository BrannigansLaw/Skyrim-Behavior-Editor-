#include "bsoffsetanimationgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: BSOffsetAnimationGenerator
*/

uint BSOffsetAnimationGenerator::refCount = 0;

QString BSOffsetAnimationGenerator::classname = "BSOffsetAnimationGenerator";

BSOffsetAnimationGenerator::BSOffsetAnimationGenerator(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      fOffsetVariable(0),
      fOffsetRangeStart(0),
      fOffsetRangeEnd(0)
{
    setType(BS_OFFSET_ANIMATION_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "BS Offset Animation Generator "+QString::number(refCount);
}

QString BSOffsetAnimationGenerator::getClassname(){
    return classname;
}

QString BSOffsetAnimationGenerator::getName() const{
    return name;
}

int BSOffsetAnimationGenerator::getIndexToInsertIcon() const{
    if (!pDefaultGenerator.constData()){    //Not sure about this...
        return 0;
    }else if (!pOffsetClipGenerator.constData()){
        return 1;
    }
    return -1;
}

bool BSOffsetAnimationGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pOffsetClipGenerator"){
            if (!pOffsetClipGenerator.readReference(index, reader)){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'pOffsetClipGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetVariable"){
            fOffsetVariable = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetVariable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeStart"){
            fOffsetRangeStart = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetRangeStart' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fOffsetRangeEnd"){
            fOffsetRangeEnd = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("BSOffsetAnimationGenerator: readData()!\nFailed to properly read 'fOffsetRangeEnd' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSOffsetAnimationGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pDefaultGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("BSOffsetAnimationGenerator: link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    //pOffsetClipGenerator
    ptr = getParentFile()->findGenerator(pOffsetClipGenerator.getReference());
    if (!ptr){
        writeToLog("BSOffsetAnimationGenerator: link()!\nFailed to properly link 'pOffsetClipGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_CLIP_GENERATOR){
        writeToLog("BSOffsetAnimationGenerator: link()!\n'pOffsetClipGenerator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        pOffsetClipGenerator = *ptr;
    }else{
        pOffsetClipGenerator = *ptr;
    }
    return true;
}

void BSOffsetAnimationGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxObjectExpSharedPtr();
    pOffsetClipGenerator = HkxObjectExpSharedPtr();
}

bool BSOffsetAnimationGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!pDefaultGenerator.data() || pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (!pOffsetClipGenerator.data() || pOffsetClipGenerator.data()->getSignature() != HKB_CLIP_GENERATOR){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSOffsetAnimationGenerator::~BSOffsetAnimationGenerator(){
    refCount--;
}
