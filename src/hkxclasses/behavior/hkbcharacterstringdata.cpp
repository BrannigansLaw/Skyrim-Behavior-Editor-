#include "hkbcharacterstringdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/projectfile.h"
/*
 * CLASS: hkbCharacterStringData
*/

uint hkbCharacterStringData::refCount = 0;

QString hkbCharacterStringData::classname = "hkbCharacterStringData";

hkbCharacterStringData::hkbCharacterStringData(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_CHARACTER_STRING_DATA, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbCharacterStringData::getClassname(){
    return classname;
}

QStringList hkbCharacterStringData::getAnimationNames() const{
    return QStringList(animationNames);
}

QString hkbCharacterStringData::getCharacterPropertyNameAt(int index) const{
    if (characterPropertyNames.size() > index && index >= 0){
        return characterPropertyNames.at(index);
    }
    return "";
}

void hkbCharacterStringData::addAnimation(const QString & name){
    animationNames.append(name);
    getParentFile()->setIsChanged(true);
}

QString hkbCharacterStringData::getAnimationNameAt(int index) const{
    if (animationNames.size() > index && index >= 0){
        return animationNames.at(index);
    }
    return "";
}

int hkbCharacterStringData::getAnimationIndex(const QString &name) const{
    for (int i = 0; i < animationNames.size(); i++){
        if (name.compare(animationNames.at(i), Qt::CaseInsensitive) == 0){
            return i;
        }
    }
    return -1;
}

bool hkbCharacterStringData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    int numElems = 0;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "deformableSkinNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'deformableSkinNames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'deformableSkinNames' data!\nObject Reference: "+ref);
                    return false;
                }
                deformableSkinNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "rigidSkinNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rigidSkinNames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rigidSkinNames' data!\nObject Reference: "+ref);
                    return false;
                }
                rigidSkinNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "animationNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'animationNames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'animationNames' data!\nObject Reference: "+ref);
                    return false;
                }
                animationNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "characterPropertyNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'characterPropertyNames' data!\nObject Reference: "+ref);
                    return false;
                }
                characterPropertyNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "retargetingSkeletonMapperFilenames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'retargetingSkeletonMapperFilenames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'retargetingSkeletonMapperFilenames' data!\nObject Reference: "+ref);
                    return false;
                }
                retargetingSkeletonMapperFilenames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "lodNames"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'lodNames' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'lodNames' data!\nObject Reference: "+ref);
                    return false;
                }
                lodNames.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "mirroredSyncPointSubstringsA"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'mirroredSyncPointSubstringsA' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'mirroredSyncPointSubstringsA' data!\nObject Reference: "+ref);
                    return false;
                }
                mirroredSyncPointSubstringsA.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "mirroredSyncPointSubstringsB"){
            numElems = reader.getNthAttributeValueAt(index, 1).toInt(&ok);
            if (!ok){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'mirroredSyncPointSubstringsB' data!\nObject Reference: "+ref);
                return false;
            }
            index++;
            numElems = numElems + index;
            for (; index < numElems; index++){
                if (reader.getElementNameAt(index) != "hkcstring" || index >= reader.getNumElements()){
                    LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'mirroredSyncPointSubstringsB' data!\nObject Reference: "+ref);
                    return false;
                }
                mirroredSyncPointSubstringsB.append(reader.getElementValueAt(index));
            }
            continue;
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "rigName"){
            rigName = reader.getElementValueAt(index);
            if (rigName == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'rigName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "ragdollName"){
            ragdollName = reader.getElementValueAt(index);
            if (ragdollName == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'ragdollName' data field!\nObject Reference: "+ref);
            }
        }else if (text == "behaviorFilename"){
            behaviorFilename = reader.getElementValueAt(index);
            if (behaviorFilename == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'behaviorFilename' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbCharacterStringData::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        QStringList list3 = {writer->name, writer->numelements};
        QStringList list4 = {"deformableSkinNames", QString::number(deformableSkinNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < deformableSkinNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), deformableSkinNames.at(i));
        }
        if (deformableSkinNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"rigidSkinNames", QString::number(rigidSkinNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < rigidSkinNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), rigidSkinNames.at(i));
        }
        if (rigidSkinNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"animationNames", QString::number(animationNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < animationNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), QString(animationNames.at(i)).replace("/", "\\"));
        }
        if (animationNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"animationFilenames", QString::number(animationFilenames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < animationFilenames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), animationFilenames.at(i));
        }
        if (animationFilenames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"characterPropertyNames", QString::number(characterPropertyNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < characterPropertyNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), characterPropertyNames.at(i));
        }
        if (characterPropertyNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"retargetingSkeletonMapperFilenames", QString::number(retargetingSkeletonMapperFilenames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < retargetingSkeletonMapperFilenames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), retargetingSkeletonMapperFilenames.at(i));
        }
        if (retargetingSkeletonMapperFilenames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"lodNames", QString::number(lodNames.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < lodNames.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), lodNames.at(i));
        }
        if (lodNames.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"mirroredSyncPointSubstringsA", QString::number(mirroredSyncPointSubstringsA.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < mirroredSyncPointSubstringsA.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), mirroredSyncPointSubstringsA.at(i));
        }
        if (mirroredSyncPointSubstringsA.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        list3 = {writer->name, writer->numelements};
        list4 = {"mirroredSyncPointSubstringsB", QString::number(mirroredSyncPointSubstringsB.size())};
        writer->writeLine(writer->parameter, list3, list4, "");
        for (int i = 0; i < mirroredSyncPointSubstringsB.size(); i++){
            writer->writeLine(writer->string, QStringList(), QStringList(), mirroredSyncPointSubstringsB.at(i));
        }
        if (mirroredSyncPointSubstringsB.size() > 0){
            writer->writeLine(writer->parameter, false);
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("rigName"), QString(rigName).replace("/", "\\"));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("ragdollName"), QString(ragdollName).replace("/", "\\"));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("behaviorFilename"), QString(behaviorFilename).replace("/", "\\"));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

bool hkbCharacterStringData::link(){
    return true;
}

QString hkbCharacterStringData::evaluateDataValidity(){
    for (int i = 0; i < deformableSkinNames.size(); i++){
        if (deformableSkinNames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < rigidSkinNames.size(); i++){
        if (rigidSkinNames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < animationNames.size(); i++){
        if (animationNames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < animationFilenames.size(); i++){
        if (animationFilenames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < characterPropertyNames.size(); i++){
        if (characterPropertyNames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < retargetingSkeletonMapperFilenames.size(); i++){
        if (retargetingSkeletonMapperFilenames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < lodNames.size(); i++){
        if (lodNames.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < mirroredSyncPointSubstringsA.size(); i++){
        if (mirroredSyncPointSubstringsA.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    for (int i = 0; i < mirroredSyncPointSubstringsB.size(); i++){
        if (mirroredSyncPointSubstringsB.at(i) == ""){
            setDataValidity(false);
            return QString();
        }
    }
    if (name == "" || rigName == "" || ragdollName == "" || behaviorFilename == ""){
        setDataValidity(false);
        return QString();
    }
    setDataValidity(true);
    return QString();
}

hkbCharacterStringData::~hkbCharacterStringData(){
    refCount--;
}
