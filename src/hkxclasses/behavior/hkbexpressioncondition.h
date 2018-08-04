#ifndef HKBEXPRESSIONCONDITION_H
#define HKBEXPRESSIONCONDITION_H

#include "src/hkxclasses/hkxobject.h"

class hkbExpressionCondition final: public HkxObject
{
public:
    hkbExpressionCondition(BehaviorFile *parent, const QString & string = "", long ref = -1);
    hkbExpressionCondition& operator=(const hkbExpressionCondition&) = delete;
    hkbExpressionCondition(const hkbExpressionCondition &) = delete;
    ~hkbExpressionCondition();
    static const QString getClassname();
    QString getExpression() const;
    void setExpression(const QString &value);
    bool operator ==(const hkbExpressionCondition & other){
        if (expression != other.expression){
            return false;
        }
        return true;
    }
private:
    bool readData(const HkxXmlReader & reader, long & index);
    QString evaluateDataValidity();
    bool link(){return true;}
    bool write(HkxXMLWriter *writer);
private:
    static uint refCount;
    static const QString classname;
    QString expression;
    mutable std::mutex mutex;
};

#endif // HKBEXPRESSIONCONDITION_H
