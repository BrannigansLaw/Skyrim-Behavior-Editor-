#include "hkbmanualselectorgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
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
    name = "hkbManualSelectorGenerator"+QString::number(refCount);
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
            generators.append(HkxObjectExpSharedPtr((HkxObject *)obj));
        }else if (index > -1){
            generators[index] = HkxObjectExpSharedPtr((HkxObject *)obj);
        }else{
            return false;
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

QList<DataIconManager *> hkbManualSelectorGenerator::getChildren() const{
    QList<DataIconManager *> list;
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).data()){
            list.append((DataIconManager *)generators.at(i).data());
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
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "generators"){
            if (!readReferences(reader.getElementValueAt(index), generators)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'generators' references!\nObject Reference: "+ref);
            }
        }else if (text == "selectedGeneratorIndex"){
            selectedGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'selectedGeneratorIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "currentGeneratorIndex"){
            currentGeneratorIndex = reader.getElementValueAt(index).toShort(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'currentGeneratorIndex' data field!\nObject Reference: "+ref);
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
        list2 = {"generators", QString::number(generators.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < generators.size(); i++){
            refString = refString+" "+generators.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }
        }
        if (generators.size() > 0){
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("selectedGeneratorIndex"), QString::number(selectedGeneratorIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("currentGeneratorIndex"), QString::number(currentGeneratorIndex));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        for (int i = 0; i < generators.size(); i++){
            if (generators.at(i).data() && !generators.at(i).data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'generators' at: "+QString::number(i)+"!!!", true);
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
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < generators.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(generators.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'generators' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            writeToLog(getClassname()+": link()!\n'generators' data field is linked to invalid child!\nObject Name: "+name);
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
        generators[i] = HkxObjectExpSharedPtr();
    }
}

bool hkbManualSelectorGenerator::evaulateDataValidity(){
    bool valid = true;
    for (int i = 0; i < generators.size(); i++){
        if (!generators.at(i).data() || generators.at(i).data()->getType() != HkxObject::TYPE_GENERATOR){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (selectedGeneratorIndex < 0 || selectedGeneratorIndex > generators.size()){
    }else if (currentGeneratorIndex < 0 || currentGeneratorIndex > generators.size()){
    }else if (name == ""){
    }else if (generators.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbManualSelectorGenerator::~hkbManualSelectorGenerator(){
    refCount--;
}
