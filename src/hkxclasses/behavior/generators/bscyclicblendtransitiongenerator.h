#ifndef BSCYCLICBLENDTRANSITIONGENERATOR_H
#define BSCYCLICBLENDTRANSITIONGENERATOR_H

#include "hkbgenerator.h"

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BSCyclicBlendTransitionGeneratorUI;
public:
    BSCyclicBlendTransitionGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSCyclicBlendTransitionGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSCyclicBlendTransitionGenerator& operator=(const BSCyclicBlendTransitionGenerator&);
    BSCyclicBlendTransitionGenerator(const BSCyclicBlendTransitionGenerator &);
private:
    static QStringList BlendCurve;   //BLEND_CURVE_SMOOTH=0;BLEND_CURVE_LINEAR=1;BLEND_CURVE_LINEAR_TO_SMOOTH=2;BLEND_CURVE_SMOOTH_TO_LINEAR=3
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxSharedPtr pBlenderGenerator;
    hkEventPayload eventToFreezeBlendValue;
    hkEventPayload eventToCrossBlend;
    qreal fBlendParameter;
    qreal fTransitionDuration;
    QString eBlendCurve;
};

#endif // BSCYCLICBLENDTRANSITIONGENERATOR_H
