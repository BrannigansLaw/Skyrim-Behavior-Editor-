#include "hkbstringeventpayload.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"
/**
 * hkbStringEventPayload
 */

uint hkbStringEventPayload::refCount = 0;

hkbStringEventPayload::hkbStringEventPayload(BehaviorFile *parent/*, long ref = 0*/)
    : HkxObject(parent/*, ref*/)
{
    refCount++;
    setType(HKB_STRING_EVENT_PAYLOAD, TYPE_OTHER);
}
