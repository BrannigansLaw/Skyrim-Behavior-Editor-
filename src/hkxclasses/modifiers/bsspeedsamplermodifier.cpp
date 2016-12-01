#include "bsspeedsamplermodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: BSSpeedSamplerModifier
*/

uint BSSpeedSamplerModifier::refCount = 0;

QString BSSpeedSamplerModifier::classname = "BSSpeedSamplerModifier";

BSSpeedSamplerModifier::BSSpeedSamplerModifier(BehaviorFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      state(-1),
      direction(0),
      goalSpeed(0),
      speedOut(0)
{
    setType(BS_SPEED_SAMPLER_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "SpeedSamplerModifier"+QString::number(refCount);
}

QString BSSpeedSamplerModifier::getClassname(){
    return classname;
}

QString BSSpeedSamplerModifier::getName() const{
    return name;
}

bool BSSpeedSamplerModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "state"){
            state = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'state' data field!\nObject Reference: "+ref);
            }
        }else if (text == "direction"){
            direction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'direction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "goalSpeed"){
            goalSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'goalSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "speedOut"){
            speedOut = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'speedOut' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSSpeedSamplerModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("state"), QString::number(state));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("direction"), getDoubleAsString(direction));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("goalSpeed"), getDoubleAsString(goalSpeed));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("speedOut"), getDoubleAsString(speedOut));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool BSSpeedSamplerModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void BSSpeedSamplerModifier::unlink(){
    HkDynamicObject::unlink();
}

bool BSSpeedSamplerModifier::evaulateDataValidity(){  //Check for valid state???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSSpeedSamplerModifier::~BSSpeedSamplerModifier(){
    refCount--;
}
