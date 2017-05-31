#include "characterfile.h"
#include "skeletonfile.h"
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

CharacterFile::CharacterFile(MainWindow *window, const QString & name)
    : HkxFile(window, name),
      skeleton(NULL),
      largestRef(0)
{
    getReader().setFile(this);
}

HkxSharedPtr * CharacterFile::findCharacterData(long ref){
    if (characterData->getReference() == ref){
        return &characterData;
    }
    return NULL;
}

HkxSharedPtr * CharacterFile::findCharacterPropertyValues(long ref){
    for (int i = 0; i < boneWeightArrays.size(); i++){
        if (boneWeightArrays.at(i).data() && boneWeightArrays.at(i).getReference() == ref){
            return &boneWeightArrays[i];
        }
    }
    return NULL;
}

QString CharacterFile::getRootBehaviorPath() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return ptr->behaviorFilename;
    }
    return "";
}

QString CharacterFile::getRigName() const{
    hkbCharacterStringData *ptr = static_cast<hkbCharacterStringData *>(stringData.data());
    if (ptr){
        return ptr->rigName;
    }
    return "";
}

hkbCharacterData * CharacterFile::getCharacterData() const{
    if (characterData.data() && characterData->getSignature() == HKB_CHARACTER_DATA){
        return static_cast<hkbCharacterData *>(characterData.data());
    }
    return NULL;
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

QString CharacterFile::getCharacterPropertyNameAt(int index) const{
    return static_cast<hkbCharacterStringData *>(stringData.data())->getCharacterPropertyNameAt(index);
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
    setProgressData("Creating HKX objects...", 60);
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
    setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("CharacterFile: parse() failed because link() failed!", true);
        return false;
    }
    return true;
}

bool CharacterFile::link(){
    if (!getRootObject().constData()){
        writeToLog("CharacterFile: link() failed!\nThe root object of this character file is NULL!", true);
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


void CharacterFile::write(){
    ulong ref = 1;
    getRootObject().data()->setReference(ref);
    ref++;
    characterData.data()->setReference(ref);
    ref++;
    getWriter().setFile(this);
    getWriter().writeToXMLFile();
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
    }
}

void CharacterFile::disableFootIK(){
    if (characterData.data() && !static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo.data()){
        static_cast<hkbCharacterData *>(characterData.data())->footIkDriverInfo = HkxSharedPtr();
    }
}

void CharacterFile::disableHandIK(){
    if (characterData.data() && !static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo.data()){
        static_cast<hkbCharacterData *>(characterData.data())->handIkDriverInfo = HkxSharedPtr();
    }
}

CharacterFile::~CharacterFile(){
    //
}
