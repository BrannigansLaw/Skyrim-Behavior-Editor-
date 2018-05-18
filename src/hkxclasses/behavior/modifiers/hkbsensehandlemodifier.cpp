#include "hkbsensehandlemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: hkbSenseHandleModifier
*/

uint hkbSenseHandleModifier::refCount = 0;

QString hkbSenseHandleModifier::classname = "hkbSenseHandleModifier";

QStringList hkbSenseHandleModifier::SensingMode = {"SENSE_IN_NEARBY_RIGID_BODIES", "SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER", "SENSE_IN_OTHER_CHARACTER_RIGID_BODIES", "SENSE_IN_THIS_CHARACTER_RIGID_BODIES", "SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES", "SENSE_IN_GIVEN_RIGID_BODY", "SENSE_IN_OTHER_CHARACTER_SKELETON", "SENSE_IN_THIS_CHARACTER_SKELETON", "SENSE_IN_GIVEN_CHARACTER_SKELETON", "SENSE_IN_GIVEN_LOCAL_FRAME_GROUP"};

hkbSenseHandleModifier::hkbSenseHandleModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      minDistance(0),
      maxDistance(0),
      distanceOut(0),
      collisionFilterInfo(-1),
      sensorRagdollBoneIndex(-1),
      sensorAnimationBoneIndex(-1),
      extrapolateSensorPosition(false),
      keepFirstSensedHandle(false),
      foundHandleOut(false),
      sensingMode(SensingMode.first())
{
    setType(HKB_SENSE_HANDLE_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "SenseHandleModifier"+QString::number(refCount);
}

QString hkbSenseHandleModifier::getClassname(){
    return classname;
}

QString hkbSenseHandleModifier::getName() const{
    return name;
}

bool hkbSenseHandleModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "sensorLocalOffset"){
            sensorLocalOffset = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sensorLocalOffset' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ranges"){
            int numlegs = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numlegs; j++){
                ranges.append(hkRanges());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "id"){
                        ranges.last().event.id = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "payload"){
                        if (!ranges.last().event.payload.readShdPtrReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
                        }
                    }else if (text == "minDistance"){
                        ranges.last().minDistance = reader.getElementValueAt(index).toDouble(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'minDistance' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "maxDistance"){
                        ranges.last().maxDistance = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxDistance' data field!\nObject Reference: "+ref);
                        }
                    }else if (text == "ignoreHandle"){
                        ranges.last().ignoreHandle = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'ignoreHandle' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "handleOut"){
            if (!handleOut.readShdPtrReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'handleOut' reference!\nObject Reference: "+ref);
            }
        }else if (text == "handleIn"){
            if (!handleIn.readShdPtrReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'handleIn' reference!\nObject Reference: "+ref);
            }
        }else if (text == "localFrameName"){
            localFrameName = reader.getElementValueAt(index);
            if (localFrameName == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'localFrameName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sensorLocalFrameName"){
            sensorLocalFrameName = reader.getElementValueAt(index);
            if (sensorLocalFrameName == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sensorLocalFrameName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minDistance"){
            minDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'minDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxDistance"){
            maxDistance = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxDistance' data field!\nObject Reference: "+ref);
            }
        }else if (text == "distanceOut"){
            distanceOut = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'distanceOut' data field!\nObject Reference: "+ref);
            }
        }else if (text == "collisionFilterInfo"){
            collisionFilterInfo = reader.getElementValueAt(index).toLong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'collisionFilterInfo' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sensorRagdollBoneIndex"){
            sensorRagdollBoneIndex = reader.getElementValueAt(index).toLong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sensorRagdollBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sensorAnimationBoneIndex"){
            sensorAnimationBoneIndex = reader.getElementValueAt(index).toLong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sensorAnimationBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "sensingMode"){
            sensingMode = reader.getElementValueAt(index);
            if (!SensingMode.contains(sensingMode)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'sensingMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "extrapolateSensorPosition"){
            extrapolateSensorPosition = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'extrapolateSensorPosition' data field!\nObject Reference: "+ref);
            }
        }else if (text == "keepFirstSensedHandle"){
            keepFirstSensedHandle = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'keepFirstSensedHandle' data field!\nObject Reference: "+ref);
            }
        }else if (text == "foundHandleOut"){
            foundHandleOut = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'foundHandleOut' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbSenseHandleModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sensorLocalOffset"), sensorLocalOffset.getValueAsString());
        list1 = {writer->name, writer->numelements};
        list2 = {"ranges", QString::number(ranges.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < ranges.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(ranges.at(i).event.id));
            if (ranges.at(i).event.payload.data()){
                refString = ranges.at(i).event.payload.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minDistance"), QString::number(ranges.at(i).minDistance, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxDistance"), QString::number(ranges.at(i).maxDistance, char('f'), 6));
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ignoreHandle"), getBoolAsString(ranges.at(i).ignoreHandle));
            writer->writeLine(writer->object, false);
        }
        if (ranges.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        if (handleOut.data()){
            refString = handleOut.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleOut"), refString);
        if (handleIn.data()){
            refString = handleIn.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("handleIn"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localFrameName"), localFrameName, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sensorLocalFrameName"), sensorLocalFrameName, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minDistance"), QString::number(minDistance, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxDistance"), QString::number(maxDistance, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("distanceOut"), QString::number(distanceOut, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("collisionFilterInfo"), QString::number(collisionFilterInfo));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sensorRagdollBoneIndex"), QString::number(sensorRagdollBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sensorAnimationBoneIndex"), QString::number(sensorAnimationBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("sensingMode"), sensingMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("extrapolateSensorPosition"), getBoolAsString(extrapolateSensorPosition));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("keepFirstSensedHandle"), getBoolAsString(keepFirstSensedHandle));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("foundHandleOut"), getBoolAsString(foundHandleOut));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        for (int i = 0; i < ranges.size(); i++){
            if (ranges.at(i).event.payload.data() && !ranges.at(i).event.payload.data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload' at"+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

int hkbSenseHandleModifier::getNumberOfRanges() const{
    return ranges.size();
}

bool hkbSenseHandleModifier::isEventReferenced(int eventindex) const{
    for (auto i = 0; i < ranges.size(); i++){
        if (ranges.at(i).event.id == eventindex){
            return true;
        }
    }
    return false;
}

void hkbSenseHandleModifier::updateEventIndices(int eventindex){
    for (auto i = 0; i < ranges.size(); i++){
        if (ranges.at(i).event.id > eventindex){
            ranges[i].event.id--;
        }
    }
}

void hkbSenseHandleModifier::mergeEventIndex(int oldindex, int newindex){
    for (auto i = 0; i < ranges.size(); i++){
        if (ranges.at(i).event.id == oldindex){
            ranges[i].event.id = newindex;
        }
    }
}

bool hkbSenseHandleModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < ranges.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(ranges.at(i).event.payload.getShdPtrReference());
        if (ptr){
            if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                return false;
            }
            ranges[i].event.payload = *ptr;
        }
    }
    return true;
}

void hkbSenseHandleModifier::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < ranges.size(); i++){
        ranges[i].event.payload = HkxSharedPtr();
    }
}

bool hkbSenseHandleModifier::evaluateDataValidity(){  //Check for valid event id???
    for (int i = 0; i < ranges.size(); i++){
        if (ranges.at(i).event.payload.data() && ranges.at(i).event.payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            setDataValidity(false);
            return false;
        }
    }
    if (!HkDynamicObject::evaluateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (!SensingMode.contains(sensingMode)){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbSenseHandleModifier::~hkbSenseHandleModifier(){
    refCount--;
}
