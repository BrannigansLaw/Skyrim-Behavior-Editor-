#include "hkbprojectstringdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/projectfile.h"
/*
 * CLASS: hkbProjectStringData
*/

uint hkbProjectStringData::refCount = 0;

QString hkbProjectStringData::classname = "hkbProjectStringData";

hkbProjectStringData::hkbProjectStringData(HkxFile *parent, long ref, const QString &characterfilename)
    : HkxObject(parent, ref)
{
    setType(HKB_PROJECT_STRING_DATA, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    if (characterfilename != ""){
        characterFilenames.append(characterfilename);
    }
}

QString hkbProjectStringData::getClassname(){
    return classname;
}

QString hkbProjectStringData::getCharacterFilePathAt(int index) const{
    if (index < characterFilenames.size()){
        return QString(characterFilenames.at(index)).replace("\\", "/").section("/", -2, -1);
    }
    return "";
}

bool hkbProjectStringData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "animationFilenames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'animationFilenames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'animationFilenames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                animationFilenames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "behaviorFilenames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'behaviorFilenames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'behaviorFilenames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                behaviorFilenames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "characterFilenames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterFilenames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterFilenames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                characterFilenames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "eventNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    writeToLog(getClassname()+": readData()!\nFailed to properly read 'eventNames' data!\nObject Reference: "+ref, true);
                    return false;
                }
                eventNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "animationPath"){
            animationPath = reader.getElementValueAt(index);
            if (animationPath == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'animationPath' data field!\nObject Reference: "+ref);
            }
        }else if (text == "behaviorPath"){
            behaviorPath = reader.getElementValueAt(index);
            if (behaviorPath == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'behaviorPath' data field!\nObject Reference: "+ref);
            }
        }else if (text == "characterPath"){
            characterPath = reader.getElementValueAt(index);
            if (characterPath == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'characterPath' data field!\nObject Reference: "+ref);
            }
        }else if (text == "fullPathToSource"){
            fullPathToSource = reader.getElementValueAt(index);
            if (fullPathToSource == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'fullPathToSource' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbProjectStringData::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        QStringList list3 = {writer->name, writer->numelements};
        QStringList list4 = {"animationFilenames", QString::number(animationFilenames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < animationFilenames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), animationFilenames.at(i));
        }
        if (animationFilenames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"behaviorFilenames", QString::number(behaviorFilenames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < behaviorFilenames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), behaviorFilenames.at(i));
        }
        if (behaviorFilenames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"characterFilenames", QString::number(characterFilenames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < characterFilenames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), QString(characterFilenames.at(i)).replace("/", "\\"));
        }
        if (characterFilenames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"eventNames", QString::number(eventNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < eventNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), eventNames.at(i));
        }
        if (eventNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("animationPath"), animationPath, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("behaviorPath"), behaviorPath, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("characterPath"), characterPath, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("fullPathToSource"), fullPathToSource, true);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbProjectStringData::link(){
    return true;
}

bool hkbProjectStringData::evaluateDataValidity(){
    for (int i = 0; i < animationFilenames.size(); i++){
        if (animationFilenames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < behaviorFilenames.size(); i++){
        if (behaviorFilenames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < characterFilenames.size(); i++){
        if (characterFilenames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    for (int i = 0; i < eventNames.size(); i++){
        if (eventNames.at(i) == ""){
            setDataValidity(false);
            return false;
        }
    }
    if (animationPath == "" || behaviorPath == "" || characterPath == "" || fullPathToSource == ""){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

hkbProjectStringData::~hkbProjectStringData(){
    refCount--;
}
