#ifndef HKBVARIABLEVALUESET_H
#define HKBVARIABLEVALUESET_H

#include <QVector>

#include "hkxobject.h"

class hkbVariableValueSet: public HkxObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
public:
    hkbVariableValueSet(BehaviorFile *parent, long ref = 0);
    virtual ~hkbVariableValueSet();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    //hkQuadVariable getQuadVariable(int index);
private:
    hkbVariableValueSet& operator=(const hkbVariableValueSet&);
    hkbVariableValueSet(const hkbVariableValueSet &);
private:
    static uint refCount;
    static QString classname;
    QVector <int> wordVariableValues;
    QVector <hkQuadVariable> quadVariableValues;
    QList <HkxObjectExpSharedPtr> variantVariableValues;
};

#endif // HKBVARIABLEVALUESET_H
