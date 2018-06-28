#include "hkbmanualselectorgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
//#include "src/ui/dataiconmanager.h"
/*
 * CLASS: hkbManualSelectorGenerator
*/

uint hkbManualSelectorGenerator::refCount = 0;

QString hkbManualSelectorGenerator::classname = "hkbManualSelectorGenerator";

hkbManualSelectorGenerator::hkbManualSelectorGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      selectedGeneratorIndex(0),
      currentGeneratorIndex(0)
{
    setType(HKB_MANUAL_SELECTOR_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ManualSelectorGenerator"+QString::number(refCount);
}

QString hkbManualSelectorGenerator::getClassname(){
    return classname;
}

QString hkbManualSelectorGenerator::getName() const{
    return name;
}

int hkbManualSelectorGenerator::getIndexToInsertIcon(HkxObject *child) const{
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).constData() == child){
            return i;
        }
    }
    return -1;
}

bool hkbManualSelectorGenerator::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        if (index >= generators.size() || index == -1){
            generators.append(HkxSharedPtr(obj));
        }else if (index == 0 || !generators.isEmpty()){
            generators.replace(index, HkxSharedPtr(obj));
        }
        return true;
    }else{
        return false;
    }
}

bool hkbManualSelectorGenerator::removeObjectAt(int index){
    if (index > -1 && index < generators.size()){
        generators.removeAt(index);
    }else if (index == -1){
        generators.clear();
    }else{
        return false;
    }
    return true;
}

bool hkbManualSelectorGenerator::hasChildren() const{
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).data()){
            return true;
        }
    }
    return false;
}

bool hkbManualSelectorGenerator::merge(HkxObject *recessiveObject){
    hkbManualSelectorGenerator *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
        obj = static_cast<hkbManualSelectorGenerator *>(recessiveObject);
        /*for (auto i = generators.size(); i < obj->generators.size(); i++){
            generators.append(obj->generators.at(i));
            getParentFile()->addObjectToFile(obj->generators.at(i).data(), -1);
        }*/
        injectWhileMerging((obj));
        return true;
    }else{
        return false;
    }
}

QList<DataIconManager *> hkbManualSelectorGenerator::getChildren() const{
    QList<DataIconManager *> list;
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).data()){
            list.append(static_cast<DataIconManager*>(generators.at(i).data()));
        }
    }
    return list;
}

int hkbManualSelectorGenerator::getIndexOfObj(DataIconManager *obj) const{
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

bool hkbManualSelectorGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "generators"){
            if (!readReferences(reader.getElementValueAt(index), generators)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'generators' references!\nObject Reference: "+ref);
            }
        }else if (text == "selectedGeneratorIndex"){
            selectedGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'selectedGeneratorIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentGeneratorIndex"){
            currentGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'currentGeneratorIndex' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbManualSelectorGenerator::write(HkxXMLWriter *writer){
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
        refString = "";
        list1 = {writer->name, writer->numelements};
        for (auto i = generators.size() - 1; i >= 0; i--){
            if (!generators.at(i).data()){
                generators.removeAt(i);
            }
        }
        list2 = {"generators", QString::number(generators.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0, j = 1; i < generators.size(); i++, j++){
            refString.append(generators.at(i).data()->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (generators.size() > 0){
            if (refString.endsWith(" \0")){
                refString.remove(refString.lastIndexOf(" "), 1);
            }
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("selectedGeneratorIndex"), QString::number(selectedGeneratorIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentGeneratorIndex"), QString::number(currentGeneratorIndex));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        for (int i = 0; i < generators.size(); i++){
            if (generators.at(i).data() && !generators.at(i).data()->write(writer)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'generators' at: "+QString::number(i)+"!!!");
            }
        }
    }
    return true;
}

bool hkbManualSelectorGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < generators.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(generators.at(i).getShdPtrReference());
        if (!ptr){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'generators' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'generators' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            generators[i] = *ptr;
        }else{
            generators[i] = *ptr;
        }
    }
    return true;
}

void hkbManualSelectorGenerator::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < generators.size(); i++){
        generators[i] = HkxSharedPtr();
    }
}

QString hkbManualSelectorGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (generators.isEmpty()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": generators is empty!\n");
    }else{
        for (int i = 0; i < generators.size(); i++){
            if (!generators.at(i).data()){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": generators at index '"+QString::number(i)+"' is null!\n");
            }else if (generators.at(i).data()->getType() != HkxObject::TYPE_GENERATOR){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid generator! Signature: "+QString::number(generators.at(i).data()->getSignature(), 16)+"\n");
            }
        }
    }
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (selectedGeneratorIndex >= generators.size()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": selectedGeneratorIndex is out of range!\n");
    }
    if (currentGeneratorIndex >= generators.size()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": currentGeneratorIndex is out of range!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbManualSelectorGenerator::~hkbManualSelectorGenerator(){
    refCount--;
}
