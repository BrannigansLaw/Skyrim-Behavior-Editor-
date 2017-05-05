#ifndef HKBEVALUATEEXPRESSIONMODIFIER_H
#define HKBEVALUATEEXPRESSIONMODIFIER_H

#include "hkbmodifier.h"

class hkbEvaluateExpressionModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbEvaluateExpressionModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbEvaluateExpressionModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbEvaluateExpressionModifier& operator=(const hkbEvaluateExpressionModifier&);
    hkbEvaluateExpressionModifier(const hkbEvaluateExpressionModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr expressions;
};

#endif // HKBEVALUATEEXPRESSIONMODIFIER_H
