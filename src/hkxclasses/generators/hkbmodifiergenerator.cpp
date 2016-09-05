#include "hkbmodifiergenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

QString hkbModifierGenerator::classname = "hkbModifierGenerator";

hkbModifierGenerator::hkbModifierGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
      userData(0)
{
    setType(HKB_MODIFIER_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Modifier Generator "+QString::number(refCount);
}

QString hkbModifierGenerator::getClassname(){
    return classname;
}

QString hkbModifierGenerator::getName() const{
    return name;
}

bool hkbModifierGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "modifier"){
            if (!modifier.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'modifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog("hkbModifierGenerator: readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbModifierGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbModifierGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //enterNotifyEvents
    HkxObjectExpSharedPtr *ptr;
    //modifier
    ptr = getParentFile()->findModifier(modifier.getReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            writeToLog("hkbModifierGenerator: link()!\n'modifier' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    //generator
    ptr = getParentFile()->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog("hkbModifierGenerator: link()!\nFailed to properly link 'generator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog("hkbModifierGenerator: link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

void hkbModifierGenerator::unlink(){
    HkDynamicObject::unlink();
    modifier = HkxObjectExpSharedPtr();
    generator = HkxObjectExpSharedPtr();
}

bool hkbModifierGenerator::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (modifier.data() && modifier.data()->getType() != HkxObject::TYPE_MODIFIER){
    }else if (!generator.data() || generator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbModifierGenerator::~hkbModifierGenerator(){
    refCount--;
}
