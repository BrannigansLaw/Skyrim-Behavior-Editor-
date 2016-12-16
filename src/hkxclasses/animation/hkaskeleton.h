#ifndef HKASKELETON_H
#define HKASKELETON_H

#include "src/hkxclasses/hkxobject.h"

#include <QVector>

class BehaviorFile;

class hkaSkeleton: public HkxObject
{
public:
    hkaSkeleton(HkxFile *parent, long ref = 0);
    virtual ~hkaSkeleton();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    QStringList getBoneNamess() const;
private:
    hkaSkeleton& operator=(const hkaSkeleton&);
    hkaSkeleton(const hkaSkeleton &);
private:
    struct hkBone{
        hkBone():lockTranslation(false){}
        QString name;
        bool lockTranslation;
    };

    struct hkLocalFrame{
        hkLocalFrame():boneIndex(-1){}
        HkxObjectExpSharedPtr localFrame;
        int boneIndex;
    };

    static uint refCount;
    static QString classname;
    QString name;
    QVector <int> parentIndices;
    QList <hkBone> bones;
    QString referencePose;
    QVector <qreal> referenceFloats;
    QStringList floatSlots;
    QList <hkLocalFrame> localFrames;
};

#endif // HKASKELETON_H
