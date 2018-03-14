#include "characterfile.h"
#include "skeletonfile.h"
#include "projectfile.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/behavior/hkbcharacterstringdata.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/hkxclasses/behavior/hkbfootikdriverinfo.h"
#include "src/hkxclasses/behavior/hkbhandikdriverinfo.h"
#include "src/hkxclasses/behavior/hkbvariablevalueset.h"
#include "src/hkxclasses/behavior/hkbmirroredskeletoninfo.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

CharacterFile::CharacterFile(MainWindow *window, ProjectFile *projectfile, const QString & name, bool autogeneratedata, const QString &skeletonrelativepath)
    : HkxFile(window, name),
      project(projectfile),
      skeleton(nullptr),
      largestRef(0)
{
    getReader().setFile(this);
    if (autogeneratedata && skeletonrelativepath != ""){
        hkRootLevelContainer *root = new hkRootLevelContainer(this);
        hkbCharacterStringData *stringdata = new hkbCharacterStringData(this, 0);
        hkbCharacterData *characterdata = new hkbCharacterData(this, 0);
        hkbMirroredSkeletonInfo *info = new hkbMirroredSkeletonInfo(this);
        hkbVariableValueSet *values = new hkbVariableValueSet(this);
        stringdata->name = name.section("/", -1, -1).remove(".hkx");
        stringdata->rigName = skeletonrelativepath;
        stringdata->ragdollName = skeletonrelativepath;
        stringdata->behaviorFilename = "Behaviors\\Master.hkx";
        characterdata->stringData = HkxSharedPtr(stringdata);
        characterdata->mirroredSkeletonInfo = HkxSharedPtr(info);
        characterdata->characterPropertyValues = HkxSharedPtr(values);
        root->addVariant("hkbCharacterData");
        root->setVariantAt(0, characterdata);
        setRootObject(HkxSharedPtr(root));
        setIsChanged(true);
    }
}

HkxSharedPtr * CharacterFile::findCharacterData(long ref){
    if (characterData->getReference() == ref){
        return &characterData;
    }
    return nullptr;
}

HkxSharedPtr * CharacterFile::findCharacterPropertyValues(long ref){
    for (int i = 0; i < boneWeightArrays.size(); i++){
        if (boneWeightArrays.at(i).data() && boneWeightArrays.at(i).getReference() == ref){
            return &boneWeightArrays[i];
        }
    }
    return nullptr;
}

/*QString CharacterFile::getRelativeRootBehaviorPath() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return ptr->behaviorFilename.section("\\", -3, -2);
    }
    return "";
}*/

QString CharacterFile::getBehaviorDirectoryName() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return QString(ptr->behaviorFilename).replace("\\", "/").section("/", -2, -2);
    }
    return "";
}

QString CharacterFile::getRigName() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return QString(ptr->rigName).replace("\\", "/").section("/", -2, -1);
    }
    return "";
}

hkbCharacterData * CharacterFile::getCharacterData() const{
    if (characterData.data() && characterData->getSignature() == HKB_CHARACTER_DATA){
        return static_cast<hkbCharacterData *>(characterData.data());
    }
    return nullptr;
}

QStringList CharacterFile::getRigBoneNames() const{
    if (skeleton){
        return skeleton->getBonesFromSkeletonAt(0);
    }
    return QStringList();
}

int CharacterFile::getNumberOfBones(bool ragdoll) const{
    if (skeleton){
        return skeleton->getNumberOfBones(ragdoll);
    }
    return -1;
}

QString CharacterFile::getRagdollName() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return ptr->ragdollName;
    }
    return "";
}

QStringList CharacterFile::getRagdollBoneNames() const{
    if (skeleton){
        return skeleton->getBonesFromSkeletonAt(1);
    }
    return QStringList();
}

QStringList CharacterFile::getAnimationNames() const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        return static_cast<hkbCharacterStringData *>(stringData.data())->getAnimationNames();
    }
    return QStringList();
}

QString CharacterFile::getAnimationNameAt(int index) const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        return static_cast<hkbCharacterStringData *>(stringData.data())->getAnimationNameAt(index);
    }
    return QString();
}

QString CharacterFile::getCharacterPropertyNameAt(int index) const{
    return static_cast<hkbCharacterStringData *>(stringData.data())->getCharacterPropertyNameAt(index);
}

int CharacterFile::getCharacterPropertyIndex(const QString &name) const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        return static_cast<hkbCharacterStringData *>(stringData.data())->characterPropertyNames.indexOf(name);
    }
    return -1;
}

QStringList CharacterFile::getCharacterPropertyNames() const{
    return static_cast<hkbCharacterData *>(characterData.data())->getCharacterPropertyNames();
}

