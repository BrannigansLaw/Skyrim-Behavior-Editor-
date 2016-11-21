#include "bsboneswitchgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSBoneSwitchGenerator
*/

uint BSBoneSwitchGenerator::refCount = 0;

QString BSBoneSwitchGenerator::classname = "BSBoneSwitchGenerator";

BSBoneSwitchGenerator::BSBoneSwitchGenerator(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),\
    userData(0)
{
    setType(BS_BONE_SWITCH_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "BS Bone Switch Generator "+QString::number(refCount);
}

QString BSBoneSwitchGenerator::getClassname(){
    return classname;
}

QString BSBoneSwitchGenerator::getName() const{
    return name;
}

int BSBoneSwitchGenerator::getIndexToInsertIcon() const{
    if (!pDefaultGenerator.constData()){    //Not sure... Need to determine source of change???
        return 0;
    }else{
        for (int i = 0; i < ChildrenA.size(); i++){
            if (!ChildrenA.at(i).constData()){
                return 1 + i;
            }
        }
    }
    return -1;
}

bool BSBoneSwitchGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "pDefaultGenerator"){
            if (!pDefaultGenerator.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pDefaultGenerator' reference!\nObject Reference: "+ref);
            }
        }else if (text == "ChildrenA"){
            if (!readReferences(reader.getElementValueAt(index), ChildrenA)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'ChildrenA' references!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSBoneSwitchGenerator::write(HkxXMLWriter *writer){
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
        if (pDefaultGenerator.data()){
            refString = pDefaultGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pDefaultGenerator"), refString);
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"ChildrenA", QString::number(ChildrenA.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < ChildrenA.size(); i++){
            refString = refString+" "+ChildrenA.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }
        }
        if (ChildrenA.size() > 0){
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < ChildrenA.size(); i++){
            if (ChildrenA.at(i).data() && !ChildrenA.at(i).data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'ChildrenA' at: "+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool BSBoneSwitchGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!");
    }
    //pDefaultGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'pDefaultGenerator' data field!");
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog(getClassname()+": link()!\n'pDefaultGenerator' data field is linked to invalid child!");
        setDataValidity(false);
        pDefaultGenerator = *ptr;
    }else{
        pDefaultGenerator = *ptr;
    }
    for (int i = 0; i < ChildrenA.size(); i++){
        //ChildrenA
        ptr = getParentFile()->findGenerator(ChildrenA.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'ChildrenA' data field!");
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            writeToLog(getClassname()+": link()!\n'ChildrenA' data field is linked to invalid child!");
            setDataValidity(false);
            ChildrenA[i] = *ptr;
        }else{
            ChildrenA[i] = *ptr;
        }
    }
    return true;
}

void BSBoneSwitchGenerator::unlink(){
    HkDynamicObject::unlink();
    pDefaultGenerator = HkxObjectExpSharedPtr();
    for (int i = 0; i < ChildrenA.size(); i++){
        if (ChildrenA.at(i).data()){
            ChildrenA[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        ChildrenA[i] = HkxObjectExpSharedPtr();
    }
}

bool BSBoneSwitchGenerator::evaulateDataValidity(){
    bool valid = true;
    for (int i = 0; i < ChildrenA.size(); i++){
        if (!ChildrenA.at(i).data() || ChildrenA.at(i).data()->getSignature() != BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!pDefaultGenerator.data() || pDefaultGenerator.data()->getType() != HkxObject::TYPE_GENERATOR){
    }else if (ChildrenA.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSBoneSwitchGenerator::~BSBoneSwitchGenerator(){
    refCount--;
}
