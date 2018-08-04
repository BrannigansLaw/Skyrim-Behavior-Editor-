#include "hkbfootikcontrolsmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"

uint hkbFootIkControlsModifier::refCount = 0;

const QString hkbFootIkControlsModifier::classname = "hkbFootIkControlsModifier";

hkbFootIkControlsModifier::hkbFootIkControlsModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true)
{
    setType(HKB_FOOT_IK_CONTROLS_MODIFIER, TYPE_MODIFIER);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "FootIkControlsModifier_"+QString::number(refCount);
}

const QString hkbFootIkControlsModifier::getClassname(){
    return classname;
}

QString hkbFootIkControlsModifier::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

bool hkbFootIkControlsModifier::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
    int numlegs;
    bool ok;
    QByteArray text;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    auto checkvalue = [&](bool value, const QString & fieldname){
        (!value) ? LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\n'"+fieldname+"' has invalid data!\nObject Reference: "+ref) : NULL;
    };
    for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            checkvalue(getVariableBindingSet().readShdPtrReference(index, reader), "variableBindingSet");
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            checkvalue(ok, "userData");
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            checkvalue((name != ""), "name");
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "enable");
        }else if (text == "onOffGain"){
            gains.onOffGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "enable");
        }else if (text == "groundAscendingGain"){
            gains.groundAscendingGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "groundAscendingGain");
        }else if (text == "groundDescendingGain"){
            gains.groundDescendingGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "groundDescendingGain");
        }else if (text == "footPlantedGain"){
            gains.footPlantedGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "footPlantedGain");
        }else if (text == "footRaisedGain"){
            gains.footRaisedGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "footRaisedGain");
        }else if (text == "footUnlockGain"){
            gains.footUnlockGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "footUnlockGain");
        }else if (text == "worldFromModelFeedbackGain"){
            gains.worldFromModelFeedbackGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "worldFromModelFeedbackGain");
        }else if (text == "errorUpDownBias"){
            gains.errorUpDownBias = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "errorUpDownBias");
        }else if (text == "alignWorldFromModelGain"){
            gains.alignWorldFromModelGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "alignWorldFromModelGain");
        }else if (text == "hipOrientationGain"){
            gains.hipOrientationGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "hipOrientationGain");
        }else if (text == "maxKneeAngleDifference"){
            gains.maxKneeAngleDifference = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "maxKneeAngleDifference");
        }else if (text == "ankleOrientationGain"){
            gains.ankleOrientationGain = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "ankleOrientationGain");
        }else if (text == "legs"){
            numlegs = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            checkvalue(ok, "legs");
            (numlegs > 0) ? index++ : NULL;
            for (auto j = 0; j < numlegs; j++, index++){
                legs.append(hkLeg());
                for (; index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"; index++){
                    text = reader.getNthAttributeValueAt(index, 0);
                    if (text == "groundPosition"){
                        legs.last().groundPosition = readVector4(reader.getElementValueAt(index), &ok);
                        checkvalue(ok, "legs.at("+QString::number(j)+").groundPosition");
                    }else if (text == "id"){
                        legs.last().id = reader.getElementValueAt(index).toDouble(&ok);
                        checkvalue(ok, "legs.at("+QString::number(j)+").id");
                    }else if (text == "payload"){
                        checkvalue(legs.last().payload.readShdPtrReference(index, reader), "legs.at("+QString::number(j)+").payload");
                    }else if (text == "verticalError"){
                        legs.last().verticalError = reader.getElementValueAt(index).toDouble(&ok);
                        checkvalue(ok, "legs.at("+QString::number(j)+").verticalError");
                    }else if (text == "hitSomething"){
                        legs.last().hitSomething = toBool(reader.getElementValueAt(index), &ok);
                        checkvalue(ok, "legs.at("+QString::number(j)+").hitSomething");
                    }else if (text == "isPlantedMS"){
                        legs.last().isPlantedMS = toBool(reader.getElementValueAt(index), &ok);
                        checkvalue(ok, "legs.at("+QString::number(j)+").isPlantedMS");
                        break;
                    }else{
                        //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
                    }
                }
            }
            (numlegs > 0) ? index-- : NULL;
        }else if (text == "errorOutTranslation"){
            errorOutTranslation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "errorOutTranslation");
        }else if (text == "alignWithGroundRotation"){
            alignWithGroundRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "alignWithGroundRotation");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbFootIkControlsModifier::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value);
    };
    auto writeref = [&](const HkxSharedPtr & shdptr, const QString & name){
        QString refString = "null";
        (shdptr.data()) ? refString = shdptr->getReferenceString() : NULL;
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), refString);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!\n");
    };
    if (writer && !getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writeref(getVariableBindingSet(), "variableBindingSet");
        writedatafield("userData", QString::number(userData));
        writedatafield("name", name);
        writedatafield("enable", getBoolAsString(enable));
        writedatafield("controlData", "");
        writer->writeLine(writer->object, true);
        writedatafield("gains", "");
        writer->writeLine(writer->object, true);
        writedatafield("onOffGain", QString::number(gains.onOffGain, char('f'), 6));
        writedatafield("groundAscendingGain", QString::number(gains.groundAscendingGain, char('f'), 6));
        writedatafield("groundDescendingGain", QString::number(gains.groundDescendingGain, char('f'), 6));
        writedatafield("footPlantedGain", QString::number(gains.footPlantedGain, char('f'), 6));
        writedatafield("footRaisedGain", QString::number(gains.footRaisedGain, char('f'), 6));
        writedatafield("footUnlockGain", QString::number(gains.footUnlockGain, char('f'), 6));
        writedatafield("worldFromModelFeedbackGain", QString::number(gains.worldFromModelFeedbackGain, char('f'), 6));
        writedatafield("errorUpDownBias", QString::number(gains.errorUpDownBias, char('f'), 6));
        writedatafield("alignWorldFromModelGain", QString::number(gains.alignWorldFromModelGain, char('f'), 6));
        writedatafield("hipOrientationGain", QString::number(gains.hipOrientationGain, char('f'), 6));
        writedatafield("maxKneeAngleDifference", QString::number(gains.maxKneeAngleDifference, char('f'), 6));
        writedatafield("ankleOrientationGain", QString::number(gains.ankleOrientationGain, char('f'), 6));
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        list1 = {writer->name, writer->numelements};
        list2 = {"legs", QString::number(legs.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (auto i = 0; i < legs.size(); i++){
            writer->writeLine(writer->object, true);
            writedatafield("fwdAxisLS", legs[i].groundPosition.getValueAsString());
            writedatafield("id", QString::number(legs.at(i).id));
            writeref(legs.at(i).payload, "legs.at("+QString::number(i)+").payload");
            writedatafield("verticalError", QString::number(legs.at(i).verticalError, char('f'), 6));
            writedatafield("hitSomething", getBoolAsString(legs.at(i).hitSomething));
            writedatafield("isPlantedMS", getBoolAsString(legs.at(i).isPlantedMS));
            writer->writeLine(writer->object, false);
        }
        if (legs.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writedatafield("errorOutTranslation", errorOutTranslation.getValueAsString());
        writedatafield("alignWithGroundRotation", alignWithGroundRotation.getValueAsString());
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        for (auto i = 0; i < legs.size(); i++){
            writechild(legs.at(i).payload, "legs.at("+QString::number(i)+").payload");
        }
    }
    return true;
}

int hkbFootIkControlsModifier::getNumberOfLegs() const{
    std::lock_guard <std::mutex> guard(mutex);
    return legs.size();
}

bool hkbFootIkControlsModifier::isEventReferenced(int eventindex) const{
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < legs.size(); i++){
        if (legs.at(i).id == eventindex){
            return true;
        }
    }
    return false;
}

void hkbFootIkControlsModifier::updateEventIndices(int eventindex){
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < legs.size(); i++){
        (legs.at(i).id > eventindex) ? legs[i].id-- : NULL;
    }
}