QStringList CharacterFile::getCharacterPropertyTypenames() const{
    return static_cast<hkbCharacterData *>(characterData.data())->getCharacterPropertyTypenames();
}

hkVariableType CharacterFile::getCharacterPropertyTypeAt(int index) const{
    return static_cast<hkbCharacterData *>(characterData.data())->getCharacterPropertyTypeAt(index);
}

int CharacterFile::getAnimationIndex(const QString &name) const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        return static_cast<hkbCharacterStringData *>(stringData.data())->getAnimationIndex(name);
    }
    return -1;
}

int CharacterFile::getNumberOfAnimations() const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        return static_cast<hkbCharacterStringData *>(stringData.data())->animationNames.size();
    }
    return -1;
}

bool CharacterFile::isAnimationUsed(const QString &animationname) const{
    return project->isAnimationUsed(animationname);
}

QString CharacterFile::getRootBehaviorFilename() const{
    if (stringData.data() && stringData->getSignature() == HKB_CHARACTER_STRING_DATA){
        QString string = static_cast<hkbCharacterStringData *>(stringData.data())->behaviorFilename;
        if (string.contains("\\")){
            return string.section("\\", -1, -1);
        }else if (string.contains("/")){
            return string.section("/", -1, -1);
        }
        return string;
    }
    return "";
}

void CharacterFile::setSkeletonFile(SkeletonFile *skel){
    skeleton = skel;
}

bool CharacterFile::addObjectToFile(HkxObject *obj, long ref){
    if (ref > largestRef){
        largestRef = ref;
    }else{
        largestRef++;
    }
    obj->setReference(largestRef);
    if (obj->getSignature() == HKB_BONE_WEIGHT_ARRAY){
        boneWeightArrays.append(HkxSharedPtr(obj, ref));
    }else if (obj->getSignature() == HKB_CHARACTER_DATA){
        characterData = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_CHARACTER_STRING_DATA){
        stringData = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_MIRRORED_SKELETON_INFO){
        mirroredSkeletonInfo = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_VARIABLE_VALUE_SET){
        characterPropertyValues = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_HAND_IK_DRIVER_INFO){
        handIkDriverInfo = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_FOOT_IK_DRIVER_INFO){
        footIkDriverInfo = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_CHARACTER_DATA){
        characterData = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HK_ROOT_LEVEL_CONTAINER){
        setRootObject(HkxSharedPtr(obj, ref));
    }else{
        writeToLog("CharacterFile: addObjectToFile() failed!\nInvalid type enum for this object!\nObject signature is: "+QString::number(obj->getSignature(), 16), true);
        return false;
    }
    return true;
}

