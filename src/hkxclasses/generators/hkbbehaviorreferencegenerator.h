#ifndef HKBBEHAVIORREFERENCEGENERATOR_H
#define HKBBEHAVIORREFERENCEGENERATOR_H

#include "hkbgenerator.h"

class hkbBehaviorReferenceGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorReferenceGenerator(BehaviorFile *parent/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorReferenceGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void removeData(){}
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    hkbBehaviorReferenceGenerator& operator=(const hkbBehaviorReferenceGenerator&);
    hkbBehaviorReferenceGenerator(const hkbBehaviorReferenceGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    QString behaviorName;
};

#endif // HKBBEHAVIORREFERENCEGENERATOR_H
