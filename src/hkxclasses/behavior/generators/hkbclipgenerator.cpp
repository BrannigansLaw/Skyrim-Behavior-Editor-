#include "hkbclipgenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbcliptriggerarray.h"

#define MAX_TRIES 5

uint hkbClipGenerator::refCount = 0;

QString hkbClipGenerator::classname = "hkbClipGenerator";

QStringList hkbClipGenerator::PlaybackMode = {"MODE_SINGLE_PLAY", "MODE_LOOPING", "MODE_USER_CONTROLLED", "MODE_PING_PONG", "MODE_COUNT"};

hkbClipGenerator::hkbClipGenerator(HkxFile *parent, long ref, bool addToAnimData, const QString &animationname)
    : hkbGenerator(parent, ref),
      userData(0),
      animationName(animationname),
      cropStartAmountLocalTime(0),
      cropEndAmountLocalTime(0),
      startTime(0),
      playbackSpeed(1),
      enforcedDuration(0),
      userControlledTimeFraction(0),
      animationBindingIndex(-1),
      mode(PlaybackMode.first()),
      flags("0")
{
    setType(HKB_CLIP_GENERATOR, TYPE_GENERATOR);
    BehaviorFile *par = static_cast<BehaviorFile *>(getParentFile());
    if (!getParentFile()){
        CRITICAL_ERROR_MESSAGE("hkbClipGenerator(): Parent file is nullptr!");
    }
    par->addObjectToFile(this, ref);
    refCount++;
    name = "ClipGenerator"+QString::number(refCount);
    if (animationname == ""){
        animationName = par->getAnimationNameAt(0);
    }else {
        name = animationname;
    }
    /*if (addToAnimData && !par->addClipGenToAnimationData(name)){
        int count = 0;
        bool added = par->addClipGenToAnimationData(name);
        while (!added && count < MAX_TRIES){
            name = "ClipGenerator"+QString::number(refCount)+QString::number(qrand());
            added = par->addClipGenToAnimationData(name);
            count++;
        }
        if (!added){
            CRITICAL_ERROR_MESSAGE("hkbClipGenerator::hkbClipGenerator(): The clip generator could not be added to the animation data!");
        }
    }*/
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
            if (!variableBindingSet.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationName"){
            animationName = reader.getElementValueAt(index);
            if (animationName == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'animationName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "triggers"){
            if (!triggers.readShdPtrReference(index, reader)){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'triggers' reference!\nObject Reference: "+ref);
            }
        }else if (text == "cropStartAmountLocalTime"){
            cropStartAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'cropStartAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "cropEndAmountLocalTime"){
            cropEndAmountLocalTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'cropEndAmountLocalTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startTime"){
            startTime = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'startTime' data field!\nObject Reference: "+ref);
            }
        }else if (text == "playbackSpeed"){
            playbackSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'playbackSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enforcedDuration"){
            enforcedDuration = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'enforcedDuration' data field!\nObject Reference: "+ref);
            }
        }else if (text == "userControlledTimeFraction"){
            userControlledTimeFraction = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'userControlledTimeFraction' data field!\nObject Reference: "+ref);
            }
        }else if (text == "animationBindingIndex"){
            animationBindingIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'animationBindingIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbClipGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("animationName"), animationName);
        if (triggers.data()){
            refString = triggers.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("triggers"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("cropStartAmountLocalTime"), QString::number(cropStartAmountLocalTime, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("cropEndAmountLocalTime"), QString::number(cropEndAmountLocalTime, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startTime"), QString::number(startTime, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("playbackSpeed"), QString::number(playbackSpeed, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enforcedDuration"), QString::number(enforcedDuration, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userControlledTimeFraction"), QString::number(userControlledTimeFraction, char('f'), 6));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("animationBindingIndex"), QString::number(animationBindingIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("mode"), mode);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), flags);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!");
        }
        if (triggers.data() && !triggers.data()->write(writer)){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": write()!\nUnable to write 'triggers'!!!");
        }
    }
    return true;
}

int hkbClipGenerator::getNumberOfTriggers() const{
    if (triggers.data()){
        return static_cast<hkbClipTriggerArray *>(triggers.data())->triggers.size();
    }
    return 0;
}

QString hkbClipGenerator::getAnimationName() const{
    return animationName;
}

bool hkbClipGenerator::isEventReferenced(int eventindex) const{
    if (triggers.constData() && triggers.constData()->isEventReferenced(eventindex)){
        return true;
    }
    return false;
}

void hkbClipGenerator::updateEventIndices(int eventindex){
    if (triggers.data()){
        triggers.data()->updateEventIndices(eventindex);
    }
}

void hkbClipGenerator::mergeEventIndex(int oldindex, int newindex){
    if (triggers.data()){
        triggers.data()->mergeEventIndex(oldindex, newindex);
    }
}

void hkbClipGenerator::fixMergedEventIndices(BehaviorFile *dominantfile){
    if (triggers.data()){
        triggers.data()->fixMergedEventIndices(dominantfile);
    }
}