void hkbFootIkControlsModifier::mergeEventIndex(int oldindex, int newindex){
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < legs.size(); i++){
        (legs.at(i).id == oldindex) ? legs[i].id = newindex : NULL;
    }
}

void hkbFootIkControlsModifier::fixMergedEventIndices(BehaviorFile *dominantfile){
    std::lock_guard <std::mutex> guard(mutex);
    hkbBehaviorGraphData *recdata;
    hkbBehaviorGraphData *domdata;
    QString thiseventname;
    int eventindex;
    if (!getIsMerged() && dominantfile){
        //TO DO: Support character properties...
        recdata = static_cast<hkbBehaviorGraphData *>(static_cast<BehaviorFile *>(getParentFile())->getBehaviorGraphData());
        domdata = static_cast<hkbBehaviorGraphData *>(dominantfile->getBehaviorGraphData());
        if (recdata && domdata){
            for (auto i = 0; i < legs.size(); i++){
                thiseventname = recdata->getEventNameAt(legs.at(i).id);
                eventindex = domdata->getIndexOfEvent(thiseventname);
                if (eventindex == -1 && thiseventname != ""){
                    domdata->addEvent(thiseventname);
                    eventindex = domdata->getNumberOfEvents() - 1;
                }
                legs[i].id = eventindex;
            }
            setIsMerged(true);
        }
    }
}


