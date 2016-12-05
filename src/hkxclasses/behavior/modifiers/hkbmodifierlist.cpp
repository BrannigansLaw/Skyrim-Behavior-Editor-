#include "hkbmodifierlist.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbModifierList
*/

uint hkbModifierList::refCount = 0;

QString hkbModifierList::classname = "hkbModifierList";

hkbModifierList::hkbModifierList(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_MODIFIER_LIST, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "ModifierList"+QString::number(refCount);
}

QString hkbModifierList::getClassname(){
    return classname;
}

QString hkbModifierList::getName() const{
    return name;
}

int hkbModifierList::getIndexToInsertIcon() const{
    for (int i = 0; i < modifiers.size(); i++){
        if (!modifiers.at(i).constData()){
            return 1 + i;
        }
    }
    return -1;
}

bool hkbModifierList::setChildAt(HkxObject *newChild, ushort index){
    if (newChild && newChild->getType() != TYPE_MODIFIER){
        return false;
    }
    if (!modifiers.isEmpty()){
        if (index < modifiers.size()){
            if (modifiers.at(index)){
                modifiers[index] = HkxObjectExpSharedPtr(newChild);
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        modifiers.append(HkxObjectExpSharedPtr(newChild));
        return true;
    }
}

bool hkbModifierList::wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace){
    if (!objToInject || objToInject->getType() != TYPE_MODIFIER){
        return false;
    }
    bool wasReplaced = false;
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).data() == childToReplace){
            if (!objToInject->setChildAt(modifiers.at(i).data())){
                return false;
            }
            modifiers[i] = HkxObjectExpSharedPtr(objToInject);
            wasReplaced = true;
        }
    }
    return wasReplaced;
}

bool hkbModifierList::appendObject(DataIconManager *objToAppend){
    if (!objToAppend || objToAppend->getType() != TYPE_MODIFIER){
        return false;
    }
    modifiers.append(HkxObjectExpSharedPtr(objToAppend));
    return true;
}

bool hkbModifierList::removeObject(DataIconManager *objToRemove, bool removeAll){
    if (removeAll){
        if (modifiers.removeAll(HkxObjectExpSharedPtr(objToRemove))){
            return true;
        }
    }else{
        if (modifiers.removeOne(HkxObjectExpSharedPtr(objToRemove))){
            return true;
        }
    }
    return false;
}

bool hkbModifierList::hasChildren() const{
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).data()){
            return true;
        }
    }
    return false;
}

int hkbModifierList::addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder){
    int objectChildCount = 0;
    if (reverseOrder){
        for (int i = modifiers.size() - 1; i >= 0; i--){
            if (modifiers.at(i).data()){
                list.append(static_cast<DataIconManager *>(modifiers.at(i).data()));
                objectChildCount++;
            }
        }
    }else{
        for (int i = 0; i < modifiers.size(); i++){
            if (modifiers.at(i).data()){
                list.append(static_cast<DataIconManager *>(modifiers.at(i).data()));
                objectChildCount++;
            }
        }
    }
    return objectChildCount;
}

bool hkbModifierList::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "modifiers"){
            if (!readReferences(reader.getElementValueAt(index), modifiers)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'modifiers' references!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbModifierList::write(HkxXMLWriter *writer){
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
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"modifiers", QString::number(modifiers.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < modifiers.size(); i++){
            refString = refString+" "+modifiers.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }
        }
        if (modifiers.size() > 0){
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        for (int i = 0; i < modifiers.size(); i++){
            if (modifiers.at(i).data() && !modifiers.at(i).data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'modifiers' at: "+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbModifierList::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < modifiers.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifiers.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'modifiers' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_MODIFIER){
            writeToLog(getClassname()+": link()!\n'modifiers' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            modifiers[i] = *ptr;
        }else{
            modifiers[i] = *ptr;
        }
    }
    return true;
}

void hkbModifierList::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < modifiers.size(); i++){
        modifiers[i] = HkxObjectExpSharedPtr();
    }
}

bool hkbModifierList::evaulateDataValidity(){
    bool valid = true;
    for (int i = 0; i < modifiers.size(); i++){
        if (!modifiers.at(i).data() || modifiers.at(i).data()->getType() != HkxObject::TYPE_MODIFIER){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (modifiers.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbModifierList::~hkbModifierList(){
    refCount--;
}
