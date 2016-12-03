#include "hkbblendingtransitioneffect.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBlendingTransitionEffect
*/

uint hkbBlendingTransitionEffect::refCount = 0;

QString hkbBlendingTransitionEffect::classname = "hkbBlendingTransitionEffect";

QStringList hkbBlendingTransitionEffect::SelfTransitionMode = {"SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC", "SELF_TRANSITION_MODE_CONTINUE", "SELF_TRANSITION_MODE_RESET", "SELF_TRANSITION_MODE_BLEND"};
QStringList hkbBlendingTransitionEffect::EventMode = {"EVENT_MODE_DEFAULT", "EVENT_MODE_PROCESS_ALL", "EVENT_MODE_IGNORE_FROM_GENERATOR", "EVENT_MODE_IGNORE_TO_GENERATOR"};
QStringList hkbBlendingTransitionEffect::Flags = {"FLAG_NONE", "FLAG_IGNORE_FROM_WORLD_FROM_MODEL", "FLAG_SYNC", "FLAG_IGNORE_TO_WORLD_FROM_MODEL"};
QStringList hkbBlendingTransitionEffect::EndMode = {"END_MODE_NONE", "END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR", "END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR"};
QStringList hkbBlendingTransitionEffect::BlendCurve = {"BLEND_CURVE_SMOOTH", "BLEND_CURVE_LINEAR", "BLEND_CURVE_LINEAR_TO_SMOOTH", "BLEND_CURVE_SMOOTH_TO_LINEAR"};

hkbBlendingTransitionEffect::hkbBlendingTransitionEffect(BehaviorFile *parent, long ref)
    : HkDynamicObject(parent, ref),
      userData(0),
      selfTransitionMode(SelfTransitionMode.first()),
      eventMode(EventMode.first()),
      duration(1),
      toGeneratorStartTimeFraction(0),
      flags(Flags.first()),
      endMode(EndMode.first()),
      blendCurve(BlendCurve.first())
{
    setType(HKB_BLENDING_TRANSITION_EFFECT, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BlendingTransitionEffect"+QString::number(refCount);
}

QString hkbBlendingTransitionEffect::getClassname(){
    return classname;
}

QString hkbBlendingTransitionEffect::getName() const{
    return name;
}

bool hkbBlendingTransitionEffect::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'selfTransitionMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eventMode"){
            eventMode = reader.getElementValueAt(index);
            if (!EventMode.contains(eventMode)){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'eventMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "duration"){
            duration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'duration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "toGeneratorStartTimeFraction"){
            toGeneratorStartTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'toGeneratorStartTimeFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endMode"){
            endMode = reader.getElementValueAt(index);
            if (endMode == ""){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'endMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendCurve"){
            blendCurve = reader.getElementValueAt(index);
            if (blendCurve == ""){
                writeToLog(getClassname()+":  readData()!\nFailed to properly read 'blendCurve' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbBlendingTransitionEffect::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("selfTransitionMode"), selfTransitionMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eventMode"), eventMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("duration"), QString::number(duration));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("toGeneratorStartTimeFraction"), QString::number(toGeneratorStartTimeFraction));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), flags);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("endMode"), endMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendCurve"), blendCurve);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+":  write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbBlendingTransitionEffect::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+":  link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbBlendingTransitionEffect::unlink(){
    HkDynamicObject::unlink();
}

bool hkbBlendingTransitionEffect::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!SelfTransitionMode.contains(selfTransitionMode)){
    }else if (!EventMode.contains(eventMode)){
    }else if (!Flags.contains(flags)){
    }else if (!EndMode.contains(endMode)){
    }else if (!BlendCurve.contains(blendCurve)){
    }else if (name == ""){
    }else{
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbBlendingTransitionEffect::~hkbBlendingTransitionEffect(){
    refCount--;
}
