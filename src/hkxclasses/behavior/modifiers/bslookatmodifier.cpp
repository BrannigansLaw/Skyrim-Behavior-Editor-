#include "bslookatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

/*
 * CLASS: BSLookAtModifier
*/

uint BSLookAtModifier::refCount = 0;

QString BSLookAtModifier::classname = "BSLookAtModifier";

BSLookAtModifier::BSLookAtModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      lookAtTarget(false),
      limitAngleDegrees(0),
      limitAngleThresholdDegrees(0),
      continueLookOutsideOfLimit(false),
      onGain(0),
      offGain(0),
      useBoneGains(false),
      targetOutsideLimits(false),
      id(-1),
      lookAtCamera(false),
      lookAtCameraX(0),
      lookAtCameraY(0),
      lookAtCameraZ(0)
{
    setType(BS_LOOK_AT_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "LookAtModifier"+QString::number(refCount);
}

QString BSLookAtModifier::getClassname(){
    return classname;
}

QString BSLookAtModifier::getName() const{
    return name;
}

bool BSLookAtModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lookAtTarget"){
            lookAtTarget = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'lookAtTarget' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bones"){
            int numbones = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numbones; j++){
                bones.append(BsBone());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "index"){
                        bones.last().index = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'index' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fwdAxisLS"){
                        bones.last().fwdAxisLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fwdAxisLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "limitAngleDegrees"){
                        bones.last().limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "onGain"){
                        bones.last().onGain = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "offGain"){
                        bones.last().offGain = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enabled"){
                        bones.last().enabled = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enabled' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
            index--;
        }else if (text == "eyeBones"){
            int numbones = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numbones; j++){
                eyeBones.append(BsBone());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "index"){
                        eyeBones.last().index = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'index' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "fwdAxisLS"){
                        eyeBones.last().fwdAxisLS = readVector4(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'fwdAxisLS' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "limitAngleDegrees"){
                        eyeBones.last().limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "onGain"){
                        eyeBones.last().onGain = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "offGain"){
                        eyeBones.last().offGain = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "enabled"){
                        eyeBones.last().enabled = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'enabled' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
            if (numbones > 0){
                index--;
            }
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleThresholdDegrees"){
            limitAngleThresholdDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'limitAngleThresholdDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "continueLookOutsideOfLimit"){
            continueLookOutsideOfLimit = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'continueLookOutsideOfLimit' data field!\nObject Reference: "+ref);
            }
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "useBoneGains"){
            useBoneGains = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'useBoneGains' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetLocation"){
            targetLocation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'targetLocation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetOutsideLimits"){
            targetOutsideLimits = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'targetOutsideLimits' data field!\nObject Reference: "+ref);
            }
        }else if (text == "id"){
            id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!payload.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }else if (text == "lookAtCamera"){
            lookAtCamera = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'lookAtCamera' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lookAtCameraX"){
            lookAtCameraX = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'lookAtCameraX' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lookAtCameraY"){
            lookAtCameraY = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'lookAtCameraY' data field!\nObject Reference: "+ref);
            }
        }else if (text == "lookAtCameraZ"){
            lookAtCameraZ = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": readData()!\nFailed to properly read 'lookAtCameraZ' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSLookAtModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lookAtTarget"), getBoolAsString(lookAtTarget));
        list1 = {writer->name, writer->numelements};
        list2 = {"bones", QString::number(bones.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < bones.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("index"), QString::number(bones.at(i).index));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fwdAxisLS"), bones[i].fwdAxisLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(bones.at(i).limitAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(bones.at(i).onGain, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(bones.at(i).offGain, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enabled"), getBoolAsString(bones.at(i).enabled));
            writer->writeLine(writer->object, false);
        }
        if (bones.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list2 = {"eyeBones", QString::number(eyeBones.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < eyeBones.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("index"), QString::number(eyeBones.at(i).index));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fwdAxisLS"), eyeBones[i].fwdAxisLS.getValueAsString());
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(eyeBones.at(i).limitAngleDegrees, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(eyeBones.at(i).onGain, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(eyeBones.at(i).offGain, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enabled"), getBoolAsString(eyeBones.at(i).enabled));
            writer->writeLine(writer->object, false);
        }
        if (eyeBones.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(limitAngleDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleThresholdDegrees"), QString::number(limitAngleThresholdDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("continueLookOutsideOfLimit"), getBoolAsString(continueLookOutsideOfLimit));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(onGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(offGain, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("useBoneGains"), getBoolAsString(useBoneGains));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetLocation"), targetLocation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetOutsideLimits"), getBoolAsString(targetOutsideLimits));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetOutOfLimitEvent"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(id));
        if (payload.data()){
            refString = payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lookAtCamera"), getBoolAsString(lookAtCamera));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lookAtCameraX"), QString::number(lookAtCameraX, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lookAtCameraY"), QString::number(lookAtCameraY, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("lookAtCameraZ"), QString::number(lookAtCameraZ, char('f'), 6));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (payload.data() && !payload.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": write()!\nUnable to write 'payload'!!!");
        }
    }
    return true;
}

int BSLookAtModifier::getNumberOfBones() const{
    return bones.size();
}

int BSLookAtModifier::getNumberOfEyeBones() const{
    return eyeBones.size();
}

bool BSLookAtModifier::isEventReferenced(int eventindex) const{
    if (id == eventindex){
        return true;
    }
    return false;
}

void BSLookAtModifier::updateEventIndices(int eventindex){
    if (id > eventindex){
        id--;
    }
}

void BSLookAtModifier::mergeEventIndex(int oldindex, int newindex){
    if (id == oldindex){
        id = newindex;
    }
}

void BSLookAtModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            auto fixIndex = [&](int & id){
                thiseventname = recdata->getEventNameAt(id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                id = eventindex;
            };
            fixIndex(id);
        }
        setIsMerged(true);
    }
}


void BSLookAtModifier::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (payload.data()){
        ref++;
        payload.data()->updateReferences(ref);
    }
}

QVector<HkxObject *> BSLookAtModifier::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (payload.data()){
        list.append(payload.data());
    }
    return list;
}

bool BSLookAtModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(payload.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            LogFile::writeToLog(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
        }
        payload = *ptr;
    }
    return true;
}

void BSLookAtModifier::unlink(){
    HkDynamicObject::unlink();
    payload = HkxSharedPtr();
}

bool BSLookAtModifier::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    if (bones.isEmpty()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": bones is empty!\n");
    }else{
        for (auto i = 0; i < bones.size(); i++){
            if (bones.at(i).index >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
                isvalid = false;
                errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid bones at index: "+QString::number(i)+"\n");
            }
        }
        for (auto i = 0; i < eyeBones.size(); i++){
            if (eyeBones.at(i).index >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
                isvalid = false;
                errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid bones at index: "+QString::number(i)+"\n");
            }
        }
    }
    if (!HkDynamicObject::evaluateDataValidity()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": event id out of range!\n");
    }
    if (payload.data() && payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
        isvalid = false;
        errors.append(getParentFile()->fileName().section("/", -1, -1)+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid payload type! Signature: "+QString::number(payload.data()->getSignature(), 16)+"\n");
    }
    if (errors != ""){
        LogFile::writeToLog(errors);
    }
    setDataValidity(isvalid);
    return isvalid;
}

BSLookAtModifier::~BSLookAtModifier(){
    refCount--;
}
