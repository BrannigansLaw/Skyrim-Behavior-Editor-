#include "hkaskeleton.h"
#include "src/hkxclasses/hksimplelocalframe.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/skeletonfile.h"

/*
 * CLASS: hkaSkeleton
*/

uint hkaSkeleton::refCount = 0;

QString hkaSkeleton::classname = "hkaSkeleton";

hkaSkeleton::hkaSkeleton(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKA_SKELETON, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkaSkeleton::getClassname(){
    return classname;
}

QStringList hkaSkeleton::getBoneNames() const{
    QStringList list;
    for (int i = 0; i < bones.size(); i++){
        list.append(bones.at(i).name);
    }
    return list;
}

QString hkaSkeleton::getLocalFrameName(int boneIndex) const{
    for (int i = 0; i < localFrames.size(); i++){
        if (boneIndex == localFrames.at(i).boneIndex && localFrames.at(i).localFrame.data() && localFrames.at(i).localFrame->getSignature() == HK_SIMPLE_LOCAL_FRAME){
            return static_cast<hkSimpleLocalFrame *>(localFrames.at(i).localFrame.data())->name;
        }
    }
    return "";
}

bool hkaSkeleton::addLocalFrame(const QString & name){
    for (int i = 0; i < localFrames.size(); i++){
        if (localFrames.at(i).localFrame.data() && localFrames.at(i).localFrame->getSignature() == HK_SIMPLE_LOCAL_FRAME){
            if (static_cast<hkSimpleLocalFrame *>(localFrames[i].localFrame.data())->name == name){
                return false;
            }
        }
    }
    localFrames.append(hkLocalFrame());
    localFrames.last().localFrame = HkxSharedPtr(new hkSimpleLocalFrame(getParentFile(), name));
    if (!getParentFile()->addObjectToFile(localFrames.last().localFrame.data(), ref)){
        return false;
    }
    return true;
}

void hkaSkeleton::setLocalFrameName(int boneIndex, const QString & name){
    for (int i = 0; i < localFrames.size(); i++){
        if (boneIndex == localFrames.at(i).boneIndex && localFrames.at(i).localFrame.data() && localFrames.at(i).localFrame->getSignature() == HK_SIMPLE_LOCAL_FRAME){
            if (name == ""){
                static_cast<SkeletonFile *>(getParentFile())->localFrames.removeAll(localFrames.at(i).localFrame);
                localFrames.removeAt(i);
            }else{
                static_cast<hkSimpleLocalFrame *>(localFrames[i].localFrame.data())->name = name;
            }
            return;
        }
    }
}

bool hkaSkeleton::removeLocalFrame(int boneIndex){
    for (int i = 0; i < localFrames.size(); i++){
        if (boneIndex == localFrames.at(i).boneIndex){
            static_cast<SkeletonFile *>(getParentFile())->localFrames.removeAll(localFrames.at(i).localFrame);
            localFrames.removeAt(i);
            return true;
        }
    }
    return false;
}

bool hkaSkeleton::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    int numtrans = 0;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "parentIndices"){
            int numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'parentIndices' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readIntegers(reader.getElementValueAt(index), parentIndices)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'parentIndices' data!\nObject Reference: "+ref, true);
                return false;
            }
        }else if (text == "bones"){
            numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                bones.append(hkBone());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (reader.getNthAttributeValueAt(index, 0) == "name"){
                        bones.last().name = reader.getElementValueAt(index);
                        if (bones.last().name == ""){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "lockTranslation"){
                        bones.last().lockTranslation = toBool(reader.getElementValueAt(index), &ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'lockTranslation' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }else if (text == "referencePose"){
            referencePose = reader.getElementValueAt(index);
            if (referencePose == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'referencePose' data field!\nObject Reference: "+ref);
            }
        }else if (text == "referenceFloats"){
            int numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'referenceFloats' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readDoubles(reader.getElementValueAt(index), referenceFloats)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'referenceFloats' data!\nObject Reference: "+ref, true);
                return false;
            }
        }else if (text == "floatSlots"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'floatSlots' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                floatSlots.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "localFrames"){
            numtrans = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                return false;
            }
            for (int j = 0; j < numtrans; j++){
                localFrames.append(hkLocalFrame());
                while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
                    if (text == "localFrame"){
                        if (!localFrames.last().localFrame.readReference(index, reader)){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'localFrame' reference!\nObject Reference: "+ref);
                        }
                    }else if (reader.getNthAttributeValueAt(index, 0) == "lockTranslation"){
                        localFrames.last().boneIndex = reader.getElementValueAt(index).toInt(&ok);
                        if (!ok){
                            writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneIndex' data field!\nObject Reference: "+ref);
                        }
                        index++;
                        break;
                    }
                    index++;
                }
            }
        }
        index++;
    }
    return true;
}

