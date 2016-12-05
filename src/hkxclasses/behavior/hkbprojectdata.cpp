#include "hkbprojectdata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/projectfile.h"

/*
 * CLASS: hkbProjectData
*/

uint hkbProjectData::refCount = 0;

QString hkbProjectData::classname = "hkbProjectData";

QStringList hkbProjectData::EventMode = {"EVENT_MODE_IGNORE_FROM_GENERATOR"};

hkbProjectData::hkbProjectData(HkxFile *parent, long ref)
    : HkxObject(parent, ref)
{
    setType(HKB_PROJECT_DATA, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbProjectData::getClassname(){
    return classname;
}

bool hkbProjectData::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "worldUpWS"){
            worldUpWS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'worldUpWS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "stringData"){
            if (!stringData.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'stringData' reference!\nObject Reference: "+ref);
            }
        }else if (text == "defaultEventMode"){
            defaultEventMode = reader.getElementValueAt(index);
            if (!EventMode.contains(defaultEventMode)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'defaultEventMode' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbProjectData::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("worldUpWS"), worldUpWS.getValueAsString());
        if (stringData.data()){
            refString = stringData.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("stringData"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("defaultEventMode"), defaultEventMode);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (stringData.data() && !stringData.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'stringData'!!!", true);
        }
    }
    return true;
}

bool hkbProjectData::link(){
    if (!getParentFile()){
        return false;
    }
    HkxObjectExpSharedPtr *ptr;
    ProjectFile *file = dynamic_cast<ProjectFile *>(getParentFile());
    if (file){
        ptr = file->findProjectData(stringData.getReference());
    }else{
        writeToLog(getClassname()+": link()!\nParent file type is invalid!!!", true);
    }
    if (!ptr){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'stringData' data field!\n");
        setDataValidity(false);
    }else if ((*ptr)->getSignature() != HKB_PROJECT_STRING_DATA){
        writeToLog(getClassname()+": link()!\n'stringData' data field is linked to invalid child!\n");
        setDataValidity(false);
        stringData = *ptr;
    }else{
        stringData = *ptr;
    }
    ptr = &file->stringData;
    return true;
}

void hkbProjectData::unlink(){
    stringData = HkxObjectExpSharedPtr();
}

bool hkbProjectData::evaulateDataValidity(){
    if (!EventMode.contains(defaultEventMode)){
    }else if (!stringData.data() || stringData.data()->getSignature() != HKB_PROJECT_STRING_DATA){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbProjectData::~hkbProjectData(){
    refCount--;
}
