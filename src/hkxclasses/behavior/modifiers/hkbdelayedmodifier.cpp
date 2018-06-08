#include "hkbdelayedmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbDelayedModifier
*/

uint hkbDelayedModifier::refCount = 0;

QString hkbDelayedModifier::classname = "hkbDelayedModifier";

hkbDelayedModifier::hkbDelayedModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      delaySeconds(0),
      durationSeconds(0),
      secondsElapsed(0)
{
    setType(HKB_DELAYED_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "DelayedModifier"+QString::number(refCount);
}

QString hkbDelayedModifier::getClassname(){
    return classname;
}

QString hkbDelayedModifier::getName() const{
    return name;
}

bool hkbDelayedModifier::insertObjectAt(int , DataIconManager *obj){
    if (((HkxObject *)obj)->getType() == TYPE_MODIFIER){
        modifier = HkxSharedPtr((HkxObject *)obj);
        return true;
    }else{
        return false;
    }
}

bool hkbDelayedModifier::removeObjectAt(int index){
    if (index == 0 || index == -1){
        modifier = HkxSharedPtr();
    }else{
        return false;
    }
    return true;
}

bool hkbDelayedModifier::hasChildren() const{
    if (modifier.data()){
        return true;
    }
    return false;
}

QList<DataIconManager *> hkbDelayedModifier::getChildren() const{
    QList<DataIconManager *> list;
    if (modifier.data()){
        list.append(static_cast<DataIconManager*>(modifier.data()));
    }
    return list;
}

int hkbDelayedModifier::getIndexOfObj(DataIconManager *obj) const{
    if (modifier.data() == (HkxObject *)obj){
        return 0;
    }
    return -1;
}

bool hkbDelayedModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "modifier"){
            if (!modifier.readShdPtrReference(index, reader)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'modifier' reference!\nObject Reference: "+ref);
            }
        }else if (text == "delaySeconds"){
            delaySeconds = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'delaySeconds' data field!\nObject Reference: "+ref);
            }
        }else if (text == "durationSeconds"){
            durationSeconds = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'durationSeconds' data field!\nObject Reference: "+ref);
            }
        }else if (text == "secondsElapsed"){
            secondsElapsed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'secondsElapsed' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbDelayedModifier::write(HkxXMLWriter *writer){
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
        if (modifier.data()){
            refString = modifier.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("modifier"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("delaySeconds"), QString::number(delaySeconds, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("durationSeconds"), QString::number(durationSeconds, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("secondsElapsed"), QString::number(secondsElapsed, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (modifier.data() && !modifier.data()->write(writer)){
            WRITE_TO_LOG(getClassname()+": write()!\nUnable to write 'modifier'!!!");
        }
    }
    return true;
}

bool hkbDelayedModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        WRITE_TO_LOG(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findModifier(modifier.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getType() != TYPE_MODIFIER){
            WRITE_TO_LOG(getClassname()+": linkVar()!\nThe linked object 'modifier' is not a modifier!");
            setDataValidity(false);
        }
        modifier = *ptr;
    }
    return true;
}

void hkbDelayedModifier::unlink(){
    HkDynamicObject::unlink();
    modifier = HkxSharedPtr();
}

bool hkbDelayedModifier::evaluateDataValidity(){
    if (!HkDynamicObject::evaluateDataValidity() || (name == "") || (!modifier.data())){
        setDataValidity(false);
        return false;
    }else{
        setDataValidity(true);
        return true;
    }
}

hkbDelayedModifier::~hkbDelayedModifier(){
    refCount--;
}
