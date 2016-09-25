#ifndef HKBSTRINGEVENTPAYLOAD_H
#define HKBSTRINGEVENTPAYLOAD_H

#include "hkxobject.h"

class hkbStringEventPayload: public HkxObject
{
    friend class StateMachineUI;
public:
    hkbStringEventPayload(BehaviorFile *parent, const QString & string = "");
    virtual ~hkbStringEventPayload();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaulateDataValidity();
    static QString getClassname();
    bool link(){return true;}
protected:
private:
    static uint refCount;
    static QString classname;
    QString data;
};

#endif // HKBSTRINGEVENTPAYLOAD_H
