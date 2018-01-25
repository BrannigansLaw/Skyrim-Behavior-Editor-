#ifndef HKBMIRROREDSKELETONINFO_H
#define HKBMIRROREDSKELETONINFO_H

#include "src/hkxclasses/hkxobject.h"

#include <QVector>

class BehaviorFile;

class hkbMirroredSkeletonInfo: public HkxObject
{
    friend class ProjectFile;
public:
    hkbMirroredSkeletonInfo(HkxFile *parent, long ref = 0);
    virtual ~hkbMirroredSkeletonInfo();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbMirroredSkeletonInfo& operator=(const hkbMirroredSkeletonInfo&);
    hkbMirroredSkeletonInfo(const hkbMirroredSkeletonInfo &);
private:
    static uint refCount;
    static QString classname;
    hkQuadVariable mirrorAxis;
    QVector <int> bonePairMap;
};

#endif // HKBMIRROREDSKELETONINFO_H
