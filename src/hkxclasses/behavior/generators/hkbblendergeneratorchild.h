#ifndef HKBBLENDERGENERATORCHILD_H
#define HKBBLENDERGENERATORCHILD_H

#include "hkbgenerator.h"

class hkbBlenderGenerator;

class hkbBlenderGeneratorChild: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class hkbBlenderGenerator;
    friend class hkbPoseMatchingGenerator;
    friend class BlenderGeneratorUI;
    friend class BlenderGeneratorChildUI;
    friend class PoseMatchingGeneratorUI;
public:
    hkbBlenderGeneratorChild(HkxFile *parent, hkbGenerator *parentBG, long ref = 0);
    virtual ~hkbBlenderGeneratorChild();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isParametricBlend() const;
    bool hasChildren() const;
    QString getName() const;
    int getThisIndex() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbGenerator *getParentGenerator() const;
    hkbBlenderGeneratorChild& operator=(const hkbBlenderGeneratorChild&);
    hkbBlenderGeneratorChild(const hkbBlenderGeneratorChild &);
private:
    static uint refCount;
    static QString classname;
    HkxSharedPtr parentBG;
    HkxSharedPtr generator;
    HkxSharedPtr boneWeights;
    qreal weight;
    qreal worldFromModelWeight;
};

#endif // HKBBLENDERGENERATORCHILD_H
