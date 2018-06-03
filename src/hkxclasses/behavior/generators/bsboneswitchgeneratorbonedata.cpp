#include "bsboneswitchgeneratorbonedata.h"
#include "bsboneswitchgenerator.h"
#include "bsboneswitchgeneratorbonedata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSBoneSwitchGeneratorBoneData
*/

uint BSBoneSwitchGeneratorBoneData::refCount = 0;

QString BSBoneSwitchGeneratorBoneData::classname = "BSBoneSwitchGeneratorBoneData";

BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData(HkxFile *parent, hkbGenerator *parentBSG, long ref)
    : hkbGenerator(parent, ref),
      parentBSG(parentBSG)
{
    setType(BS_BONE_SWITCH_GENERATOR_BONE_DATA, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    if (parentBSG && (parentBSG->getSignature() != BS_BONE_SWITCH_GENERATOR)){
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneData::BSBoneSwitchGeneratorBoneData: parentBSG is incorrect type!!!");
    }
}

QString BSBoneSwitchGeneratorBoneData::getClassname(){
    return classname;
}

bool BSBoneSwitchGeneratorBoneData::readData(const HkxXmlReader &reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pGenerator"){
            if (!pGenerator.readShdPtrReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'pGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "spBoneWeight"){
            if (!spBoneWeight.readShdPtrReference(index, reader)){
                writeToLog("BSBoneSwitchGeneratorBoneData: readData()!\nFailed to properly read 'spBoneWeight' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::write(HkxXMLWriter *writer){
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
        if (pGenerator.data()){
            refString = pGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pGenerator"), refString);
        if (spBoneWeight.data()){
            refString = spBoneWeight.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("spBoneWeight"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (pGenerator.data() && !pGenerator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pGenerator'!!!", true);
        }
        if (spBoneWeight.data() && !spBoneWeight.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'spBoneWeight'!!!", true);
        }
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::hasChildren() const{
    if (pGenerator.data()){
        return true;
    }
    return false;
}

QString BSBoneSwitchGeneratorBoneData::getName() const{
    if (parentBSG.constData()){
        const BSBoneSwitchGenerator *par = static_cast<const BSBoneSwitchGenerator *>(parentBSG.constData());
        return par->getName()+"_CHILD"+QString::number(getThisIndex());
    }else{
        getParentFile()->writeToLog(getClassname()+": getName()!\nNo parent BSBoneSwitchGenerator'!!!", true);
    }
    return "";
}

int BSBoneSwitchGeneratorBoneData::getThisIndex() const{
    if (parentBSG.constData()){
        return static_cast<DataIconManager *>(parentBSG.data())->getIndexOfObj((DataIconManager *)this);
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneData:::getThisIndex() !\nNo parent BSBoneSwitchGenerator'!!!");
    }
    return -1;
}

void BSBoneSwitchGeneratorBoneData::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (spBoneWeight.data()){
        ref++;
        spBoneWeight.data()->setReference(ref);
    }
}

QList<DataIconManager *> BSBoneSwitchGeneratorBoneData::getChildren() const{
    QList<DataIconManager *> list;
    if (pGenerator.data()){
        list.append(static_cast<DataIconManager *>(pGenerator.data()));
    }
    return list;
}

int BSBoneSwitchGeneratorBoneData::getIndexOfObj(DataIconManager *obj) const{
    if (pGenerator.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool BSBoneSwitchGeneratorBoneData::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        pGenerator = HkxSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::removeObjectAt(int index){
    if (index == 0 || index == -1){
        pGenerator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool BSBoneSwitchGeneratorBoneData::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog("BSBoneSwitchGeneratorBoneData: link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(pGenerator.getShdPtrReference());
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
    ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(spBoneWeight.getShdPtrReference());
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
    pGenerator = HkxSharedPtr();
    spBoneWeight = HkxSharedPtr();
}

bool BSBoneSwitchGeneratorBoneData::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (spBoneWeight.data() && spBoneWeight.data()->getSignature() != HKB_BONE_WEIGHT_ARRAY){
    }else if (!pGenerator.data() || pGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSBoneSwitchGeneratorBoneData::~BSBoneSwitchGeneratorBoneData(){
    refCount--;
}
