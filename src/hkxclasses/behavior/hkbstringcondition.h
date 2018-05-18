#ifndef HKBSTRINGCONDITION_H
#define HKBSTRINGCONDITION_H

#include "src/hkxclasses/hkxobject.h"

class hkbStringCondition: public HkxObject
{
    friend class TransitionsUI;
public:
    hkbStringCondition(BehaviorFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkbStringCondition();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaluateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    QString getExpression() const;
    bool write(HkxXMLWriter *writer);
protected:
private:
    static uint refCount;
    static QString classname;
    QString conditionString;
};

#endif // HKBSTRINGCONDITION_H
