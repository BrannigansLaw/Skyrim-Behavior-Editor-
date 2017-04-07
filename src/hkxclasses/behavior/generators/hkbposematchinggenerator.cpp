#include "hkbposematchinggenerator.h"
#include "hkbblendergeneratorchild.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbPoseMatchingGenerator
*/

uint hkbPoseMatchingGenerator::refCount = 0;

QString hkbPoseMatchingGenerator::classname = "hkbPoseMatchingGenerator";

QStringList hkbPoseMatchingGenerator::Mode = {"MODE_MATCH", "MODE_PLAY"};
//QStringList hkbPoseMatchingGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};
QStringList hkbPoseMatchingGenerator::Flags = {"0", "1", "2", "8", "16", "32", "64"};

hkbPoseMatchingGenerator::hkbPoseMatchingGenerator(HkxFile *parent, long ref)
    : hkbGenerator(parent, ref),
    userData(0),
    referencePoseWeightThreshold(0),
    blendParameter(0),
    minCyclicBlendParameter(0),
    maxCyclicBlendParameter(0),
    indexOfSyncMasterChild(-1),
    flags(Flags.first()),
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
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbPoseMatchingGenerator"+QString::number(refCount);
}

QString hkbPoseMatchingGenerator::getClassname(){
    return classname;
}

QString hkbPoseMatchingGenerator::getName() const{
    return name;
}

int hkbPoseMatchingGenerator::getNumberOfChildren() const{
    return children.size();
}

int hkbPoseMatchingGenerator::getIndexToInsertIcon(HkxObject *child) const{
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).constData() == child){
            return i;
        }
    }
    return -1;
}

bool hkbPoseMatchingGenerator::insertObjectAt(int index, DataIconManager *obj){
    hkbBlenderGeneratorChild *objChild;
    if (((HkxObject *)obj)->getType() == TYPE_GENERATOR){
        if (index >= children.size()){
            objChild = new hkbBlenderGeneratorChild(getParentFile(), this, -1);
            children.append(HkxObjectExpSharedPtr(objChild));
            objChild->generator = HkxObjectExpSharedPtr((HkxObject *)obj);
        }else if (index > -1){
            objChild = static_cast<hkbBlenderGeneratorChild *>(children.at(index - 1).data());
            objChild->generator = HkxObjectExpSharedPtr((HkxObject *)obj);
        }else{
            return false;
        }
        return true;
    }else{
        return false;
    }
}

bool hkbPoseMatchingGenerator::removeObjectAt(int index){
    hkbBlenderGeneratorChild *objChild;
    if (index > -1 && index < children.size()){
        objChild = static_cast<hkbBlenderGeneratorChild *>(children.at(index).data());
        children.removeAt(index);
    }else{
        return false;
    }
    return true;
}

bool hkbPoseMatchingGenerator::hasChildren() const{
    hkbBlenderGeneratorChild *child;
    for (int i = 0; i < children.size(); i++){
        child = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
        if (child->generator.data()){
            return true;
        }
    }
    return false;
}

int hkbPoseMatchingGenerator::getIndexOfObj(DataIconManager *obj) const{
    hkbBlenderGeneratorChild *child;
    for (int i = 0; i < children.size(); i++){
        child = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
        if (child->generator.data() == (HkxObject *)obj){
            return i;
        }
    }
    return -1;
}

QList<DataIconManager *> hkbPoseMatchingGenerator::getChildren() const{
    QList<DataIconManager *> list;
    hkbBlenderGeneratorChild *child;
    for (int i = 0; i < children.size(); i++){
        child = static_cast<hkbBlenderGeneratorChild *>(children.at(i).data());
        if (child->generator.data()){
            list.append((DataIconManager *)child->generator.data());
        }
    }
    return list;
}

bool hkbPoseMatchingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
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
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'referencePoseWeightThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'blendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'minCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'maxCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'indexOfSyncMasterChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'subtractLastChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'children' references!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelRotation"){
            worldFromModelRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'worldFromModelRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendSpeed"){
            blendSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'blendSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSpeedToSwitch"){
            minSpeedToSwitch = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'minSpeedToSwitch' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeNoError"){
            minSwitchTimeNoError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'minSwitchTimeNoError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeFullError"){
            minSwitchTimeFullError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'minSwitchTimeFullError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startPlayingEventId"){
            startPlayingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'startPlayingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startMatchingEventId"){
            startMatchingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'startMatchingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootBoneIndex"){
            rootBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'rootBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "otherBoneIndex"){
            otherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'otherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "anotherBoneIndex"){
            anotherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'anotherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pelvisIndex"){
            pelvisIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'pelvisIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbPoseMatchingGenerator::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("referencePoseWeightThreshold"), QString::number(referencePoseWeightThreshold));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendParameter"), QString::number(blendParameter));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minCyclicBlendParameter"), QString::number(minCyclicBlendParameter));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("maxCyclicBlendParameter"), QString::number(maxCyclicBlendParameter));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("indexOfSyncMasterChild"), QString::number(indexOfSyncMasterChild));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("flags"), flags);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("subtractLastChild"), getBoolAsString(subtractLastChild));
        refString = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"children", QString::number(children.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < children.size(); i++){
            refString = refString+" "+children.at(i).data()->getReferenceString();
            if (i > 0 && i % 16 == 0){
                refString = refString+"\n";
            }
        }
        if (children.size() > 0){
            writer->writeLine(refString);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldFromModelRotation"), worldFromModelRotation.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("blendSpeed"), QString::number(blendSpeed));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minSpeedToSwitch"), QString::number(minSpeedToSwitch));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minSwitchTimeNoError"), QString::number(minSwitchTimeNoError));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("minSwitchTimeFullError"), QString::number(minSwitchTimeFullError));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startPlayingEventId"), QString::number(startPlayingEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("startMatchingEventId"), QString::number(startMatchingEventId));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rootBoneIndex"), QString::number(rootBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("otherBoneIndex"), QString::number(otherBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("anotherBoneIndex"), QString::number(anotherBoneIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("pelvisIndex"), QString::number(pelvisIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("mode"), mode);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        for (int i = 0; i < children.size(); i++){
            if (children.at(i).data() && !children.at(i).data()->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'children' at: "+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkbPoseMatchingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<HkDynamicObject *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        ptr = static_cast<BehaviorFile *>(getParentFile())->findGeneratorChild(children.at(i).getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'children' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            writeToLog(getClassname()+": link()!\n'children' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
            static_cast<hkbBlenderGeneratorChild *>(children[i].data())->parentBG = this;
        }
    }
    return true;
}

void hkbPoseMatchingGenerator::unlink(){
    HkDynamicObject::unlink();
    for (int i = 0; i < children.size(); i++){
        if (children.at(i).data()){
            children[i].data()->unlink(); //Do here since this is not stored in the hkx file for long...
        }
        children[i] = HkxObjectExpSharedPtr();
    }
}

bool hkbPoseMatchingGenerator::evaulateDataValidity(){
    bool valid = true;
    for (int i = 0; i < children.size(); i++){
        if (!children.at(i).data() || children.at(i).data()->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            valid = false;
        }
    }
    QStringList list = flags.split('|');
    for (int i = 0; i < list.size(); i++){
        if (!Flags.contains(list.at(i))){
            valid = false;
        }
    }
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (!Flags.contains(flags)){
    }else if (!Mode.contains(mode)){
    }else if (name == ""){
    }else if (children.isEmpty()){
    }else if (valid){
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbPoseMatchingGenerator::~hkbPoseMatchingGenerator(){
    refCount--;
}
