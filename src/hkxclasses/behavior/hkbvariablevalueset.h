#ifndef HKBVARIABLEVALUESET_H
#define HKBVARIABLEVALUESET_H

#include <QVector>

#include "src/hkxclasses/hkxobject.h"

class hkbVariableValueSet: public HkxObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
    friend class hkbCharacterData;
    friend class CharacterPropertiesUI;
    friend class BehaviorFile;
    friend class CharacterFile;
public:
    hkbVariableValueSet(HkxFile *parent, long ref = 0);
    virtual ~hkbVariableValueSet();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool merge(HkxObject *recessiveobj);
private:
    hkbVariableValueSet& operator=(const hkbVariableValueSet&);
    hkbVariableValueSet(const hkbVariableValueSet &);
private:
    static uint refCount;
    static QString classname;
    QVector <int> wordVariableValues;
    QVector <hkQuadVariable> quadVariableValues;
    QList <HkxSharedPtr> variantVariableValues;
};

#endif // HKBVARIABLEVALUESET_H
