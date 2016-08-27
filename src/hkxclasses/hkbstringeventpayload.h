#ifndef HKBSTRINGEVENTPAYLOAD_H
#define HKBSTRINGEVENTPAYLOAD_H

#include "hkxobject.h"

class hkbStringEventPayload: public HkxObject
{
public:
    hkbStringEventPayload(BehaviorFile *parent = NULL/*, long ref = 0*/);
protected:
private:
    static uint refCount;
    QString data;
};

#endif // HKBSTRINGEVENTPAYLOAD_H
