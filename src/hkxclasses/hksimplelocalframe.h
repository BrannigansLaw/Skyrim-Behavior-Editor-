#ifndef HKSIMPLELOCALFRAME_H
#define HKSIMPLELOCALFRAME_H

#include "src/hkxclasses/hkxobject.h"

class hkSimpleLocalFrame: public HkxObject
{
    friend class hkaSkeleton;
    friend class SkeletonFile;
public:
    hkSimpleLocalFrame(HkxFile *parent, const QString & string = "", long ref = -1);
    virtual ~hkSimpleLocalFrame();
    bool readData(const HkxXmlReader & reader, long index);
    bool evaluateDataValidity();
    static QString getClassname();
    bool link(){return true;}
    bool write(HkxXMLWriter *writer);
    QString getName() const;
protected:
private:
    static uint refCount;
    static QString classname;
    //hkTransform transform;
    //QList <hkLocalFrame> children;
    //hkLocalFrame parentFrame;
    //hkLocalFrameGroup group;
    QString name;
};

#endif // HKSIMPLELOCALFRAME_H
