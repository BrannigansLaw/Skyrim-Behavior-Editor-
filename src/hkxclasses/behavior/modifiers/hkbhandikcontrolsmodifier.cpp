#include "hkbhandikcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

uint hkbHandIkControlsModifier::refCount = 0;

QString hkbHandIkControlsModifier::classname = "hkbHandIkControlsModifier";

QStringList hkbHandIkControlsModifier::HandleChangeMode = {
    "HANDLE_CHANGE_MODE_ABRUPT",
    "HANDLE_CHANGE_MODE_CONSTANT_VELOCITY"
};

hkbHandIkControlsModifier::hkbHandIkControlsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_HAND_IK_CONTROLS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "HandIkControlsModifier"+QString::number(refCount);
}

QString hkbHandIkControlsModifier::getClassname(){
    return classname;
}

QString hkbHandIkControlsModifier::getName() const{
    return name;
}

bool hkbHandIkControlsModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
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
        }else if (text == "hands"){
            int numhands = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numhands; j++){
                hands.append(hkHand());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "targetPosition"){
                        hands.last().controlData.targetPosition = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetPosition' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "targetRotation"){
                        hands.last().controlData.targetRotation = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetRotation' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "targetNormal"){
                        hands.last().controlData.targetNormal = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetNormal' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "targetHandle"){
                        if (!hands.last().controlData.targetHandle.readShdPtrReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetHandle' reference!\nObject Reference: "+ref);
                        }
                    }else if (text == "transformOnFraction"){
                        hands.last().controlData.transformOnFraction = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'transformOnFraction' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "normalOnFraction"){
                        hands.last().controlData.normalOnFraction = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'normalOnFraction' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "fadeInDuration"){
                        hands.last().controlData.fadeInDuration = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'fadeInDuration' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "fadeOutDuration"){
                        hands.last().controlData.fadeOutDuration = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'fadeOutDuration' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "extrapolationTimeStep"){
                        hands.last().controlData.extrapolationTimeStep = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'extrapolationTimeStep' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "handleChangeSpeed"){
                        hands.last().controlData.handleChangeSpeed = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'handleChangeSpeed' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "handleChangeMode"){
                        hands.last().controlData.handleChangeMode = reader.getElementValueAt(index);
                        if (!HandleChangeMode.contains(hands.last().controlData.handleChangeMode)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'handleChangeMode' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "fixUp"){
                        hands.last().controlData.fixUp = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'fixUp' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "handIndex"){
                        hands.last().handIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'handIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "enable"){
                        hands.last().enable = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'hands.enable' data field!\nObject Reference: "+ref);
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

bool hkbHandIkControlsModifier::write(HkxXMLWriter *writer){
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
        list1 = {writer->name, writer->numelements};
        list2 = {"hands", QString::number(hands.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < hands.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("controlData"), "");
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetPosition"), hands.at(i).controlData.targetPosition.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetRotation"), hands.at(i).controlData.targetRotation.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetNormal"), hands.at(i).controlData.targetNormal.getValueAsString());
            if (hands.at(i).controlData.targetHandle.data()){
                refString = hands.at(i).controlData.targetHandle.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetHandle"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transformOnFraction"), QString::number(hands.at(i).controlData.transformOnFraction, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("normalOnFraction"), QString::number(hands.at(i).controlData.normalOnFraction, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fadeInDuration"), QString::number(hands.at(i).controlData.fadeInDuration, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fadeOutDuration"), QString::number(hands.at(i).controlData.fadeOutDuration, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("extrapolationTimeStep"), QString::number(hands.at(i).controlData.extrapolationTimeStep, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleChangeSpeed"), QString::number(hands.at(i).controlData.handleChangeSpeed, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleChangeMode"), hands.at(i).controlData.handleChangeMode);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fixUp"), getBoolAsString(hands.at(i).controlData.fixUp));
            writer->writeLine(writer->object, false);
            writer->writeLine(writer->parameter, false);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handIndex"), QString::number(hands.at(i).handIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(hands.at(i).enable));
            writer->writeLine(writer->object, false);
        }
        if (hands.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

int hkbHandIkControlsModifier::getNumberOfHands() const{
    return hands.size();
}

bool hkbHandIkControlsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbHandIkControlsModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbHandIkControlsModifier::evaluateDataValidity(){  //Check for valid event id???
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbHandIkControlsModifier::~hkbHandIkControlsModifier(){
    refCount--;
}
