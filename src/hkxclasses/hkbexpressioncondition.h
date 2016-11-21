#ifndef HKBEXPRESSIONCONDITION_H
#define HKBEXPRESSIONCONDITION_H

#include "hkxobject.h"

class hkbExpressionCondition: public HkxObject
{
    friend class TransitionUI;
public:
    hkbExpressionCondition(BehaviorFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkbExpressionCondition();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaulateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    QString getExpression() const;
    bool write(HkxXMLWriter *writer);
protected:
private:
    static uint refCount;
    static QString classname;
    QString expression;
};

#endif // HKBEXPRESSIONCONDITION_H
