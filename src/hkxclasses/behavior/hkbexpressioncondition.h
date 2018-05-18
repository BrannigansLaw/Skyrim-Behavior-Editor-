#ifndef HKBEXPRESSIONCONDITION_H
#define HKBEXPRESSIONCONDITION_H

#include "src/hkxclasses/hkxobject.h"

class hkbExpressionCondition: public HkxObject
{
    friend class TransitionUI;
public:
    hkbExpressionCondition(BehaviorFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkbExpressionCondition();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaluateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    QString getExpression() const;
    bool write(HkxXMLWriter *writer);
    bool operator ==(const hkbExpressionCondition & other){
        if (expression != other.expression){
            return false;
        }
        return true;
    }
protected:
private:
    static uint refCount;
    static QString classname;
    QString expression;
};

#endif // HKBEXPRESSIONCONDITION_H
