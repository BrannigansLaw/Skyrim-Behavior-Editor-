#include "bsistatemanagermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSIStateManagerModifier
*/

uint BSIStateManagerModifier::refCount = 0;

QString BSIStateManagerModifier::classname = "BSIStateManagerModifier";

BSIStateManagerModifier::BSIStateManagerModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      iStateVar(-1)
{
    setType(BS_I_STATE_MANAGER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "IStateManagerModifier"+QString::number(refCount);
}

QString BSIStateManagerModifier::getClassname(){
    return classname;
}

QString BSIStateManagerModifier::getName() const{
    return name;
}

bool BSIStateManagerModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "iStateVar"){
            iStateVar = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'iStateVar' data field!\nObject Reference: "+ref);
            }
        }if (text == "stateData"){
            int numtriggers = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtriggers; j++){
                stateData.append(BSiStateData());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "pStateMachine"){
                        if (!stateData.last().pStateMachine.readShdPtrReference(index, reader)){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'pStateMachine' reference!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "stateID"){
                        stateData.last().stateID = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "iStateToSetAs"){
                        stateData.last().iStateToSetAs = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            return false;
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool BSIStateManagerModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("iStateVar"), QString::number(iStateVar));
        list1 = {writer->name, writer->numelements};
        list2 = {"stateData", QString::number(stateData.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < stateData.size(); i++){
            writer->writeLine(writer->object, true);
            if (stateData.at(i).pStateMachine.data()){
                refString = stateData.at(i).pStateMachine.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pStateMachine"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("stateID"), QString::number(stateData.at(i).stateID));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("iStateToSetAs"), QString::number(stateData.at(i).iStateToSetAs));
            writer->writeLine(writer->object, false);
        }
        if (stateData.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool BSIStateManagerModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < stateData.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(stateData.at(i).pStateMachine.getShdPtrReference());
        if (!ptr){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'stateData' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'stateData' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            stateData[i].pStateMachine = *ptr;
        }else{
            stateData[i].pStateMachine = *ptr;
        }
    }
    return true;
}

void BSIStateManagerModifier::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < stateData.size(); i++){
        stateData[i].pStateMachine = HkxSharedPtr();
    }
}

QString BSIStateManagerModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (stateData.isEmpty()){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": stateData is empty!\n");
    }else{
        for (int i = 0; i < stateData.size(); i++){
            if (!stateData.at(i).pStateMachine.data()){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": stateData at index '"+QString::number(i)+"' is null!\n");
            }else if (stateData.at(i).pStateMachine.data()->getSignature() != HKB_STATE_MACHINE){
                isvalid = false;
                errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid stateData! Signature: "+QString::number(stateData.at(i).pStateMachine.data()->getSignature(), 16)+"\n");
            }
        }
    }
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

BSIStateManagerModifier::~BSIStateManagerModifier(){
    refCount--;
}
