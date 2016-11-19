#include "hkbstringeventpayload.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/**
 * hkbStringEventPayload
 */

uint hkbStringEventPayload::refCount = 0;

QString hkbStringEventPayload::classname = "hkbStringEventPayload";

hkbStringEventPayload::hkbStringEventPayload(BehaviorFile *parent, const QString &string, long ref)
    : HkxObject(parent, ref),
      data(string)
{
    setType(HKB_STRING_EVENT_PAYLOAD, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);refCount++;
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
                writeToLog("hkbStringEventPayload: readData()!\nFailed to properly read 'data' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbStringEventPayload::evaulateDataValidity(){
    if (data == ""){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

hkbStringEventPayload::~hkbStringEventPayload(){
    refCount--;
}
