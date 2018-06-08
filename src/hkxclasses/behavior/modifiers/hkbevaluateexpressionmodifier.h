#ifndef HKBEVALUATEEXPRESSIONMODIFIER_H
#define HKBEVALUATEEXPRESSIONMODIFIER_H

#include "hkbmodifier.h"

class hkbEvaluateExpressionModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class EvaluateExpressionModifierUI;
public:
    hkbEvaluateExpressionModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbEvaluateExpressionModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfExpressions() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    bool merge(HkxObject *recessiveObject);
    void updateReferences(long &ref);
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
