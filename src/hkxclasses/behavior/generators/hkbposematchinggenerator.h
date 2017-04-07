#ifndef HKBPOSEMATCHINGGENERATOR_H
#define HKBPOSEMATCHINGGENERATOR_H

#include "hkbgenerator.h"

class hkbPoseMatchingGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbPoseMatchingGenerator(HkxFile *parent, long ref = 0);
    virtual ~hkbPoseMatchingGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getNumberOfChildren() const;
    int getIndexToInsertIcon(HkxObject *child) const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbPoseMatchingGenerator& operator=(const hkbPoseMatchingGenerator&);
    hkbPoseMatchingGenerator(const hkbPoseMatchingGenerator &);
private:
    static QStringList Flags;   //FLAG_SYNC=1;FLAG_SMOOTH_GENERATOR_WEIGHTS=4;FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS=8;FLAG_PARAMETRIC_BLEND=16;FLAG_IS_PARAMETRIC_BLEND_CYCLIC=32;FLAG_FORCE_DENSE_POSE=64
    static QStringList Mode;    //MODE_MATCH=0;MODE_PLAY=1
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    qreal referencePoseWeightThreshold;
    qreal blendParameter;
    qreal minCyclicBlendParameter;
    qreal maxCyclicBlendParameter;
    int indexOfSyncMasterChild;
    QString flags;
    bool subtractLastChild;
    QList <HkxObjectExpSharedPtr> children;
    hkQuadVariable worldFromModelRotation;
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
