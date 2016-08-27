#ifndef HKBPOSEMATCHINGGENERATOR_H
#define HKBPOSEMATCHINGGENERATOR_H

#include "hkbgenerator.h"

class hkbPoseMatchingGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbPoseMatchingGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbPoseMatchingGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
private:
    hkbPoseMatchingGenerator& operator=(const hkbPoseMatchingGenerator&);
    hkbPoseMatchingGenerator(const hkbPoseMatchingGenerator &);
private:
    static QStringList Flags;
    static QStringList Mode;
    static uint refCount;
    ulong userData;
    QString name;
    qreal referencePoseWeightThreshold;
    qreal blendParameter;
    qreal minCyclicBlendParameter;
    qreal maxCyclicBlendParameter;
    qint16 indexOfSyncMasterChild;
    QString flags;
    bool subtractLastChild;
    QList <HkxObjectExpSharedPtr> children;
    hkVector4 worldFromModelRotation;
    qreal blendSpeed;
    qreal minSpeedToSwitch;
    qreal minSwitchTimeNoError;
    qreal minSwitchTimeFullError;
    int startPlayingEventId;
    int startMatchingEventId;
    int rootBoneIndex;
    int otherBoneIndex;
    int anotherBoneIndex;
    int pelvisIndex;
    QString mode;
};

#endif // HKBPOSEMATCHINGGENERATOR_H
