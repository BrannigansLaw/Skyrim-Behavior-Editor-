#ifndef HKBBLENDINGTRANSITIONEFFECT_H
#define HKBBLENDINGTRANSITIONEFFECT_H

#include "src/hkxclasses/hkxobject.h"

class hkbBlendingTransitionEffect: public HkDynamicObject
{
    friend class BehaviorGraphView;
    friend class BlendingTransitionEffectUI;
public:
    hkbBlendingTransitionEffect(HkxFile *parent, long ref = -1);
    virtual ~hkbBlendingTransitionEffect();
    bool link();
    bool readData(const HkxXmlReader & reader, long index);
    void unlink();
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    QString getName() const;
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool operator ==(const hkbBlendingTransitionEffect & other){//TO DO: check bound variable...
        if (name == other.name){
            return true;
        }
        if (
                userData != other.userData ||
                //name != other.name ||
                selfTransitionMode != other.selfTransitionMode ||
                eventMode != other.eventMode ||
                duration != other.duration ||
                toGeneratorStartTimeFraction != other.toGeneratorStartTimeFraction ||
                flags != other.flags ||
                endMode != other.endMode  ||
                blendCurve != other.blendCurve
                )
        {
            return false;
        }
        return true;
    }
private:
    hkbBlendingTransitionEffect& operator=(const hkbBlendingTransitionEffect&);
    hkbBlendingTransitionEffect(const hkbBlendingTransitionEffect &);
private:
    static QStringList SelfTransitionMode;  //SelfTransitionMode (SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC=0;SELF_TRANSITION_MODE_CONTINUE=1;SELF_TRANSITION_MODE_RESET=2;SELF_TRANSITION_MODE_BLEND=3)
    static QStringList EventMode;   //EventMode (EVENT_MODE_DEFAULT=0;EVENT_MODE_PROCESS_ALL=1;EVENT_MODE_IGNORE_FROM_GENERATOR=2;EVENT_MODE_IGNORE_TO_GENERATOR=3)
    static QStringList Flags;   //FlagBits (FLAG_NONE=0;FLAG_IGNORE_FROM_WORLD_FROM_MODEL=1;FLAG_SYNC=2;FLAG_IGNORE_TO_WORLD_FROM_MODEL=4)
    static QStringList EndMode; //EndMode (END_MODE_NONE=0;END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR=1;END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR=2)
    static QStringList BlendCurve; //BlendCurve (BLEND_CURVE_SMOOTH=0;BLEND_CURVE_LINEAR=1;BLEND_CURVE_LINEAR_TO_SMOOTH=2;BLEND_CURVE_SMOOTH_TO_LINEAR=3)
    static QString classname;
    static uint refCount;
    ulong userData;
    QString name;
    QString selfTransitionMode;
    QString eventMode;
    qreal duration;
    qreal toGeneratorStartTimeFraction;
    QString flags;
    QString endMode;
    QString blendCurve;
};

#endif // HKBBLENDINGTRANSITIONEFFECT_H
