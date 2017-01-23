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
public:
    hkbBlenderGeneratorChild(HkxFile *parent, hkbGenerator *parentBG, long ref = 0);
    virtual ~hkbBlenderGeneratorChild();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbGenerator *getParentGenerator() const;
    hkbBlenderGeneratorChild& operator=(const hkbBlenderGeneratorChild&);
    hkbBlenderGeneratorChild(const hkbBlenderGeneratorChild &);
private:
    HkxObjectExpSharedPtr parentBG;
    static uint refCount;
    static QString classname;
    HkxObjectExpSharedPtr generator;
    HkxObjectExpSharedPtr boneWeights;
    qreal weight;
    qreal worldFromModelWeight;
};

#endif // HKBBLENDERGENERATORCHILD_H
