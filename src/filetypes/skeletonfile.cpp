#include "skeletonfile.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/hksimplelocalframe.h"
#include "src/hkxclasses/animation/hkaskeleton.h"
#include "src/hkxclasses/animation/hkaanimationcontainer.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

SkeletonFile::SkeletonFile(MainWindow *window, const QString & name)
    : HkxFile(window, name),
      largestRef(0)
{
    getReader().setFile(this);
}

HkxObjectExpSharedPtr * SkeletonFile::findSkeleton(long ref){
    for (int i = 0; i < skeletons.size(); i++){
        if (skeletons.at(i).data() && skeletons.at(i).getReference() == ref){
            return &skeletons[i];
        }
    }
    return NULL;
}

HkxObjectExpSharedPtr * SkeletonFile::findLocalFrame(long ref){
    for (int i = 0; i < localFrames.size(); i++){
        if (localFrames.at(i).data() && localFrames.at(i).getReference() == ref){
            return &localFrames[i];
        }
    }
    return NULL;
}

QStringList SkeletonFile::getBonesFromSkeletonAt(int index) const{
    if (index < skeletons.size() && index >= 0){
        if (skeletons.at(index).data()){
            return static_cast<hkaSkeleton *>(skeletons.at(index).data())->getBoneNames();
        }
    }
    return QStringList();
}

bool SkeletonFile::addObjectToFile(HkxObject *obj, long ref){
    if (ref > largestRef){
        largestRef = ref;
    }else{
        largestRef++;
    }
    obj->setReference(largestRef);
    if (obj->getSignature() == HKA_SKELETON){
        skeletons.append(HkxObjectExpSharedPtr(obj, ref));
    }else if (obj->getSignature() == HK_SIMPLE_LOCAL_FRAME){
        localFrames.append(HkxObjectExpSharedPtr(obj, ref));
    }else if (obj->getSignature() == HK_ROOT_LEVEL_CONTAINER){
        setRootObject(HkxObjectExpSharedPtr(obj, ref));
    }else{
        writeToLog("SkeletonFile: addObjectToFile() failed!\nInvalid type enum for this object!\nObject signature is: "+QString::number(obj->getSignature(), 16), true);
        return false;
    }
    return true;
}

bool SkeletonFile::parse(){
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
                    writeToLog("SkeletonFile: parse() failed!\nThe object reference string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                signature = (HkxSignature)value.toULongLong(&ok, 16);
                if (!ok){
                    writeToLog("SkeletonFile: parse() failed!\nThe object signature string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                if (signature == HK_SIMPLE_LOCAL_FRAME){
                    if (!appendAndReadData(index, new hkSimpleLocalFrame(this, "", ref))){
                        return false;
                    }
                }else if (signature == HKA_SKELETON){
                    if (!appendAndReadData(index, new hkaSkeleton(this, ref))){
                        return false;
                    }
                }else if (signature == HKA_ANIMATION_CONTAINER){
                    if (!appendAndReadData(index, new hkaAnimationContainer(this, ref))){
                        return false;
                    }
                }else if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this, ref))){
                        return false;
                    }
                }else{
                    writeToLog("SkeletonFile: parse()!\nUnknown signature detected!\nUnknown object class name is: "+getReader().getNthAttributeValueAt(index, 1)+"\nUnknown object signature is: "+QString::number(signature, 16));
                }
            }
        }
        index++;
    }
    closeFile();
    getReader().clear();
    setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("SkeletonFile: parse() failed because link() failed!", true);
        return false;
    }
    return true;
}

bool SkeletonFile::link(){
    if (!getRootObject().constData()){
        writeToLog("SkeletonFile: link() failed!\nThe root object of this character file is NULL!", true);
        return false;
    }else if (getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        writeToLog("SkeletonFile: link() failed!\nThe root object of this character file is NOT a hkRootLevelContainer!\nThe root object signature is: "+QString::number(getRootObject()->getSignature(), 16), true);
        return false;
    }
    if (!getRootObject().data()->link()){
        writeToLog("SkeletonFile: link() failed!\nThe root object of this character file failed to link to it's children!", true);
        return false;
    }
    return true;
}


void SkeletonFile::write(){
    ulong ref = 1;
    getRootObject().data()->setReference(ref);
    ref++;
    getWriter().setFile(this);
    getWriter().writeToXMLFile();
}

SkeletonFile::~SkeletonFile(){
    //
}
