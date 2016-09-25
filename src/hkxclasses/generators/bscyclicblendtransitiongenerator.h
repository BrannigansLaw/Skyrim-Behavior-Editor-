#ifndef BSCYCLICBLENDTRANSITIONGENERATOR_H
#define BSCYCLICBLENDTRANSITIONGENERATOR_H

#include "hkbgenerator.h"

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSCyclicBlendTransitionGenerator(BehaviorFile *parent/*, qint16 ref = 0*/);
    virtual ~BSCyclicBlendTransitionGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    BSCyclicBlendTransitionGenerator& operator=(const BSCyclicBlendTransitionGenerator&);
    BSCyclicBlendTransitionGenerator(const BSCyclicBlendTransitionGenerator &);
private:
    static QStringList BlendCurve;   //BLEND_CURVE_SMOOTH=0;BLEND_CURVE_LINEAR=1;BLEND_CURVE_LINEAR_TO_SMOOTH=2;BLEND_CURVE_SMOOTH_TO_LINEAR=3
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxObjectExpSharedPtr pBlenderGenerator;
    int eventToFreezeBlendValueId;
    HkxObjectExpSharedPtr eventToFreezeBlendValuePayload;
    int eventToCrossBlendId;
    HkxObjectExpSharedPtr eventToCrossBlendPayload;
    qreal fBlendParameter;
    qreal fTransitionDuration;
    QString eBlendCurve;
};

#endif // BSCYCLICBLENDTRANSITIONGENERATOR_H
