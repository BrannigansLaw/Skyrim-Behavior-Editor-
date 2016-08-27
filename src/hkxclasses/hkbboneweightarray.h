#ifndef HKBBONEWEIGHTARRAY_H
#define HKBBONEWEIGHTARRAY_H

#include <QVector>

#include "hkxobject.h"

class hkbBoneWeightArray: public HkDynamicObject
{
    friend class BehaviorGraphView;
public:
    hkbBoneWeightArray(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBoneWeightArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBoneWeightArray& operator=(const hkbBoneWeightArray&);
    hkbBoneWeightArray(const hkbBoneWeightArray &);
private:
    static uint refCount;
    QVector <qreal> boneWeights;
};

#endif // HKBBONEWEIGHTARRAY_H
