#include "hkbstringeventpayload.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"
/**
 * hkbStringEventPayload
 */

uint hkbStringEventPayload::refCount = 0;

QString hkbStringEventPayload::classname = "hkbStringEventPayload";

hkbStringEventPayload::hkbStringEventPayload(HkxFile *parent, const QString &string, long ref)
    : HkxObject(parent, ref),
      data(string)
{
    setType(HKB_STRING_EVENT_PAYLOAD, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkbStringEventPayload::getClassname(){
    return classname;
}

bool hkbStringEventPayload::readData(const HkxXmlReader & reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "data"){
            data = reader.getElementValueAt(index);
            if (data == ""){
                LogFile::writeToLog(getParentFile()->getFileName()+": "+getClassname()+": readData()!\nFailed to properly read 'data' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbStringEventPayload::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("data"), data);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
    }
    return true;
}

QString hkbStringEventPayload::evaluateDataValidity(){
    if (data == ""){
        setDataValidity(false);
        return QString();
    }
    setDataValidity(true);
    return QString();
}

hkbStringEventPayload::~hkbStringEventPayload(){
    refCount--;
}