bool hkbClipGenerator::merge(HkxObject *recessiveObject){
    hkbClipGenerator *obj = nullptr;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_CLIP_GENERATOR){
        obj = static_cast<hkbClipGenerator *>(recessiveObject);
        injectWhileMerging(obj);
        if (triggers.data()){
            if (obj->triggers.data()){
                triggers.data()->merge(obj->triggers.data());
            }
        }else if (obj->triggers.data()){
            triggers = obj->triggers;
            getParentFile()->addObjectToFile(obj->triggers.data(), -1);
            obj->triggers.data()->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        }
        return true;
    }else{
        return false;
    }
}

void hkbClipGenerator::updateReferences(long &ref){
    setReference(ref);
    ref++;
    setBindingReference(ref);
    if (triggers.data()){
        ref++;
        triggers.data()->setReference(ref);
    }
}

QVector<HkxObject *> hkbClipGenerator::getChildrenOtherTypes() const{
    QVector<HkxObject *> list;
    if (triggers.data()){
        list.append(triggers.data());
    }
    return list;
}

SkyrimClipGeneratoData hkbClipGenerator::getClipGeneratorAnimData(ProjectAnimData *parent, uint animationIndex) const{
    QVector <SkyrimClipTrigger> animTrigs;
    qreal trigtime;
    hkbClipTriggerArray *trigs = static_cast<hkbClipTriggerArray *>(triggers.data());
    if (trigs){
        for (int i = 0; i < trigs->triggers.size(); i++){
            if (trigs->triggers.at(i).relativeToEndOfClip){
                trigtime = trigs->triggers.at(i).localTime + static_cast<BehaviorFile *>(getParentFile())->getAnimationDurationFromAnimData(animationName);
            }else{
                trigtime = trigs->triggers.at(i).localTime;
            }
            animTrigs.append(SkyrimClipTrigger(trigtime, static_cast<BehaviorFile *>(getParentFile())->getEventNameAt(trigs->triggers.at(i).event.id)));
        }
    }
    return SkyrimClipGeneratoData(parent, name, animationIndex, playbackSpeed, cropStartAmountLocalTime, cropEndAmountLocalTime, animTrigs);
}

void hkbClipGenerator::setName(const QString &oldclipname, const QString &newclipname){
    //static_cast<BehaviorFile *>(getParentFile())->setClipNameAnimData(oldclipname, newclipname);    //Unsafe...
    name = newclipname;
}

void hkbClipGenerator::setAnimationName(int index, const QString &animationname){
    animationName = animationname;
    //static_cast<BehaviorFile *>(getParentFile())->setAnimationIndexAnimData(index, name);    //Unsafe...
}

void hkbClipGenerator::setPlaybackSpeed(qreal speed){
    playbackSpeed = speed;
    //static_cast<BehaviorFile *>(getParentFile())->setPlaybackSpeedAnimData(name, speed);    //Unsafe...
}

void hkbClipGenerator::setCropStartAmountLocalTime(qreal time){
    cropStartAmountLocalTime = time;
    //static_cast<BehaviorFile *>(getParentFile())->setCropStartAmountLocalTimeAnimData(name, time);    //Unsafe...
}

void hkbClipGenerator::setCropEndAmountLocalTime(qreal time){
    cropEndAmountLocalTime = time;
    //static_cast<BehaviorFile *>(getParentFile())->setCropEndAmountLocalTimeAnimData(name, time);    //Unsafe...
}

bool hkbClipGenerator::link(){
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(triggers.getShdPtrReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
            LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": link()!\n'triggers' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
        }
        triggers = *ptr;
    }
    return true;
}

void hkbClipGenerator::unlink(){
    HkDynamicObject::unlink();
    triggers = HkxSharedPtr();
}

QString hkbClipGenerator::evaluateDataValidity(){
    QString errors;
    bool isvalid = true;
    bool valid = true;
    QString temp = HkDynamicObject::evaluateDataValidity(); if (temp != ""){errors.append(temp+getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid variable binding set!\n");}
    if (name == ""){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid name!\n");
    }
    if (!static_cast<BehaviorFile *>(getParentFile())->getAnimationNames().contains(animationName, Qt::CaseInsensitive)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid animationName!\n");
    }
    if (triggers.data()){
        if (triggers.data()->getSignature() != HKB_CLIP_TRIGGER_ARRAY){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid triggers type! Signature: "+QString::number(triggers.data()->getSignature(), 16)+"\n");
        }else if (triggers.data()->isDataValid() && triggers.data()->evaluateDataValidity() != ""){
            isvalid = false;
            errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid triggers data!\n");
        }
    }
    if (!PlaybackMode.contains(mode)){
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid mode!\n");
    }
    if (flags.toUInt(&valid) >= INVALID_FLAG || !valid){    //TO DO: Fix this...
        isvalid = false;
        errors.append(getParentFile()->getFileName()+": "+getClassname()+": Ref: "+getReferenceString()+": "+getName()+": Invalid flags!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbClipGenerator::~hkbClipGenerator(){
    //static_cast<BehaviorFile *>(getParentFile())->removeClipGenFromAnimData(animationName.section("\\", -1, -1), name);
    refCount--;
}
