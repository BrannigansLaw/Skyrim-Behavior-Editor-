#ifndef BSCYCLICBLENDTRANSITIONGENERATOR_H
#define BSCYCLICBLENDTRANSITIONGENERATOR_H

#include "hkbgenerator.h"

class BSCyclicBlendTransitionGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSCyclicBlendTransitionGenerator(BehaviorFile *parent, long ref = 0);
    virtual ~BSCyclicBlendTransitionGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(hkbGenerator *objToAppend);
    bool removeObject(hkbGenerator *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder);
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
