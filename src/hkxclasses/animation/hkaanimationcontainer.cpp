#include "hkaanimationcontainer.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/skeletonfile.h"
#include "src/filetypes/characterfile.h"

/*
 * CLASS: hkaAnimationContainer
*/

uint hkaAnimationContainer::refCount = 0;

QString hkaAnimationContainer::classname = "hkaAnimationContainer";

hkaAnimationContainer::hkaAnimationContainer(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKA_ANIMATION_CONTAINER, TYPE_OTHER);
    parent->addObjectToFile(this, ref);
    refCount++;
}

QString hkaAnimationContainer::getClassname(){
    return classname;
}

bool hkaAnimationContainer::readData(const HkxXmlReader &reader, long & index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "skeletons"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nFailed to properly read 'skeletons' data!\nObject Reference: "+ref);
                return false;
            }
            if (numElems > 0 && !readReferences(reader.getElementValueAt(index), skeletons)){
                LogFile::writeToLog(getParentFilename()+": "+getClassname()+": readData()!\nFailed to properly read 'skeletons' data!\nObject Reference: "+ref);
                return false;
            }
        }
        index++;
    }
    index--;
    return true;
}

bool hkaAnimationContainer::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        list1 = {writer->name, writer->numelements};
        list2 = {"skeletons", QString::number(skeletons.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        QString refs;
        for (auto i = 0; i < skeletons.size(); i++){
            if (skeletons.at(i).data()){
                refs = refs + skeletons.at(i)->getReferenceString()+" ";
            }
        }
        writer->writeLine(refs);
        if (skeletons.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list1 = {writer->name, writer->numelements};
        list2 = {"animations", "0"};
        writer->writeLine(writer->parameter, list1, list2, " ");
        list2 = {"bindings", "0"};
        writer->writeLine(writer->parameter, list1, list2, " ");
        list2 = {"attachments", "0"};
        writer->writeLine(writer->parameter, list1, list2, " ");
        list2 = {"skins", "0"};
        writer->writeLine(writer->parameter, list1, list2, " ");
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        for (auto i = 0; i < skeletons.size(); i++){
            if (skeletons.at(i).data()){
                skeletons.at(i)->write(writer);
            }
        }
    }
    return true;
}

bool hkaAnimationContainer::link(){
    if (!getParentFile()){
        return false;
    }
    HkxSharedPtr *ptr = nullptr;
    for (auto i = 0; i < skeletons.size(); i++){
        ptr = static_cast<SkeletonFile *>(getParentFile())->findSkeleton(skeletons.at(i).getShdPtrReference());
        if (!ptr || !ptr->data()){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\nFailed to properly link 'skeletons' data field!\n");
            setDataValidity(false);
        }else if (!(*ptr).data() || (*ptr)->getSignature() != HKA_SKELETON){
            LogFile::writeToLog(getParentFilename()+": "+getClassname()+": link()!\n'skeletons' data field is linked to invalid child!\n");
            setDataValidity(false);
            skeletons[i] = *ptr;
        }else{
            skeletons[i] = *ptr;
        }
    }
    return true;
}

QString hkaAnimationContainer::evaluateDataValidity(){
    for (auto i = 0; i < skeletons.size(); i++){
        if (!skeletons.at(i).data() || skeletons.at(i)->getSignature() != HKA_SKELETON){
            setDataValidity(false);
            return QString();
        }
    }
    setDataValidity(true);
    return QString();
}

hkaAnimationContainer::~hkaAnimationContainer(){
    refCount--;
}
