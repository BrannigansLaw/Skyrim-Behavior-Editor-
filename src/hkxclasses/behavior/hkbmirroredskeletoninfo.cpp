#include "hkbmirroredskeletoninfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/projectfile.h"

/*
 * CLASS: hkbMirroredSkeletonInfo
*/

uint hkbMirroredSkeletonInfo::refCount = 0;

QString hkbMirroredSkeletonInfo::classname = "hkbMirroredSkeletonInfo";

hkbMirroredSkeletonInfo::hkbMirroredSkeletonInfo(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_MIRRORED_SKELETON_INFO, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbMirroredSkeletonInfo::getClassname(){
    return classname;
}

bool hkbMirroredSkeletonInfo::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "mirrorAxis"){
            mirrorAxis = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'mirrorAxis' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bonePairMap"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bonePairMap' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readIntegers(reader.getElementValueAt(index), bonePairMap)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'bonePairMap' data!\nObject Reference: "+ref, true);
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbMirroredSkeletonInfo::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString bones;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldUpWS"), worldUpWS.getValueAsString());
        list1 = {writer->name, writer->numelements};
        list2 = {"bonePairMap", QString::number(bonePairMap.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < bonePairMap.size(); i++){
            bones = bones+" "+QString::number(bonePairMap.at(i));
            if (i > 0 && i % 16 == 0){
                bones = bones+"\n";
            }
        }
        if (bonePairMap.size() > 0){
            writer->writeLine(bones);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbMirroredSkeletonInfo::link(){
    if (!getParentFile()){
        return false;
    }
    return true;
}

void hkbMirroredSkeletonInfo::unlink(){
    //
}

bool hkbMirroredSkeletonInfo::evaulateDataValidity(){
    setDataValidity(true);
    return true;
}

hkbMirroredSkeletonInfo::~hkbMirroredSkeletonInfo(){
    refCount--;
}
