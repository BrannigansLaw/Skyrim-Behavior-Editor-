#include "hkbposematchinggenerator.h"
#include "hkbblendergeneratorchild.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

uint hkbPoseMatchingGenerator::refCount = 0;

const QString hkbPoseMatchingGenerator::classname = "hkbPoseMatchingGenerator";

const QStringList hkbPoseMatchingGenerator::Mode = {"MODE_MATCH", "MODE_PLAY"};

hkbPoseMatchingGenerator::hkbPoseMatchingGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
    userData(0),
    referencePoseWeightThreshold(0),
    blendParameter(0),
    minCyclicBlendParameter(0),
    maxCyclicBlendParameter(0),
    indexOfSyncMasterChild(-1),
    flags("0"),
    subtractLastChild(false),
    blendSpeed(0),
    minSpeedToSwitch(0),
    minSwitchTimeNoError(0),
    minSwitchTimeFullError(0),
    startPlayingEventId(-1),
    startMatchingEventId(-1),
    rootBoneIndex(-1),
    otherBoneIndex(-1),
    anotherBoneIndex(-1),
    pelvisIndex(-1),
    mode(Mode.first())
{
    setType(HKB_POSE_MATCHING_GENERATOR, TYPE_GENERATOR);
    parent->addObjectToFile(this, ref);
    refCount++;
    name = "PoseMatchingGenerator_"+QString::number(refCount);
}

const QString hkbPoseMatchingGenerator::getClassname(){
    return classname;
}

QString hkbPoseMatchingGenerator::getName() const{
    std::lock_guard <std::mutex> guard(mutex);
    return name;
}

int hkbPoseMatchingGenerator::getNumberOfChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    return children.size();
}

bool hkbPoseMatchingGenerator::insertObjectAt(int index, DataIconManager *obj){
    std::lock_guard <std::mutex> guard(mutex);
    if (obj){
        if (obj->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            if (index >= children.size() || index == -1){
                children.append(HkxSharedPtr(obj));
            }else if (index == 0 || !children.isEmpty()){
                children.replace(index, HkxSharedPtr(obj));
            }
            return true;
        }
    }
    return false;
}

bool hkbPoseMatchingGenerator::removeObjectAt(int index){
    std::lock_guard <std::mutex> guard(mutex);
    if (index > -1 && index < children.size()){
        static_cast<hkbBlenderGeneratorChild *>(children.at(index).data())->unlink();
        children.removeAt(index);
    }else if (index == -1){
        for (auto i = 0; i < children.size(); i++){
            static_cast<hkbBlenderGeneratorChild *>(children.at(i).data())->unlink();
        }
        children.clear();
    }else{
        return false;
    }
    return true;
}

bool hkbPoseMatchingGenerator::hasChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            return true;
        }
    }
    return false;
}

bool hkbPoseMatchingGenerator::isEventReferenced(int eventindex) const{
    std::lock_guard <std::mutex> guard(mutex);
    if (startPlayingEventId == eventindex || startMatchingEventId == eventindex){
        return true;
    }
    return false;
}

void hkbPoseMatchingGenerator::updateEventIndices(int eventindex){
    std::lock_guard <std::mutex> guard(mutex);
    auto updateindices = [&](int & index){
        (index > eventindex) ? index-- : index;
    };
    updateindices(startPlayingEventId);
    updateindices(startMatchingEventId);
}

