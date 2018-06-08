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
      userData(1),
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

bool hkbModifierList::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_MODIFIER){
        if (index >= modifiers.size() || index == -1){
            modifiers.append(HkxSharedPtr(obj));
        }else if (index == 0 || !modifiers.isEmpty()){
            modifiers.replace(index, HkxSharedPtr(obj));
        }
        return true;
    }else{
        return false;
    }
}

bool hkbModifierList::removeObjectAt(int index){
    if (index > -1 && index < modifiers.size()){
        modifiers.removeAt(index);
    }else if (index == -1){
        modifiers.clear();
    }else{
        return false;
    }
    return true;
}

bool hkbModifierList::hasChildren() const{
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).data()){
            return true;
        }
    }
    return false;
}

bool hkbModifierList::merge(HkxObject *recessiveObject){
    hkbModifierList *obj = nullptr;
    bool found;
    int size = modifiers.size();
    QList <DataIconManager *> objects;
    QList <DataIconManager *> children;
    hkbModifier *modifier = nullptr;
    if (recessiveObject && recessiveObject->getSignature() == HKB_MODIFIER_LIST){
        obj = static_cast<hkbModifierList *>(recessiveObject);
        for (auto i = 0; i < obj->modifiers.size(); i++){
            found = false;
            for (auto j = 0; j < size; j++){
                if (modifiers.at(j).data()->getSignature() == obj->modifiers.at(i).data()->getSignature() && static_cast<hkbModifier *>(modifiers.at(j).data())->getName() == static_cast<hkbModifier *>(obj->modifiers.at(i).data())->getName()){
                    found = true;
                }
            }
            if (!found){
                modifiers.append(obj->modifiers.at(i));
                getParentFile()->addObjectToFile(obj->modifiers.at(i).data(), -1);
                objects = static_cast<DataIconManager *>(obj->modifiers.at(i).data())->getChildren();
                while (!objects.isEmpty()){
                    if (objects.last()->getType() == HkxObject::TYPE_MODIFIER){
                        modifier = static_cast<hkbModifier *>(objects.last());
                        if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(modifier)){
                            getParentFile()->addObjectToFile(modifier, -1);
                            children = modifier->getChildren();
                        }
                    }
                    objects.removeLast();
                    objects = objects + children;
                    children.clear();
                }
            }
        }
        return true;
    }else{
        return false;
    }
}

QList<DataIconManager *> hkbModifierList::getChildren() const{
    QList<DataIconManager *> list;
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).data()){
            list.append(static_cast<DataIconManager*>(modifiers.at(i).data()));
        }
    }
    return list;
}

int hkbModifierList::getIndexOfObj(DataIconManager *obj) const{
    for (int i = 0; i < modifiers.size(); i++){
        if (modifiers.at(i).data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

bool hkbModifierList::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "modifiers"){
            if (!readReferences(reader.getElementValueAt(index), modifiers)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'modifiers' references!\nObject Reference: "+ref);
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
        for (int i = 0, j = 1; i < modifiers.size(); i++, j++){
            refString.append(modifiers.at(i).data()->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (modifiers.size() > 0){
            if (refString.endsWith(" \0")){
                refString.remove(refString.lastIndexOf(" "), 1);
            }
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        for (int i = 0; i < modifiers.size(); i++){
            if (modifiers.at(i).data() && !modifiers.at(i).data()->write(writer)){
                WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'modifiers' at: "+QString::number(i)+"!!!");
            }
        }
    }
    return true;
}

bool hkbModifierList::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < modifiers.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifiers.at(i).getShdPtrReference());
        if (!ptr){
            WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'modifiers' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getType() != TYPE_MODIFIER){
            WRITE_TO_LOG(getClassname()+": link()!\n'modifiers' data field is linked to invalid child!\nObject Name: "+name);
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
        modifiers[i] = HkxSharedPtr();
    }
}

bool hkbModifierList::evaluateDataValidity(){
    bool valid = true;
    for (int i = 0; i < modifiers.size(); i++){
        if (!modifiers.at(i).data() || modifiers.at(i).data()->getType() != HkxObject::TYPE_MODIFIER){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaluateDataValidity() || (name == "") || (modifiers.isEmpty()) || !valid){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbModifierList::~hkbModifierList(){
    refCount--;
}
