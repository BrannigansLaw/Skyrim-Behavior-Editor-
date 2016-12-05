#ifndef HKBBLENDERGENERATOR_H
#define HKBBLENDERGENERATOR_H

#include "hkbgenerator.h"

class hkbBlenderGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBlenderGenerator(HkxFile *parent, long ref = 0);
    virtual ~hkbBlenderGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getNumberOfChildren() const;
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder = true);
    hkbBlenderGenerator& operator=(const hkbBlenderGenerator&);
    hkbBlenderGenerator(const hkbBlenderGenerator &);
private:
    static QStringList Flags;   //{FLAG_SYNC=1, FLAG_SMOOTH_GENERATOR_WEIGHTS=4, FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS=8, FLAG_PARAMETRIC_BLEND=16, FLAG_IS_PARAMETRIC_BLEND_CYCLIC=32, FLAG_FORCE_DENSE_POSE=64};
    static uint refCount;
    static QString classname;
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
};

#endif // HKBBLENDERGENERATOR_H
