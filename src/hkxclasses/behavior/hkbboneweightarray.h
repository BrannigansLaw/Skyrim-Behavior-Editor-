#ifndef HKBBONEWEIGHTARRAY_H
#define HKBBONEWEIGHTARRAY_H

#include <QVector>

#include "src/hkxclasses/hkxobject.h"

class hkbBoneWeightArray: public HkDynamicObject
{
    friend class BehaviorGraphView;
    friend class BoneWeightArrayUI;
public:
    hkbBoneWeightArray(HkxFile *parent, int size = 0, long ref = 0);
    virtual ~hkbBoneWeightArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    //bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbBoneWeightArray& operator=(const hkbBoneWeightArray&);
    hkbBoneWeightArray(const hkbBoneWeightArray &);
private:
    static uint refCount;
    static QString classname;
    QVector <qreal> boneWeights;
};

#endif // HKBBONEWEIGHTARRAY_H
