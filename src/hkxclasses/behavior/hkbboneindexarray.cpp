#include "hkbboneindexarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/*
 * CLASS: hkbBoneIndexArray
*/

uint hkbBoneIndexArray::refCount = 0;

QString hkbBoneIndexArray::classname = "hkbBoneIndexArray";

hkbBoneIndexArray::hkbBoneIndexArray(HkxFile *parent, long ref)
    : HkDynamicObject(parent, ref)
{
    setType(HKB_BONE_INDEX_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbBoneIndexArray::getClassname(){
    return classname;
}

bool hkbBoneIndexArray::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "boneIndices"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneIndices' data!\nObject Reference: "+ref, true);
                return false;
            }
            if (numElems > 0 && !readIntegers(reader.getElementValueAt(index), boneIndices)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneIndices' data!\nObject Reference: "+ref, true);
                return false;
            }
        }
        index++;
    }
    return true;
}

bool hkbBoneIndexArray::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QString bones;
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        list1 = {writer->name, writer->numelements};
        list2 = {"boneIndices", QString::number(boneIndices.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < boneIndices.size(); i++){
            bones = bones+QString::number(boneIndices.at(i));
            if (i > 0 && i % 15 == 0){
                bones = bones+"\n";
            }else{
                bones = bones+" ";
            }
        }
        if (boneIndices.size() > 0){
            writer->writeLine(bones);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbBoneIndexArray::link(){
    if (!getParentFile()){
        return false;
    }
    if (!this->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\n");
    }
    return true;
}

hkbBoneIndexArray::~hkbBoneIndexArray(){
    refCount--;
}
