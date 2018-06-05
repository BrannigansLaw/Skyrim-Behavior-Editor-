#include "hkbmirroredskeletoninfo.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/projectfile.h"

/*
 * CLASS: hkbMirroredSkeletonInfo
*/

uint hkbMirroredSkeletonInfo::refCount = 0;

QString hkbMirroredSkeletonInfo::classname = "hkbMirroredSkeletonInfo";

hkbMirroredSkeletonInfo::hkbMirroredSkeletonInfo(HkxFile *parent, long ref, int size)
    : HkxObject(parent, ref),
      mirrorAxis(1, 0, 0, 0)
{
    setType(HKB_MIRRORED_SKELETON_INFO, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    //bonePairMap.reserve(size);
    for (int i = 0; i < size; i++){
        bonePairMap.append(0);
    }
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
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'mirrorAxis' data field!\nObject Reference: "+ref);
            }
        }else if (text == "bonePairMap"){
            int numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'bonePairMap' data!\nObject Reference: "+ref);
                return false;
            }
            if (numElems > 0 && !readIntegers(reader.getElementValueAt(index), bonePairMap)){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'bonePairMap' data!\nObject Reference: "+ref);
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("mirrorAxis"), mirrorAxis.getValueAsString());
        list1 = {writer->name, writer->numelements};
        list2 = {"bonePairMap", QString::number(bonePairMap.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0, j = 1; i < bonePairMap.size(); i++, j++){
            bones.append(QString::number(bonePairMap.at(i)));
            if (j % 16 == 0){
                bones.append("\n");
            }else{
                bones.append(" ");
            }
        }
        if (bonePairMap.size() > 0){
            if (bones.endsWith(" \0")){
                bones.remove(bones.lastIndexOf(" "), 1);
            }
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

bool hkbMirroredSkeletonInfo::evaluateDataValidity(){
    setDataValidity(true);
    return true;
}

hkbMirroredSkeletonInfo::~hkbMirroredSkeletonInfo(){
    refCount--;
}
