#ifndef HKBBONEINDEXARRAY_H
#define HKBBONEINDEXARRAY_H

#include <QVector>

#include "src/hkxclasses/hkxobject.h"

class hkbBoneIndexArray: public HkDynamicObject
{
    friend class BehaviorGraphView;
    friend class BoneIndexArrayUI;
public:
    hkbBoneIndexArray(HkxFile *parent, long ref = 0);
    virtual ~hkbBoneIndexArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    //bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbBoneIndexArray& operator=(const hkbBoneIndexArray&);
    hkbBoneIndexArray(const hkbBoneIndexArray &);
private:
    static uint refCount;
    static QString classname;
    QVector <int> boneIndices;
};

#endif // HKBBONEINDEXARRAY_H
