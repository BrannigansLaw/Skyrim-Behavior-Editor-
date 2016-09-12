#ifndef HKBSTRINGEVENTPAYLOAD_H
#define HKBSTRINGEVENTPAYLOAD_H

#include "hkxobject.h"

class hkbStringEventPayload: public HkxObject
{
    friend class StateMachineUI;
public:
    hkbStringEventPayload(BehaviorFile *parent = NULL/*, long ref = 0*/);
    virtual ~hkbStringEventPayload();
    bool evaulateDataValidity();
    static QString getClassname();
protected:
private:
    static uint refCount;
    static QString classname;
    QString data;
};

#endif // HKBSTRINGEVENTPAYLOAD_H