int hkbPoseMatchingGenerator::getIndexOfObj(DataIconManager *obj) const{
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < children.size(); i++){
        if (children.at(i).data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

QVector<DataIconManager *> hkbPoseMatchingGenerator::getChildren() const{
    std::lock_guard <std::mutex> guard(mutex);
    QVector<DataIconManager *> list;
    for (auto i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            list.append(static_cast<DataIconManager*>(children.at(i).data()));
        }
    }
    return list;
}

bool hkbPoseMatchingGenerator::merge(HkxObject *recessiveObject){ //TO DO: Make thread safe!!!
    std::lock_guard <std::mutex> guard(mutex);
    hkbPoseMatchingGenerator *recobj;
    hkbBlenderGeneratorChild *domchild;
    hkbBlenderGeneratorChild *recchild;
    if (!getIsMerged() && recessiveObject && recessiveObject->getSignature() == HKB_POSE_MATCHING_GENERATOR){
        recobj = static_cast<hkbPoseMatchingGenerator *>(recessiveObject);
        if (getVariableBindingSetData()){
            getVariableBindingSet()->merge(recobj->getVariableBindingSetData());
        }else if (recobj->getVariableBindingSetData()){
            getVariableBindingSet() = HkxSharedPtr(recobj->getVariableBindingSetData());
            recobj->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
            getParentFile()->addObjectToFile(recobj->getVariableBindingSetData(), -1);
        }
        for (auto i = 0; i < children.size(); i++){
            domchild = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
            for (auto j = 0; j < recobj->children.size(); j++){
                recchild = static_cast<hkbBlenderGeneratorChild *>(recobj->children.at(j).data());
                if (*recchild == *domchild){
                    domchild->merge(recchild);
                    break;
                }
            }
        }
        setIsMerged(true);
    }
    return true;
}

int hkbPoseMatchingGenerator::getIndexOfChild(hkbBlenderGeneratorChild *child) const{
    std::lock_guard <std::mutex> guard(mutex);
    for (auto i = 0; i < children.size(); i++){
        if (children.at(i).data() == child){
            return i;
        }
    }
    return -1;
}

bool hkbPoseMatchingGenerator::readData(const HkxXmlReader &reader, long & index){
    std::lock_guard <std::mutex> guard(mutex);
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
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "referencePoseWeightThreshold");
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "blendParameter");
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "minCyclicBlendParameter");
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "maxCyclicBlendParameter");
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "indexOfSyncMasterChild");
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            checkvalue((flags != ""), "flags");    //TO DO: fix...
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "subtractLastChild");
        }else if (text == "children"){
            checkvalue(readReferences(reader.getElementValueAt(index), children), "children");
        }else if (text == "worldFromModelRotation"){
            worldFromModelRotation = readVector4(reader.getElementValueAt(index), &ok);
            checkvalue(ok, "worldFromModelRotation");
        }else if (text == "blendSpeed"){
            blendSpeed = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "blendSpeed");
        }else if (text == "minSpeedToSwitch"){
            minSpeedToSwitch = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "minSpeedToSwitch");
        }else if (text == "minSwitchTimeNoError"){
            minSwitchTimeNoError = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "minSwitchTimeNoError");
        }else if (text == "minSwitchTimeFullError"){
            minSwitchTimeFullError = reader.getElementValueAt(index).toDouble(&ok);
            checkvalue(ok, "minSwitchTimeFullError");
        }else if (text == "startPlayingEventId"){
            startPlayingEventId = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "startPlayingEventId");
        }else if (text == "startMatchingEventId"){
            startMatchingEventId = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "startMatchingEventId");
        }else if (text == "rootBoneIndex"){
            rootBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "rootBoneIndex");
        }else if (text == "otherBoneIndex"){
            otherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "otherBoneIndex");
        }else if (text == "anotherBoneIndex"){
            anotherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "anotherBoneIndex");
        }else if (text == "pelvisIndex"){
            pelvisIndex = reader.getElementValueAt(index).toInt(&ok);
            checkvalue(ok, "pelvisIndex");
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            checkvalue(Mode.contains(mode), "mode");
        }else{
            //LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nUnknown field '"+text+"' found!\nObject Reference: "+ref);
        }
    }
    index--;
    return true;
}

