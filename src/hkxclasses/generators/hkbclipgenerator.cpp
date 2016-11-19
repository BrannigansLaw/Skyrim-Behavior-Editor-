#include "hkbclipgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbClipGenerator
*/

uint hkbClipGenerator::refCount = 0;

QString hkbClipGenerator::classname = "hkbClipGenerator";

QStringList hkbClipGenerator::PlaybackMode = {"MODE_SINGLE_PLAY", "MODE_LOOPING", "MODE_USER_CONTROLLED", "MODE_PING_PONG", "MODE_COUNT"};
QStringList hkbClipGenerator::ClipFlags = {"0", "FLAG_CONTINUE_MOTION_AT_END", "FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE", "FLAG_MIRROR", "FLAG_FORCE_DENSE_POSE", "FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS", "FLAG_IGNORE_MOTION"};

hkbClipGenerator::hkbClipGenerator(BehaviorFile *parent, long ref)
    : hkbGenerator(parent, ref),
      userData(0),
      cropStartAmountLocalTime(0),
      cropEndAmountLocalTime(0),
      startTime(0),
      playbackSpeed(0),
      enforcedDuration(0),
      userControlledTimeFraction(0),
      animationBindingIndex(-1),
      mode(PlaybackMode.first()),
      flags(ClipFlags.first())
{
    setType(HKB_CLIP_GENERATOR, TYPE_GENERATOR);
    getParentFile()->addObjectToFile(this, ref);refCount++;
    name = "Clip Generator "+QString::number(refCount);
}

QString hkbClipGenerator::getClassname(){
    return classname;
}

QString hkbClipGenerator::getName() const{
    return name;
}

bool hkbClipGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationName"){
            animationName = reader.getElementValueAt(index);
            if (animationName == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'animationName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "triggers"){
            if (!triggers.readReference(index, reader)){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'triggers' reference!\nObject Reference: "+ref);
            }
        }else if (text == "cropStartAmountLocalTime"){
            cropStartAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'cropStartAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "cropEndAmountLocalTime"){
            cropEndAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'cropEndAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startTime"){
            startTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'startTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "playbackSpeed"){
            playbackSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'playbackSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enforcedDuration"){
            enforcedDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'enforcedDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userControlledTimeFraction"){
            userControlledTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'userControlledTimeFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationBindingIndex"){
            animationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'animationBindingIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog("hkbClipGenerator: readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbClipGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbClipGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //triggers
    HkxObjectExpSharedPtr *ptr = getParentFile()->findHkxObject(triggers.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
            writeToLog("hkbClipGenerator: link()!\n'triggers' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
        }
        triggers = *ptr;
    }
    return true;
}

void hkbClipGenerator::unlink(){
    HkDynamicObject::unlink();
    triggers = HkxObjectExpSharedPtr();
}

bool hkbClipGenerator::evaulateDataValidity(){
    bool valid = true;
    QStringList list = flags.split('|');
    for (int i = 0; i < list.size(); i++){
        if (!ClipFlags.contains(list.at(i))){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!PlaybackMode.contains(mode)){
    }else if (triggers.data() && triggers.data()->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
    }else if (name == ""){
    }else if (animationName == ""){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbClipGenerator::~hkbClipGenerator(){
    refCount--;
}