bool hkaSkeleton::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString bonesS;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        list1 = {writer->name, writer->numelements};
        list2 = {"parentIndices", QString::number(parentIndices.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < parentIndices.size(); i++){
            bonesS = bonesS+QString::number(parentIndices.at(i));
            if (i > 0 && i % 16 == 0){
                bonesS = bonesS+"\n";
            }else{
                bonesS = bonesS+" ";
            }
        }
        if (parentIndices.size() > 0){
            writer->writeLine(bonesS);
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"bones", QString::number(bones.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < bones.size(); i++){
            writer->writeLine(writer->object, true);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), bones.at(i).name);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableIndex"), getBoolAsString(bones.at(i).lockTranslation));
            writer->writeLine(writer->object, false);
        }
        if (bones.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        bonesS = "";
        list1 = {writer->name, writer->numelements};
        list2 = {"referenceFloats", QString::number(referenceFloats.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < referenceFloats.size(); i++){
            bonesS = bonesS+QString::number(referenceFloats.at(i), char('f'), 6);
            if (i > 0 && i % 16 == 0){
                bonesS = bonesS+"\n";
            }else{
                bonesS = bonesS+" ";
            }
        }
        if (referenceFloats.size() > 0){
            writer->writeLine(bonesS);
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"floatSlots", QString::number(floatSlots.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < floatSlots.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), floatSlots.at(i));
        }
        if (floatSlots.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        QString refString = "null";
        list1 = {writer->name, writer->numelements};
        list2 = {"localFrames", QString::number(localFrames.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < localFrames.size(); i++){
            writer->writeLine(writer->object, true);
            if (localFrames.at(i).localFrame.data()){
                refString = localFrames.at(i).localFrame.data()->getReferenceString();
            }else{
                refString = "null";
            }
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("localFrame"), refString);
            writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("boneIndex"), QString::number(localFrames.at(i).boneIndex));
            writer->writeLine(writer->object, false);
        }
        if (localFrames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (int i = 0; i < localFrames.size(); i++){
            if (localFrames.at(i).localFrame.data() && !localFrames.at(i).localFrame->write(writer)){
                getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'localFrame' at: "+QString::number(i)+"!!!", true);
            }
        }
    }
    return true;
}

bool hkaSkeleton::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr;
    for (int i = 0; i < localFrames.size(); i++){
        ptr = static_cast<SkeletonFile *>(getParentFile())->findLocalFrame(localFrames.at(i).localFrame.getReference());
        if (!ptr){
            writeToLog(getClassname()+": link()!\nFailed to properly link 'localFrames' data field!\nObject Name: "+name);
            setDataValidity(false);
        }else if (!(*ptr).data() || (*ptr)->getSignature() != HK_SIMPLE_LOCAL_FRAME){
            writeToLog(getClassname()+": link()!\n'localFrames' data field is linked to invalid child!\nObject Name: "+name);
            setDataValidity(false);
            localFrames[i].localFrame = *ptr;
        }else{
            localFrames[i].localFrame = *ptr;
        }
    }
    return true;
}

void hkaSkeleton::unlink(){
    for (int i = 0; i < localFrames.size(); i++){
        localFrames[i].localFrame = HkxSharedPtr();
    }
}

bool hkaSkeleton::evaulateDataValidity(){
    for (int i = 0; i < localFrames.size(); i++){
        if (!localFrames.at(i).localFrame.data() || localFrames.at(i).localFrame.data()->getSignature() != HK_SIMPLE_LOCAL_FRAME){
            setDataValidity(false);
            return false;
        }
    }
    setDataValidity(true);
    return true;
}

hkaSkeleton::~hkaSkeleton(){
    refCount--;
}
