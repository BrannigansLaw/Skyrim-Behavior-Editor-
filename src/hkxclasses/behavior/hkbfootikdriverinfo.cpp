#include "hkbfootikdriverinfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbFootIkDriverInfo
*/

uint hkbFootIkDriverInfo::refCount = 0;

QString hkbFootIkDriverInfo::classname = "hkbFootIkDriverInfo";

hkbFootIkDriverInfo::hkbFootIkDriverInfo(HkxFile *parent, long ref)
    : HkxObject(parent, ref),
      raycastDistanceUp(0),
      raycastDistanceDown(0),
      originalGroundHeightMS(0),
      verticalOffset(0),
      collisionFilterInfo(0),
      forwardAlignFraction(0),
      sidewaysAlignFraction(0),
      sidewaysSampleWidth(0),
      lockFeetWhenPlanted(true),
      useCharacterUpVector(true),
      isQuadrupedNarrow(true)
{
    setType(HKB_FOOT_IK_DRIVER_INFO, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbFootIkDriverInfo::getClassname(){
    return classname;
}

void hkbFootIkDriverInfo::addLeg(){
    legs.append(hkbFootIkDriverInfoLeg());
}

void hkbFootIkDriverInfo::removeLegAt(int index){
    if (index >= 0 && index < legs.size()){
        legs.removeAt(index);
    }
}

hkbFootIkDriverInfo::hkbFootIkDriverInfoLeg * hkbFootIkDriverInfo::getLegAt(int index){
    if (index >= 0 && index < legs.size()){
        return &legs[index];
    }
    return nullptr;
}

bool hkbFootIkDriverInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "legs"){
            int numlegs = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numlegs; j++){
                legs.append(hkbFootIkDriverInfoLeg());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    text = reader.getNthAttributeValueAt(index, 0);
                    if (text == "kneeAxisLS"){
                        legs.last().kneeAxisLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'kneeAxisLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "footEndLS"){
                        legs.last().footEndLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'footEndLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "footPlantedAnkleHeightMS"){
                        legs.last().footPlantedAnkleHeightMS = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'footPlantedAnkleHeightMS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "footRaisedAnkleHeightMS"){
                        legs.last().footRaisedAnkleHeightMS = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'footRaisedAnkleHeightMS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "maxAnkleHeightMS"){
                        legs.last().maxAnkleHeightMS = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'maxAnkleHeightMS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "minAnkleHeightMS"){
                        legs.last().minAnkleHeightMS = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'minAnkleHeightMS' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "maxKneeAngleDegrees"){
                        legs.last().maxKneeAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'maxKneeAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "minKneeAngleDegrees"){
                        legs.last().minKneeAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'minKneeAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "maxAnkleAngleDegrees"){
                        legs.last().maxAnkleAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'maxAnkleAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "hipIndex"){
                        legs.last().hipIndex = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'hipIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "kneeIndex"){
                        legs.last().kneeIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'kneeIndex' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "ankleIndex"){
                        legs.last().ankleIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'ankleIndex' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
            index--;
        }else if (text == "raycastDistanceUp"){
            raycastDistanceUp = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'raycastDistanceUp' data field!\nObject Reference: "+ref);
            }
        }else if (text == "raycastDistanceDown"){
            raycastDistanceDown = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'raycastDistanceDown' data field!\nObject Reference: "+ref);
            }
        }else if (text == "raycastDistanceUp"){
            raycastDistanceUp = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'raycastDistanceUp' data field!\nObject Reference: "+ref);
            }
        }else if (text == "originalGroundHeightMS"){
            originalGroundHeightMS = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'originalGroundHeightMS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "verticalOffset"){
            verticalOffset = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'verticalOffset' data field!\nObject Reference: "+ref);
            }
        }else if (text == "collisionFilterInfo"){
            collisionFilterInfo = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'collisionFilterInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "forwardAlignFraction"){
            forwardAlignFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'forwardAlignFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sidewaysAlignFraction"){
            sidewaysAlignFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'sidewaysAlignFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sidewaysSampleWidth"){
            sidewaysSampleWidth = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'sidewaysSampleWidth' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lockFeetWhenPlanted"){
            lockFeetWhenPlanted = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'lockFeetWhenPlanted' data field!\nObject Reference: "+ref);
            }
        }else if (text == "useCharacterUpVector"){
            useCharacterUpVector = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'useCharacterUpVector' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isQuadrupedNarrow"){
            isQuadrupedNarrow = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'isQuadrupedNarrow' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbFootIkDriverInfo::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"legs", QString::number(legs.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < legs.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("kneeAxisLS"), legs[i].kneeAxisLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footEndLS"), legs[i].footEndLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footPlantedAnkleHeightMS"), QString::number(legs.at(i).footPlantedAnkleHeightMS, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("footRaisedAnkleHeightMS"), QString::number(legs.at(i).footRaisedAnkleHeightMS, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxAnkleHeightMS"), QString::number(legs.at(i).maxAnkleHeightMS, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minAnkleHeightMS"), QString::number(legs.at(i).minAnkleHeightMS, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxKneeAngleDegrees"), QString::number(legs.at(i).maxKneeAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minKneeAngleDegrees"), QString::number(legs.at(i).minKneeAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxAnkleAngleDegrees"), QString::number(legs.at(i).maxAnkleAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("hipIndex"), QString::number(legs.at(i).hipIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("kneeIndex"), QString::number(legs.at(i).kneeIndex));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ankleIndex"), QString::number(legs.at(i).ankleIndex));
            writer->writeLine(writer->object, false);
        }
        if (legs.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("raycastDistanceUp"), QString::number(raycastDistanceUp, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("raycastDistanceDown"), QString::number(raycastDistanceDown, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("originalGroundHeightMS"), QString::number(originalGroundHeightMS, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("verticalOffset"), QString::number(verticalOffset, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("collisionFilterInfo"), QString::number(collisionFilterInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("forwardAlignFraction"), QString::number(forwardAlignFraction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sidewaysAlignFraction"), QString::number(sidewaysAlignFraction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sidewaysSampleWidth"), QString::number(sidewaysSampleWidth, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lockFeetWhenPlanted"), getBoolAsString(lockFeetWhenPlanted));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("useCharacterUpVector"), getBoolAsString(useCharacterUpVector));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isQuadrupedNarrow"), getBoolAsString(isQuadrupedNarrow));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbFootIkDriverInfo::link(){
    return true;
}

void hkbFootIkDriverInfo::unlink(){
    //
}

QString hkbFootIkDriverInfo::evaluateDataValidity(){
    if (!legs.isEmpty()){
        setDataValidity(true);
        return QString();
    }
    setDataValidity(false);
    return QString();
}

hkbFootIkDriverInfo::~hkbFootIkDriverInfo(){
    refCount--;
}
