#include "hkbmodifiergenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbModifierGenerator
*/

uint hkbModifierGenerator::refCount = 0;

QString hkbModifierGenerator::classname = "hkbModifierGenerator";

hkbModifierGenerator::hkbModifierGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(1)
{
    setType(HKB_MODIFIER_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ModifierGenerator"+QString::number(refCount);
}

QString hkbModifierGenerator::getClassname(){
    return classname;
}

QString hkbModifierGenerator::getName() const{
    return name;
}

int hkbModifierGenerator::getIndexToInsertIcon(HkxObject *child) const{
    if (modifier.constData() == child){
        return 0;
    }else if (generator.constData() == child){
        return 1;
    }
    return -1;
}

bool hkbModifierGenerator::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_MODIFIER){
        modifier = HkxSharedPtr((HkxObject *)obj);
    }else if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        generator = HkxSharedPtr((HkxObject *)obj);
    }else{
        return false;
    }
    return true;
}

bool hkbModifierGenerator::removeObjectAt(int index){
    if (index == 0){
        modifier = HkxSharedPtr();
    }else if (index == 1){
        generator = HkxSharedPtr();
    }else if (index == -1){
        modifier = HkxSharedPtr();
        generator = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbModifierGenerator::hasChildren() const{
    if (generator.data() || modifier.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> hkbModifierGenerator::getChildren() const{
    QList<DataIconManager *> list;
    if (modifier.data()){
        list.append(static_cast<DataIconManager*>(modifier.data()));
    }
    if (generator.data()){
        list.append(static_cast<DataIconManager*>(generator.data()));
    }
    return list;
}

int hkbModifierGenerator::getIndexOfObj(DataIconManager *obj) const{
    if (modifier.data() == (HkxObject *)obj){
        return 0;
    }else if (generator.data() == (HkxObject *)obj){
        return 1;
    }
    return -1;
}

bool hkbModifierGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "modifier"){
            if (!modifier.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'modifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "generator"){
            if (!generator.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'generator' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbModifierGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userData"), QString::number(userData));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        if (modifier.data()){
            refString = modifier.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("modifier"), refString);
        if (generator.data()){
            refString = generator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("generator"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (modifier.data() && !modifier.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'modifier'!!!");
        }
        if (generator.data() && !generator.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'generator'!!!");
        }
    }
    return true;
}

bool hkbModifierGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifier.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'modifier' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(generator.getShdPtrReference());
    if (!ptr){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'generator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'generator' data field is linked to invalid child!\nObject Name: "+name);
        setDataValidity(false);
        generator = *ptr;
    }else{
        generator = *ptr;
    }
    return true;
}

void hkbModifierGenerator::unlink(){
    HkDynamicObject::unlink();
    modifier = HkxSharedPtr();
    generator = HkxSharedPtr();
}

QString hkbModifierGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (!modifier.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null modifier!\n");
    }else if (modifier.data()->getType() != HkxObject::TYPE_MODIFIER){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid modifier type! Signature: "+QString::number(modifier.data()->getSignature(), 16)+"\n");
    }
    if (!generator.data()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Null generator!\n");
    }else if (generator.data()->getType() != HkxObject::TYPE_GENERATOR){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid generator type! Signature: "+QString::number(generator.data()->getSignature(), 16)+"\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbModifierGenerator::~hkbModifierGenerator(){
    refCount--;
}