void hkbFootIkControlsModifier::updateReferences(long &ref){
    std::lock_guard <std::mutex> guard(mutex);
    setReference(ref);
    setBindingReference(++ref);
    for (auto i = 0; i < legs.size(); i++){
        (legs.at(i).payload.data()) ? legs[i].payload->updateReferences(++ref) : NULL;
    }
}

QVector<HkxObject *> hkbFootIkControlsModifier::getChildrenOtherTypes() const{
    std::lock_guard <std::mutex> guard(mutex);
    QVector<HkxObject *> list;
    for (auto i = 0; i < legs.size(); i++){
        (legs.at(i).payload.data()) ? list.append(legs.at(i).payload.data()) : NULL;
    }
    return list;
}

bool hkbFootIkControlsModifier::merge(HkxObject *recessiveObject){ //TO DO: Make thread safe!!!
    std::lock_guard <std::mutex> guard(mutex);
    hkbFootIkControlsModifier *recobj;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_FOOT_IK_CONTROLS_MODIFIER){
        recobj = static_cast<hkbFootIkControlsModifier *>(recessiveObject);
        injectWhileMerging(recobj);
        for (auto i = 0; i < legs.size(); i++){
            if (legs.at(i).payload.data()){
                if (!legs.at(i).payload.data() && recobj->legs.at(i).payload.data()){
                    getParentFile()->addObjectToFile(recobj->legs.at(i).payload.data(), -1);
                }
            }
        }
        return true;
    }
    return false;
}

bool hkbFootIkControlsModifier::link(){
    std::lock_guard <std::mutex> guard(mutex);
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (auto i = 0; i < legs.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(legs.at(i).payload.getShdPtrReference());
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
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
    for (auto i = 0; i < legs.size(); i++){
        legs[i].payload = HkxSharedPtr();
    }
}

QString hkbFootIkControlsModifier::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    bool isvalid = true;
    if (legs.isEmpty()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": legs is empty!\n");
    }else{
        for (auto i = 0; i < legs.size(); i++){
            if (legs.at(i).id >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
                isvalid = false;
                errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": id in legs at "+QString::number(i)+" out of range! Setting to max index in range!\n");
                legs[i].id = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
            }
            if (legs.at(i).payload.data() && legs.at(i).payload->getSignature() != HKB_STRING_EVENT_PAYLOAD){
                isvalid = false;
                errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid payload type! Signature: "+QString::number(legs.at(i).payload->getSignature(), 16)+" Setting null value!\n");
                legs[i].payload = HkxSharedPtr();
            }
        }
    }
    QString temp = HkDynamicObject::evaluateDataValidity();
    if (temp != ""){
        errors.append(temp+getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid variable binding set!\n");
    }
    if (name == ""){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid name!\n");
    }
    setDataValidity(isvalid);
    return errors;
}

hkbFootIkControlsModifier::~hkbFootIkControlsModifier(){
    refCount--;
}
