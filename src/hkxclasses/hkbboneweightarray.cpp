#include "hkbboneweightarray.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/*
 * CLASS: hkbBoneWeightArray
*/

uint hkbBoneWeightArray::refCount = 0;

QString hkbBoneWeightArray::classname = "hkbBoneWeightArray";

hkbBoneWeightArray::hkbBoneWeightArray(BehaviorFile *parent, long ref)
    : HkDynamicObject(parent, ref)
{
    setType(HKB_BONE_WEIGHT_ARRAY, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
}

QString hkbBoneWeightArray::getClassname(){
    return classname;
}

bool hkbBoneWeightArray::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "boneWeights"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneWeights' data!\nObject Reference: "+ref, true);
                return false;
            }
            //index++;
            if (numElems > 0 && !readDoubles(reader.getElementValueAt(index), boneWeights)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'boneWeights' data!\nObject Reference: "+ref, true);
                return false;
            }
            //continue;
        }
        index++;
    }
    return true;
}

bool hkbBoneWeightArray::write(HkxXMLWriter *writer){
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
        list2 = {"boneWeights", QString::number(boneWeights.size())};
        writer->writeLine(writer->parameter, list1, list2, "");
        for (int i = 0; i < boneWeights.size(); i++){
            bones = bones+" "+getDoubleAsString(boneWeights.at(i));
            if (i > 0 && i % 16 == 0){
                bones = bones+"\n";
            }
        }
        if (boneWeights.size() > 0){
            writer->writeLine(bones);
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbBoneWeightArray::link(){
    if (!getParentFile()){
        return false;
    }
    //variableBindingSet
    if (!this->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\n");
    }
    return true;
}

hkbBoneWeightArray::~hkbBoneWeightArray(){
    refCount--;
}
