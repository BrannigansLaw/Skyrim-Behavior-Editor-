#ifndef BSCYCLICBLENDTRANSITIONGENERATOR_H
#define BSCYCLICBLENDTRANSITIONGENERATOR_H

#include "hkbgenerator.h"

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSCyclicBlendTransitionGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSCyclicBlendTransitionGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
private:
    BSCyclicBlendTransitionGenerator& operator=(const BSCyclicBlendTransitionGenerator&);
    BSCyclicBlendTransitionGenerator(const BSCyclicBlendTransitionGenerator &);
private:
    static QStringList BlendCurve;   //BLEND_CURVE_SMOOTH
    static uint refCount;
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