bool hkbPoseMatchingGenerator::write(HkxXMLWriter *writer){
    std::lock_guard <std::mutex> guard(mutex);
    auto writedatafield = [&](const QString & name, const QString & value, bool allownull){
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList(name), value, allownull);
    };
    auto writechild = [&](const HkxSharedPtr & shdptr, const QString & datafield){
        if (shdptr.data() && !shdptr->write(writer))
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": write()!\nUnable to write '"+datafield+"'!!!\n");
    };
    if (writer && !getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (getVariableBindingSetData()){
            refString = getVariableBindingSet()->getReferenceString();
        }
        writedatafield("variableBindingSet", refString, false);
        writedatafield("userData", QString::number(userData), false);
        writedatafield("name", name, false);
        writedatafield("referencePoseWeightThreshold", QString::number(referencePoseWeightThreshold, char('f'), 6), false);
        writedatafield("blendParameter", QString::number(blendParameter, char('f'), 6), false);
        writedatafield("minCyclicBlendParameter", QString::number(minCyclicBlendParameter, char('f'), 6), false);
        writedatafield("maxCyclicBlendParameter", QString::number(maxCyclicBlendParameter, char('f'), 6), false);
        writedatafield("indexOfSyncMasterChild", QString::number(indexOfSyncMasterChild), false);
        writedatafield("flags", flags, false);
        writedatafield("subtractLastChild", getBoolAsString(subtractLastChild), false);
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"children", QString::number(children.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (auto i = 0, j = 1; i < children.size(); i++, j++){
            refString.append(children.at(i)->getReferenceString());
            if (j % 16 == 0){
                refString.append("\n");
            }else{
                refString.append(" ");
            }
        }
        if (children.size() > 0){
            if (refString.endsWith(" \0")){
                refString.remove(refString.lastIndexOf(" "), 1);
            }
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writedatafield("worldFromModelRotation", worldFromModelRotation.getValueAsString(), false);
        writedatafield("blendSpeed", QString::number(blendSpeed, char('f'), 6), false);
        writedatafield("minSpeedToSwitch", QString::number(minSpeedToSwitch, char('f'), 6), false);
        writedatafield("minSwitchTimeNoError", QString::number(minSwitchTimeNoError, char('f'), 6), false);
        writedatafield("minSwitchTimeFullError", QString::number(minSwitchTimeFullError, char('f'), 6), false);
        writedatafield("startPlayingEventId", QString::number(startPlayingEventId), false);
        writedatafield("startMatchingEventId", QString::number(startMatchingEventId), false);
        writedatafield("rootBoneIndex", QString::number(rootBoneIndex), false);
        writedatafield("otherBoneIndex", QString::number(otherBoneIndex), false);
        writedatafield("anotherBoneIndex", QString::number(anotherBoneIndex), false);
        writedatafield("pelvisIndex", QString::number(pelvisIndex), false);
        writedatafield("mode", mode, false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        writechild(getVariableBindingSet(), "variableBindingSet");
        for (auto i = 0; i < children.size(); i++){
            writechild(children.at(i), "children.at("+QString::number(i)+")");
        }
    }
    return true;
}

bool hkbPoseMatchingGenerator::link(){
    std::lock_guard <std::mutex> guard(mutex);
    auto baddata = [&](const QString & fieldname){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\n'"+fieldname+"' is linked to invalid data!");
        setDataValidity(false);
    };
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        LogFile::writeToLog(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxSharedPtr *ptr;
    for (auto i = 0; i < children.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGenerator(children.at(i).getShdPtrReference());
        if (!ptr || !ptr->data()){
            baddata("children.at("+QString::number(i)+")");
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            baddata("children.at("+QString::number(i)+")");
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
            static_cast<hkbBlenderGeneratorChild *>(children[i].data())->setParentBG(this);
        }
    }
    return true;
}

void hkbPoseMatchingGenerator::unlink(){
    std::lock_guard <std::mutex> guard(mutex);
    HkDynamicObject::unlink();
    for (auto i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            children[i]->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        children[i] = HkxSharedPtr();
    }
}

QString hkbPoseMatchingGenerator::evaluateDataValidity(){
    std::lock_guard <std::mutex> guard(mutex);
    QString errors;
    auto isvalid = true;
    auto valid = true;
    if (children.isEmpty()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": children is empty!\n");
    }else{
        for (auto i = children.size() - 1; i >= 0; i--){
            if (!children.at(i).data()){
                isvalid = false;
                errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": children at index '"+QString::number(i)+"' is null! Removing child!\n");
                children.removeAt(i);
            }else if (children.at(i)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
                isvalid = false;
                errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid child! Signature: "+QString::number(children.at(i)->getSignature(), 16)+" Removing child!\n");
                children.removeAt(i);
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
    if (flags.toUInt(&valid) >= INVALID_FLAG || !valid){    //TO DO: fix
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid flags!\n");
    }
    if (indexOfSyncMasterChild >= children.size()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": indexOfSyncMasterChild is out of range! Setting default value!\n");
        indexOfSyncMasterChild = -1;
    }
    if (!Mode.contains(mode)){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": Invalid mode! Setting default value!\n");
        mode = Mode.first();
    }
    if (startPlayingEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": startPlayingEventId event id out of range! Setting to last event index!\n");
        startPlayingEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (startMatchingEventId >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": startMatchingEventId event id out of range! Setting to last event index!\n");
        startMatchingEventId = static_cast<BehaviorFile *>(getParentFile())->getNumberOfEvents() - 1;
    }
    if (rootBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": rootBoneIndex out of range! Setting to last bone index!\n");
        rootBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    if (otherBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": otherBoneIndex out of range! Setting to last bone index!\n");
        otherBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    if (anotherBoneIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": anotherBoneIndex out of range! Setting to last bone index!\n");
        anotherBoneIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    if (pelvisIndex >= static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones()){
        isvalid = false;
        errors.append(getParentFilename()+": "+getClassname()+": Ref: "+getReferenceString()+": "+name+": pelvisIndex out of range! Setting to last bone index!\n");
        pelvisIndex = static_cast<BehaviorFile *>(getParentFile())->getNumberOfBones() - 1;
    }
    setDataValidity(isvalid);
    return errors;
}

hkbPoseMatchingGenerator::~hkbPoseMatchingGenerator(){
    refCount--;
}
