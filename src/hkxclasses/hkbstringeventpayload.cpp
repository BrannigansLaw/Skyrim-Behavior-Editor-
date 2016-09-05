#include "hkbstringeventpayload.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/**
 * hkbStringEventPayload
 */

uint hkbStringEventPayload::refCount = 0;

QString hkbStringEventPayload::classname = "hkbStringEventPayload";

hkbStringEventPayload::hkbStringEventPayload(BehaviorFile *parent/*, long ref = 0*/)
    : HkxObject(parent/*, ref*/)
{
    refCount++;
    setType(HKB_STRING_EVENT_PAYLOAD, TYPE_OTHER);
}

QString hkbStringEventPayload::getClassname(){
    return classname;
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
