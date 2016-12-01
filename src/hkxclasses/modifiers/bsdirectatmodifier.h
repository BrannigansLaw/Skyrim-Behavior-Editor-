#ifndef BSDIRECTATMODIFIER_H
#define BSDIRECTATMODIFIER_H

#include "hkbmodifier.h"

class BSDirectAtModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSDirectAtModifier(BehaviorFile *parent, long ref = 0);
    virtual ~BSDirectAtModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSDirectAtModifier& operator=(const BSDirectAtModifier&);
    BSDirectAtModifier(const BSDirectAtModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    bool directAtTarget;
    int sourceBoneIndex;
    int startBoneIndex;
    int endBoneIndex;
    qreal limitHeadingDegrees;
    qreal limitPitchDegrees;
    qreal offsetHeadingDegrees;
    qreal offsetPitchDegrees;
    qreal onGain;
    qreal offGain;
    hkQuadVariable targetLocation;
    int userInfo;
    bool directAtCamera;
    qreal directAtCameraX;
    qreal directAtCameraY;
    qreal directAtCameraZ;
    bool active;
    qreal currentHeadingOffset;
    qreal currentPitchOffset;
};

#endif // BSDIRECTATMODIFIER_H
