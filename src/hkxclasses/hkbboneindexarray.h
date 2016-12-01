#ifndef HKBBONEINDEXARRAY_H
#define HKBBONEINDEXARRAY_H

#include <QVector>

#include "hkxobject.h"

class hkbBoneIndexArray: public HkDynamicObject
{
    friend class BehaviorGraphView;
public:
    hkbBoneIndexArray(BehaviorFile *parent, long ref = 0);
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
    QVector <qint16> boneIndices;
};

#endif // HKBBONEINDEXARRAY_H
