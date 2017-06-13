#include "hkbblendergeneratorchild.h"
#include "hkbblendergenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBlenderGeneratorChild
*/

uint hkbBlenderGeneratorChild::refCount = 0;

QString hkbBlenderGeneratorChild::classname = "hkbBlenderGeneratorChild";

hkbBlenderGeneratorChild::hkbBlenderGeneratorChild(HkxFile *parent, hkbGenerator *parentBG, long ref)
    : hkbGenerator(parent, ref),
      weight(0),
      worldFromModelWeight(0),
      parentBG(parentBG)
{
    setType(HKB_BLENDER_GENERATOR_CHILD, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbBlenderGeneratorChild::getClassname(){
    return classname;
}

hkbGenerator * hkbBlenderGeneratorChild::getParentGenerator() const{
    if (parentBG.data() && (parentBG.data()->getSignature() == HKB_BLENDER_GENERATOR || parentBG.data()->getSignature() == HKB_POSE_MATCHING_GENERATOR)){
        return reinterpret_cast<hkbGenerator *>(parentBG.data());
    }
    return NULL;
}

bool hkbBlenderGeneratorChild::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "boneWeights"){
            if (!boneWeights.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneWeights' reference!\nObject Reference: "+ref);
            }
        }else if (text == "weight"){
            weight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'weight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelWeight"){
            worldFromModelWeight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'worldFromModelWeight' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBlenderGeneratorChild::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        if (generator.data()){
            refString = generator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("generator"), refString);
        if (boneWeights.data()){
            refString = boneWeights.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneWeights"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("weight"), QString::number(weight, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldFromModelWeight"), QString::number(worldFromModelWeight, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (generator.data() && !generator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'generator'!!!", true);
        }
        if (boneWeights.data() && !boneWeights.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'boneWeights'!!!", true);
        }
    }
    return true;
}

bool hkbBlenderGeneratorChild::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(boneWeights.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_BONE_WEIGHT_ARRAY){
            writeToLog(getClassname()+": link()!\n'boneWeights' data field is linked to invalid child!");
            setDataValidity(false);
        }
        boneWeights = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(generator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'generator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog(getClassname()+": link()!\n'generator' data field is linked to invalid child!");
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

void hkbBlenderGeneratorChild::unlink(){
    HkDynamicObject::unlink();
    parentBG = HkxSharedPtr();
    generator = HkxSharedPtr();
    boneWeights = HkxSharedPtr();
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