bool CharacterFile::parse(){
    if (!getReader().parse()){
        return false;
    }
    int index = 2;
    bool ok = true;
    HkxSignature signature;
    QByteArray value;
    long ref = 0;
    //setProgressData("Creating HKX objects...", 60);
    while (index < getReader().getNumElements()){
        value = getReader().getNthAttributeNameAt(index, 1);
        if (value == "class"){
            value = getReader().getNthAttributeValueAt(index, 2);
            if (value != ""){
                ref = getReader().getNthAttributeValueAt(index, 0).remove(0, 1).toLong(&ok);
                if (!ok){
                    writeToLog("CharacterFile: parse() failed!\nThe object reference string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                signature = (HkxSignature)value.toULongLong(&ok, 16);
                if (!ok){
                    writeToLog("CharacterFile: parse() failed!\nThe object signature string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                if (signature == HKB_BONE_WEIGHT_ARRAY){
                    if (!appendAndReadData(index, new hkbBoneWeightArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_FOOT_IK_DRIVER_INFO){
                    if (!appendAndReadData(index, new hkbFootIkDriverInfo(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_HAND_IK_DRIVER_INFO){
                    if (!appendAndReadData(index, new hkbHandIkDriverInfo(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MIRRORED_SKELETON_INFO){
                    if (!appendAndReadData(index, new hkbMirroredSkeletonInfo(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_VARIABLE_VALUE_SET){
                    if (!appendAndReadData(index, new hkbVariableValueSet(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_CHARACTER_DATA){
                    if (!appendAndReadData(index, new hkbCharacterData(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_CHARACTER_STRING_DATA){
                    if (!appendAndReadData(index, new hkbCharacterStringData(this, ref))){
                        return false;
                    }
                }else if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this, ref))){
                        return false;
                    }
                }else{
                    writeToLog("CharacterFile: parse()!\nUnknown signature detected!\nUnknown object class name is: "+getReader().getNthAttributeValueAt(index, 1)+"\nUnknown object signature is: "+QString::number(signature, 16));
                }
            }
        }
        index++;
    }
    closeFile();
    getReader().clear();
    //setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("CharacterFile: parse() failed because link() failed!", true);
        return false;
    }
    return true;
}

bool CharacterFile::link(){
    if (!getRootObject().constData()){
        writeToLog("CharacterFile: link() failed!\nThe root object of this character file is nullptr!", true);
        return false;
    }else if (getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        writeToLog("CharacterFile: link() failed!\nThe root object of this character file is NOT a hkRootLevelContainer!\nThe root object signature is: "+QString::number(getRootObject()->getSignature(), 16), true);
        return false;
    }
    if (!getRootObject().data()->link()){
        writeToLog("CharacterFile: link() failed!\nThe root object of this character file failed to link to it's children!", true);
        return false;
    }
    if (!characterData.data() || !characterData.data()->link()){
        writeToLog("CharacterFile: link() failed!\ncharacterData failed to link to it's children!\n", true);
        return false;
    }
    if (!characterPropertyValues.data() || !characterPropertyValues.data()->link()){
        writeToLog("CharacterFile: link() failed!\ncharacterPropertyValues failed to link to it's children!\n", true);
        return false;
    }
    return true;
}

hkbBoneWeightArray *CharacterFile::addNewBoneWeightArray(){
    hkbBoneWeightArray *ptr = new hkbBoneWeightArray(this, 0, skeleton->getNumberOfBones());
    boneWeightArrays.append(HkxSharedPtr(ptr));
    return ptr;
}

void CharacterFile::write(){
    if (getRootObject().data()){
        ulong ref = getRootObject().data()->getReference();
        getRootObject().data()->setIsWritten(false);
        stringData.data()->setIsWritten(false);
        characterData.data()->setIsWritten(false);
        characterPropertyValues.data()->setIsWritten(false);
        if (mirroredSkeletonInfo.data()){
            mirroredSkeletonInfo.data()->setIsWritten(false);
        }else{
            FATAL_RUNTIME_ERROR("CharacterFile::write(): no mirroredSkeletonInfo!!");
        }
        if (handIkDriverInfo.data()){
            handIkDriverInfo.data()->setIsWritten(false);
        }
        if (footIkDriverInfo.data()){
            footIkDriverInfo.data()->setIsWritten(false);
        }
        ref++;
        stringData.data()->setReference(ref);
        ref++;
        characterData.data()->setReference(ref);
        ref++;
        characterPropertyValues.data()->setReference(ref);
        ref++;
        mirroredSkeletonInfo.data()->setReference(ref);
        ref++;
        if (handIkDriverInfo.data()){
            handIkDriverInfo.data()->setReference(ref);
            ref++;
        }
        if (footIkDriverInfo.data()){
            footIkDriverInfo.data()->setReference(ref);
            ref++;
        }
        for (int i = 0; i < boneWeightArrays.size(); i++, ref++){
            boneWeightArrays.at(i).data()->setIsWritten(false);
            boneWeightArrays.at(i).data()->setReference(ref);
        }
        getWriter().setFile(this);
        if (!getWriter().writeToXMLFile()){
            FATAL_RUNTIME_ERROR("CharacterFile::write(): writeToXMLFile() failed!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("CharacterFile::write(): The root object is nullptr!!");
    }
}

void CharacterFile::addFootIK(){
    if (characterData.data()){
        if (!footIkDriverInfo.data() && !static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo.data()){
            hkbFootIkDriverInfo *ptr = new hkbFootIkDriverInfo(this);
            addObjectToFile(ptr);
            footIkDriverInfo = HkxSharedPtr(ptr);
            static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo = HkxSharedPtr(ptr);
        }else{
            static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo = footIkDriverInfo;
        }
        setIsChanged(true);
    }
}

void CharacterFile::addHandIK(){
    if (characterData.data()){
        if (!handIkDriverInfo.data() && !static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo.data()){
            hkbHandIkDriverInfo *ptr = new hkbHandIkDriverInfo(this);
            addObjectToFile(ptr);
            handIkDriverInfo = HkxSharedPtr(ptr);
            static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo = HkxSharedPtr(ptr);
        }else{
            static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo = handIkDriverInfo;
        }
        setIsChanged(true);
    }
}

void CharacterFile::disableFootIK(){
    if (characterData.data() && !static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo.data()){
        static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo = HkxSharedPtr();
        setIsChanged(true);
    }
}

void CharacterFile::disableHandIK(){
    if (characterData.data() && !static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo.data()){
        static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo = HkxSharedPtr();
        setIsChanged(true);
    }
}

CharacterFile::~CharacterFile(){
    if (skeleton){
        delete skeleton;
    }
}
