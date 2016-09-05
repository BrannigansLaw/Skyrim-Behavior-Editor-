#ifndef HKBBLENDERGENERATORCHILD_H
#define HKBBLENDERGENERATORCHILD_H

#include "hkbgenerator.h"

class hkbBlenderGeneratorChild: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBlenderGeneratorChild(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBlenderGeneratorChild();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
private:
    hkbBlenderGeneratorChild& operator=(const hkbBlenderGeneratorChild&);
    hkbBlenderGeneratorChild(const hkbBlenderGeneratorChild &);
private:
    static uint refCount;
    static QString classname;
    HkxObjectExpSharedPtr generator;
    HkxObjectExpSharedPtr boneWeights;
    qreal weight;
    qreal worldFromModelWeight;
};

#endif // HKBBLENDERGENERATORCHILD_H
