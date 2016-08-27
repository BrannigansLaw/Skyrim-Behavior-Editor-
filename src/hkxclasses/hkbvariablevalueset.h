#ifndef HKBVARIABLEVALUESET_H
#define HKBVARIABLEVALUESET_H

#include <QVector>

#include "hkxobject.h"

class hkbVariableValueSet: public HkxObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
public:
    hkbVariableValueSet(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbVariableValueSet();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbVariableValueSet& operator=(const hkbVariableValueSet&);
    hkbVariableValueSet(const hkbVariableValueSet &);
private:
    static uint refCount;
    QVector <int> wordVariableValues;
    QVector <hkVector4> quadVariableValues;
    QList <HkxObjectExpSharedPtr> variantVariableValues;
};

#endif // HKBVARIABLEVALUESET_H
