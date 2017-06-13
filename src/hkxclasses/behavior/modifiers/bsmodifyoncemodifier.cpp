#include "bsmodifyoncemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSModifyOnceModifier
*/

uint BSModifyOnceModifier::refCount = 0;

QString BSModifyOnceModifier::classname = "BSModifyOnceModifier";

BSModifyOnceModifier::BSModifyOnceModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(BS_MODIFY_ONCE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BSModifyOnceModifier"+QString::number(refCount);
}

QString BSModifyOnceModifier::getClassname(){
    return classname;
}

QString BSModifyOnceModifier::getName() const{
    return name;
}

int BSModifyOnceModifier::getIndexToInsertIcon() const{
    if (!pOnActivateModifier.constData()){    //Not sure about this...
        return 0;
    }else if (!pOnDeactivateModifier.constData()){
        return 1;
    }
    return -1;
}

bool BSModifyOnceModifier::insertObjectAt(int index, DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_MODIFIER){
        if (index == 1){
            pOnDeactivateModifier = HkxSharedPtr((HkxObject *)obj);
        }else{
            pOnDeactivateModifier = HkxSharedPtr((HkxObject *)obj);
        }
    }else{
        return false;
    }
    return true;
}

bool BSModifyOnceModifier::removeObjectAt(int index){
    if (index == 0){
        pOnActivateModifier = HkxSharedPtr();
    }else if (index == 1){
        pOnDeactivateModifier = HkxSharedPtr();
    }else if (index == -1){
        pOnActivateModifier = HkxSharedPtr();
        pOnDeactivateModifier = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool BSModifyOnceModifier::hasChildren() const{
    if (pOnActivateModifier.data() || pOnDeactivateModifier.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> BSModifyOnceModifier::getChildren() const{
    QList<DataIconManager *> list;
    if (pOnActivateModifier.data()){
        list.append(static_cast<DataIconManager*>(pOnActivateModifier.data()));
    }
    if (pOnDeactivateModifier.data()){
        list.append(static_cast<DataIconManager*>(pOnDeactivateModifier.data()));
    }
    return list;
}

int BSModifyOnceModifier::getIndexOfObj(DataIconManager *obj) const{
    if (pOnActivateModifier.data() == (HkxObject *)obj){
        return 0;
    }else if (pOnDeactivateModifier.data() == (HkxObject *)obj){
        return 1;
    }
    return -1;
}

bool BSModifyOnceModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "pOnActivateModifier"){
            if (!pOnActivateModifier.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pOnActivateModifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "pOnDeactivateModifier"){
            if (!pOnDeactivateModifier.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pOnDeactivateModifier' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSModifyOnceModifier::write(HkxXMLWriter *writer){
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
        if (pOnActivateModifier.data()){
            refString = pOnActivateModifier.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pOnActivateModifier"), refString);
        if (pOnDeactivateModifier.data()){
            refString = pOnDeactivateModifier.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pOnDeactivateModifier"), refString);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (pOnActivateModifier.data() && !pOnActivateModifier.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pOnActivateModifier'!!!", true);
        }
        if (pOnDeactivateModifier.data() && !pOnDeactivateModifier.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'pOnDeactivateModifier'!!!", true);
        }
    }
    return true;
}

bool BSModifyOnceModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(pOnActivateModifier.getReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'pOnActivateModifier' is not a modifier!");
            setDataValidity(false);
        }
        pOnActivateModifier = *ptr;
    }
    ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(pOnDeactivateModifier.getReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'pOnDeactivateModifier' is not a modifier!");
            setDataValidity(false);
        }
        pOnDeactivateModifier = *ptr;
    }
    return true;
}

void BSModifyOnceModifier::unlink(){
    HkDynamicObject::unlink();
    pOnActivateModifier = HkxSharedPtr();
    pOnDeactivateModifier = HkxSharedPtr();
}

bool BSModifyOnceModifier::evaulateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!pOnActivateModifier.data()){
    }else if (!pOnDeactivateModifier.data()){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSModifyOnceModifier::~BSModifyOnceModifier(){
    refCount--;
}
