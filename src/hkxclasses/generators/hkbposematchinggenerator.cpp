#include "hkbposematchinggenerator.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbPoseMatchingGenerator
*/

uint hkbPoseMatchingGenerator::refCount = 0;

QString hkbPoseMatchingGenerator::classname = "hkbPoseMatchingGenerator";

QStringList hkbPoseMatchingGenerator::Mode = {"MODE_MATCH", "MODE_PLAY"};
//QStringList hkbPoseMatchingGenerator::Flags = {"0", "FLAG_SYNC", "FLAG_SMOOTH_GENERATOR_WEIGHTS", "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS", "FLAG_PARAMETRIC_BLEND", "FLAG_IS_PARAMETRIC_BLEND_CYCLIC", "FLAG_FORCE_DENSE_POSE"};
QStringList hkbPoseMatchingGenerator::Flags = {"0", "1", "2", "8", "16", "32", "64"};

hkbPoseMatchingGenerator::hkbPoseMatchingGenerator(BehaviorFile *parent/*, qint16 ref*/)
    : hkbGenerator(parent/*, ref*/),
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
    pelvisIndex(-1)
{
    setType(HKB_POSE_MATCHING_GENERATOR, TYPE_GENERATOR);
    refCount++;
    name = "Pose Matching Generator "+QString::number(refCount);
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

bool hkbPoseMatchingGenerator::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "referencePoseWeightThreshold"){
            referencePoseWeightThreshold = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'referencePoseWeightThreshold' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendParameter"){
            blendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'blendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minCyclicBlendParameter"){
            minCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "maxCyclicBlendParameter"){
            maxCyclicBlendParameter = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'maxCyclicBlendParameter' data field!\nObject Reference: "+ref);
            }
        }else if (text == "indexOfSyncMasterChild"){
            indexOfSyncMasterChild = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'indexOfSyncMasterChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "flags"){
            flags = reader.getElementValueAt(index);
            if (flags == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'flags' data field!\nObject Reference: "+ref);
            }
        }else if (text == "subtractLastChild"){
            subtractLastChild = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'subtractLastChild' data field!\nObject Reference: "+ref);
            }
        }else if (text == "children"){
            if (!readReferences(reader.getElementValueAt(index), children)){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'children' references!\nObject Reference: "+ref);
            }
        }else if (text == "worldFromModelRotation"){
            worldFromModelRotation = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'worldFromModelRotation' data field!\nObject Reference: "+ref);
            }
        }else if (text == "blendSpeed"){
            blendSpeed = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'blendSpeed' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSpeedToSwitch"){
            minSpeedToSwitch = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSpeedToSwitch' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeNoError"){
            minSwitchTimeNoError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSwitchTimeNoError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "minSwitchTimeFullError"){
            minSwitchTimeFullError = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'minSwitchTimeFullError' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startPlayingEventId"){
            startPlayingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'startPlayingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "startMatchingEventId"){
            startMatchingEventId = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'startMatchingEventId' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rootBoneIndex"){
            rootBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'rootBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "otherBoneIndex"){
            otherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'otherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "anotherBoneIndex"){
            anotherBoneIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'anotherBoneIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "pelvisIndex"){
            pelvisIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'pelvisIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "mode"){
            mode = reader.getElementValueAt(index);
            if (mode == ""){
                writeToLog("hkbPoseMatchingGenerator: readData()!\nFailed to properly read 'mode' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbPoseMatchingGenerator::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!static_cast<hkbGenerator *>(this)->linkVar()){
        writeToLog("hkbPoseMatchingGenerator: link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    //children
    HkxObjectExpSharedPtr *ptr;
    for (int i = 0; i < children.size(); i++){
        //generators
        ptr = getParentFile()->findGenerator(children.at(i).getReference());
        if (!ptr){
            writeToLog("hkbPoseMatchingGenerator: link()!\nFailed to properly link 'children' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if ((*ptr)->getSignature() != HKB_BLENDER_GENERATOR_CHILD){
            writeToLog("hkbPoseMatchingGenerator: link()!\n'children' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            children[i] = *ptr;
        }else{
            children[i] = *ptr;
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