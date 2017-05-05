#ifndef BSCYCLICBLENDTRANSITIONGENERATOR_H
#define BSCYCLICBLENDTRANSITIONGENERATOR_H

#include "hkbgenerator.h"

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSCyclicBlendTransitionGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSCyclicBlendTransitionGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
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
