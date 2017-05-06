#ifndef HKBSTRINGEVENTPAYLOAD_H
#define HKBSTRINGEVENTPAYLOAD_H

#include "src/hkxclasses/hkxobject.h"

class hkbStringEventPayload: public HkxObject
{
    friend class StateMachineUI;
    friend class StateUI;
    friend class EventUI;
public:
    hkbStringEventPayload(HkxFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkbStringEventPayload();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaulateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    bool write(HkxXMLWriter *writer);
protected:
private:
    static uint refCount;
    static QString classname;
    QString data;
};

#endif // HKBSTRINGEVENTPAYLOAD_H
