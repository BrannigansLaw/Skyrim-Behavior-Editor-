#include "hkbhandikdriverinfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbHandIkDriverInfo
*/

uint hkbHandIkDriverInfo::refCount = 0;

QString hkbHandIkDriverInfo::classname = "hkbHandIkDriverInfo";

QStringList hkbHandIkDriverInfo::BlendCurve = {"BLEND_CURVE_SMOOTH", "BLEND_CURVE_LINEAR", "BLEND_CURVE_LINEAR_TO_SMOOTH", "BLEND_CURVE_SMOOTH_TO_LINEAR"};

hkbHandIkDriverInfo::hkbHandIkDriverInfo(HkxFile *parent, long ref)
    : HkxObject(parent, ref),
      fadeInOutCurve(BlendCurve.first())
{
    setType(HKB_HAND_IK_DRIVER_INFO, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbHandIkDriverInfo::getClassname(){
    return classname;
}

void hkbHandIkDriverInfo::addHand(){
    hands.append(hkbHandIkDriverInfoHand());
}

void hkbHandIkDriverInfo::removeHandAt(int index){
    if (index >= 0 && index < hands.size()){
        hands.removeAt(index);
    }
}

hkbHandIkDriverInfo::hkbHandIkDriverInfoHand * hkbHandIkDriverInfo::getHandAt(int index){
    if (index >= 0 && index < hands.size()){
        return &hands[index];
    }
    return NULL;
}

bool hkbHandIkDriverInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "hands"){
            int numhands = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numhands; j++){
                hands.append(hkbHandIkDriverInfoHand());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "elbowAxisLS"){
                        hands.last().elbowAxisLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'elbowAxisLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "backHandNormalLS"){
                        hands.last().backHandNormalLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'backHandNormalLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "handOffsetLS"){
                        hands.last().handOffsetLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'handOffsetLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "handOrienationOffsetLS"){
                        hands.last().handOrienationOffsetLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'handOrienationOffsetLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "maxElbowAngleDegrees"){
                        hands.last().maxElbowAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxElbowAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "minElbowAngleDegrees"){
                        hands.last().minElbowAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'minElbowAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "shoulderIndex"){
                        hands.last().shoulderIndex = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'shoulderIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "shoulderSiblingIndex"){
                        hands.last().shoulderSiblingIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'shoulderSiblingIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "elbowIndex"){
                        hands.last().elbowIndex = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'elbowIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "elbowSiblingIndex"){
                        hands.last().elbowSiblingIndex = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'elbowSiblingIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "wristIndex"){
                        hands.last().wristIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'wristIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "enforceEndPosition"){
                        hands.last().enforceEndPosition = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'enforceEndPosition' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "enforceEndRotation"){
                        hands.last().enforceEndRotation = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'enforceEndRotation' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "localFrameName"){
                        hands.last().localFrameName = reader.getElementValueAt(index);
                        /*if (hands.last().localFrameName == ""){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'localFrameName' data field!\nObject Reference: "+ref);
                        }*/
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "fadeInOutCurve"){
            fadeInOutCurve = reader.getElementValueAt(index);
            if (!BlendCurve.contains(fadeInOutCurve)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fadeInOutCurve' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbHandIkDriverInfo::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"hands", QString::number(hands.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < hands.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("elbowAxisLS"), hands[i].elbowAxisLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("backHandNormalLS"), hands[i].backHandNormalLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handOffsetLS"), hands[i].handOffsetLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handOrienationOffsetLS"), hands[i].handOrienationOffsetLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxElbowAngleDegrees"), QString::number(hands.at(i).maxElbowAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minElbowAngleDegrees"), QString::number(hands.at(i).minElbowAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("shoulderIndex"), QString::number(hands.at(i).shoulderIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("shoulderSiblingIndex"), QString::number(hands.at(i).shoulderSiblingIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("elbowIndex"), QString::number(hands.at(i).elbowIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("elbowSiblingIndex"), QString::number(hands.at(i).elbowSiblingIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("wristIndex"), QString::number(hands.at(i).wristIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enforceEndPosition"), getBoolAsString(hands.at(i).enforceEndPosition));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enforceEndRotation"), getBoolAsString(hands.at(i).enforceEndRotation));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localFrameName"), hands.at(i).localFrameName);
            writer->writeLine(writer->object, false);
        }
        if (hands.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fadeInOutCurve"), fadeInOutCurve);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbHandIkDriverInfo::link(){
    return true;
}

void hkbHandIkDriverInfo::unlink(){
    //
}

bool hkbHandIkDriverInfo::evaulateDataValidity(){
    if (!hands.isEmpty()){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbHandIkDriverInfo::~hkbHandIkDriverInfo(){
    refCount--;
}
