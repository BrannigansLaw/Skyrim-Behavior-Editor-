#include "bsistatetagginggenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSiStateTaggingGenerator
*/

uint BSiStateTaggingGenerator::refCount = 0;

QString BSiStateTaggingGenerator::classname = "BSiStateTaggingGenerator";

BSiStateTaggingGenerator::BSiStateTaggingGenerator(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(1),
      iStateToSetAs(-1),
      iPriority(0)
{
    setType(BS_I_STATE_TAGGING_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "BS iState Tagging Generator "+QString::number(refCount);
}

QString BSiStateTaggingGenerator::getClassname(){
    return classname;
}

QString BSiStateTaggingGenerator::getName() const{
    return name;
}

bool BSiStateTaggingGenerator::setChildAt(HkxObject *newChild, ushort index){
    if (index == 0 && (!newChild || newChild->getType() == TYPE_GENERATOR)){
        pDefaultGenerator = HkxObjectExpSharedPtr(newChild);
        return true;
    }else{
        return false;
    }
}

bool BSiStateTaggingGenerator::wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace){
    if (pDefaultGenerator.data() == childToReplace){
        if (!objToInject->setChildAt(pDefaultGenerator.data())){
            return false;
        }
        pDefaultGenerator = HkxObjectExpSharedPtr(objToInject);
        return true;
    }
    return false;
}

bool BSiStateTaggingGenerator::appendObject(hkbGenerator *objToAppend){
    pDefaultGenerator = HkxObjectExpSharedPtr(objToAppend);
    return true;
}

bool BSiStateTaggingGenerator::removeObject(hkbGenerator *objToRemove, bool removeAll){
    if (pDefaultGenerator.data() == objToRemove){
        pDefaultGenerator = HkxObjectExpSharedPtr();
        return true;
    }
    return false;
}

int BSiStateTaggingGenerator::addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder){
    int objectChildCount = 0;
    if (pDefaultGenerator.data()){
        list.append(pDefaultGenerator);
        objectChildCount++;
    }
    return objectChildCount;
}

bool BSiStateTaggingGenerator::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "iStateToSetAs"){
            iStateToSetAs = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'iStateToSetAs' data field!\nObject Reference: "+ref);
            }
        }else if (text == "iPriority"){
            iPriority = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'iPriority' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSiStateTaggingGenerator::write(HkxXMLWriter *writer){
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
        if (pDefaultGenerator.data()){
            refString = pDefaultGenerator.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pDefaultGenerator"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("iStateToSetAs"), QString::number(iStateToSetAs));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("iPriority"), QString::number(iPriority));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (pDefaultGenerator.data() && !pDefaultGenerator.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pDefaultGenerator'!!!", true);
        }
    }
    return true;
}

bool BSiStateTaggingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //pDefaultGenerator
    HkxObjectExpSharedPtr *ptr = getParentFile()->findGenerator(pDefaultGenerator.getReference());
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'pDefaultGenerator' data field!\nObject Name: "+name);
        setDataValidity(false);
    }else if ((*ptr)->getType() != TYPE_GENERATOR || (*ptr)->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA || (*ptr)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || (*ptr)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
        writeToLog(getClassname()+": link()!\n'pDefaultGenerator' data field is linked to invalid child!\nObject Name: "+name);
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
