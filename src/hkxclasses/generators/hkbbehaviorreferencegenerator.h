#ifndef HKBBEHAVIORREFERENCEGENERATOR_H
#define HKBBEHAVIORREFERENCEGENERATOR_H

#include "hkbgenerator.h"

class hkbBehaviorReferenceGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorReferenceGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorReferenceGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void removeData(){}
    QString getName() const;
private:
    hkbBehaviorReferenceGenerator& operator=(const hkbBehaviorReferenceGenerator&);
    hkbBehaviorReferenceGenerator(const hkbBehaviorReferenceGenerator &);
private:
    static uint refCount;
    ulong userData;
    QString name;
    QString behaviorName;
};

#endif // HKBBEHAVIORREFERENCEGENERATOR_H
