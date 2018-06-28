#ifndef HKBSTRINGEVENTPAYLOAD_H
#define HKBSTRINGEVENTPAYLOAD_H

#include "src/hkxclasses/hkxobject.h"

class hkbStringEventPayload: public HkxObject
{
public:
    hkbStringEventPayload(HkxFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkbStringEventPayload();
    bool readData(const HkxXmlReader & reader, long index);
    QString evaluateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    bool write(HkxXMLWriter *writer);
public:
    QString data;
protected:
private:
    static uint refCount;
    static QString classname;
};

#endif // HKBSTRINGEVENTPAYLOAD_H
