#ifndef BSDIRECTATMODIFIER_H
#define BSDIRECTATMODIFIER_H

#include "hkbmodifier.h"

class BSDirectAtModifier final: public hkbModifier
{
    friend class BSDirectAtModifierUI;
public:
    BSDirectAtModifier(HkxFile *parent, long ref = 0);
    BSDirectAtModifier& operator=(const BSDirectAtModifier&) = delete;
    BSDirectAtModifier(const BSDirectAtModifier &) = delete;
    ~BSDirectAtModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
private:
    static uint refCount;
    static const QString classname;
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
    mutable std::mutex mutex;
};

#endif // BSDIRECTATMODIFIER_H
