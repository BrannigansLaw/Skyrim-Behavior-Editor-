#include "hkbblendingtransitioneffect.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBlendingTransitionEffect
*/

uint hkbBlendingTransitionEffect::refCount = 0;

QString hkbBlendingTransitionEffect::classname = "hkbBlendingTransitionEffect";

QStringList hkbBlendingTransitionEffect::SelfTransitionMode = {
    "SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC",
    "SELF_TRANSITION_MODE_CONTINUE", "SELF_TRANSITION_MODE_RESET",
    "SELF_TRANSITION_MODE_BLEND"
};

QStringList hkbBlendingTransitionEffect::EventMode = {
    "EVENT_MODE_DEFAULT",
    "EVENT_MODE_PROCESS_ALL",
    "EVENT_MODE_IGNORE_FROM_GENERATOR",
    "EVENT_MODE_IGNORE_TO_GENERATOR"
};

QStringList hkbBlendingTransitionEffect::Flags = {
    "0",
    "FLAG_NONE",
    "FLAG_IGNORE_FROM_WORLD_FROM_MODEL",
    "FLAG_SYNC",
    "FLAG_IGNORE_TO_WORLD_FROM_MODEL"
};

QStringList hkbBlendingTransitionEffect::EndMode = {
    "END_MODE_NONE",
    "END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR",
    "END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR"
};

QStringList hkbBlendingTransitionEffect::BlendCurve = {
    "BLEND_CURVE_SMOOTH",
    "BLEND_CURVE_LINEAR",
    "BLEND_CURVE_LINEAR_TO_SMOOTH",
    "BLEND_CURVE_SMOOTH_TO_LINEAR"
};

hkbBlendingTransitionEffect::hkbBlendingTransitionEffect(HkxFile *parent, long ref)
    : HkDynamicObject(parent, ref),
      userData(0),
      selfTransitionMode(SelfTransitionMode.first()),
      eventMode(EventMode.first()),
      duration(0.2),
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

QVector<HkxObject *> hkbBlendingTransitionEffect::getChildrenOtherTypes() const{
    QVector <HkxObject *> list;
    if (variableBindingSet.data()){
        list.append(variableBindingSet.data());
    }
    return list;
}

bool hkbBlendingTransitionEffect::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "selfTransitionMode"){
            selfTransitionMode = reader.getElementValueAt(index);
            if (!SelfTransitionMode.contains(selfTransitionMode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'selfTransitionMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eventMode"){
            eventMode = reader.getElementValueAt(index);
            if (!EventMode.contains(eventMode)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'eventMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "duration"){
            duration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'duration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "toGeneratorStartTimeFraction"){
            toGeneratorStartTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'toGeneratorStartTimeFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "endMode"){
            endMode = reader.getElementValueAt(index);
            if (endMode == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'endMode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendCurve"){
            blendCurve = reader.getElementValueAt(index);
            if (blendCurve == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  readData()!\nFailed to properly read 'blendCurve' data field!\nObject Reference: "+ref);
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("duration"), QString::number(duration, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("toGeneratorStartTimeFraction"), QString::number(toGeneratorStartTimeFraction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), flags);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("endMode"), endMode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendCurve"), blendCurve);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  write()!\nUnable to write 'variableBindingSet'!!!");
        }
    }
    return true;
}

bool hkbBlendingTransitionEffect::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+":  link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbBlendingTransitionEffect::unlink(){
    HkDynamicObject::unlink();
}

QString hkbBlendingTransitionEffect::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (!SelfTransitionMode.contains(selfTransitionMode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid selfTransitionMode! Setting default value!\n");
        selfTransitionMode = SelfTransitionMode.first();
    }
    if (!EventMode.contains(eventMode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid eventMode! Setting default value!\n");
        eventMode = EventMode.first();
    }
    if (!Flags.contains(flags)){    //TO DO: fix...
        //isvalid = false;
        //errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid flags!\n");
    }
    if (!EndMode.contains(endMode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid endMode! Setting default value!\n");
        endMode = EndMode.first();
    }
    if (!BlendCurve.contains(blendCurve)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid blendCurve! Setting default value!\n");
        blendCurve = BlendCurve.first();
    }
    setDataValidity(isvalid);
    return errors;
}

hkbBlendingTransitionEffect::~hkbBlendingTransitionEffect(){
    refCount--;
}
