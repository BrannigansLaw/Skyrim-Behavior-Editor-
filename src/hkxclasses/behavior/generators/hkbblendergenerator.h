#ifndef HKBBLENDERGENERATOR_H
#define HKBBLENDERGENERATOR_H

#include "hkbgenerator.h"

class hkbBlenderGeneratorChild;

class hkbBlenderGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BlenderGeneratorUI;
    friend class BlenderGeneratorChildUI;
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
    int getIndexOfChild(hkbBlenderGeneratorChild *child) const;
    bool isParametricBlend() const;
    enum BlenderFlag{
        FLAG_NONE = 0,
        FLAG_SYNC = 1,
        FLAG_SMOOTH_GENERATOR_WEIGHTS = 4,
        FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS = 8,
        FLAG_PARAMETRIC_BLEND = 16,
        FLAG_IS_PARAMETRIC_BLEND_CYCLIC = 32,
        FLAG_FORCE_DENSE_POSE = 64,
        INVALID_FLAG = 128
    };
    Q_DECLARE_FLAGS(BlenderFlags, BlenderFlag)
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbBlenderGenerator& operator=(const hkbBlenderGenerator&);
    hkbBlenderGenerator(const hkbBlenderGenerator &);
    //int generatorCount(hkbGenerator *gen);
private:
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
    QList <HkxSharedPtr> children;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(hkbBlenderGenerator::BlenderFlags)

#endif // HKBBLENDERGENERATOR_H
