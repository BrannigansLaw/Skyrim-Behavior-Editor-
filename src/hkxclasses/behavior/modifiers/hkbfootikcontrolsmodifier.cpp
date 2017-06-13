#include "hkbfootikcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbFootIkControlsModifier
*/

uint hkbFootIkControlsModifier::refCount = 0;

QString hkbFootIkControlsModifier::classname = "hkbFootIkControlsModifier";

hkbFootIkControlsModifier::hkbFootIkControlsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_FOOT_IK_CONTROLS_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbFootIkControlsModifier"+QString::number(refCount);
}

QString hkbFootIkControlsModifier::getClassname(){
    return classname;
}

QString hkbFootIkControlsModifier::getName() const{
    return name;
}

bool hkbFootIkControlsModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "onOffGain"){
            gains.onOffGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'onOffGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "groundAscendingGain"){
            gains.groundAscendingGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'groundAscendingGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "groundDescendingGain"){
            gains.groundDescendingGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'groundDescendingGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "footPlantedGain"){
            gains.footPlantedGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'footPlantedGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "footRaisedGain"){
            gains.footRaisedGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'footRaisedGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "footUnlockGain"){
            gains.footUnlockGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'footUnlockGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelFeedbackGain"){
            gains.worldFromModelFeedbackGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'worldFromModelFeedbackGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "errorUpDownBias"){
            gains.errorUpDownBias = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'errorUpDownBias' data field!\nObject Reference: "+ref);
            }
        }else if (text == "alignWorldFromModelGain"){
            gains.alignWorldFromModelGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'alignWorldFromModelGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "hipOrientationGain"){
            gains.hipOrientationGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'hipOrientationGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxKneeAngleDifference"){
            gains.maxKneeAngleDifference = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxKneeAngleDifference' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ankleOrientationGain"){
            gains.ankleOrientationGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'ankleOrientationGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "legs"){
            int numlegs = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numlegs; j++){
                legs.append(BSLeg());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "groundPosition"){
                        legs.last().groundPosition = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'groundPosition' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "id"){
                        legs.last().id = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "payload"){
                        if (!legs.last().payload.readReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                        }
                    }else if (text == "verticalError"){
                        legs.last().verticalError = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'verticalError' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "hitSomething"){
                        legs.last().hitSomething = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'hitSomething' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "isPlantedMS"){
                        legs.last().isPlantedMS = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'isPlantedMS' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "errorOutTranslation"){
            errorOutTranslation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'errorOutTranslation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "alignWithGroundRotation"){
            alignWithGroundRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'alignWithGroundRotation' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbFootIkControlsModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("controlData"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("gains"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onOffGain"), QString::number(gains.onOffGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("groundAscendingGain"), QString::number(gains.groundAscendingGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("groundDescendingGain"), QString::number(gains.groundDescendingGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footPlantedGain"), QString::number(gains.footPlantedGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footRaisedGain"), QString::number(gains.footRaisedGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footUnlockGain"), QString::number(gains.footUnlockGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldFromModelFeedbackGain"), QString::number(gains.worldFromModelFeedbackGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("errorUpDownBias"), QString::number(gains.errorUpDownBias, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("alignWorldFromModelGain"), QString::number(gains.alignWorldFromModelGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("hipOrientationGain"), QString::number(gains.hipOrientationGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxKneeAngleDifference"), QString::number(gains.maxKneeAngleDifference, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ankleOrientationGain"), QString::number(gains.ankleOrientationGain, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        list1 = {writer->name, writer->numelements};
        list2 = {"legs", QString::number(legs.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < legs.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fwdAxisLS"), legs[i].groundPosition.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(legs.at(i).id));
            if (legs.at(i).payload.data()){
                refString = legs.at(i).payload.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalError"), QString::number(legs.at(i).verticalError, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("hitSomething"), getBoolAsString(legs.at(i).hitSomething));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isPlantedMS"), getBoolAsString(legs.at(i).isPlantedMS));
            writer->writeLine(writer->object, false);
        }
        if (legs.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("errorOutTranslation"), errorOutTranslation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("alignWithGroundRotation"), alignWithGroundRotation.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        for (int i = 0; i < legs.size(); i++){
            if (legs.at(i).payload.data() && !legs.at(i).payload.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbFootIkControlsModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < legs.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(legs.at(i).payload.getReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            legs[i].payload = *ptr;
        }
    }
    return true;
}

void hkbFootIkControlsModifier::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < legs.size(); i++){
        legs[i].payload = HkxSharedPtr();
    }
}

bool hkbFootIkControlsModifier::evaulateDataValidity(){  //Check for valid event id???
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

hkbFootIkControlsModifier::~hkbFootIkControlsModifier(){
    refCount--;
}
