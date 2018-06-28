#ifndef HKBGENERATORTRANSITIONEFFECT_H
#define HKBGENERATORTRANSITIONEFFECT_H

#include "src/hkxclasses/hkxobject.h"

class hkbGeneratorTransitionEffect: public HkDynamicObject
{
    friend class BehaviorGraphView;
public:
    hkbGeneratorTransitionEffect(HkxFile *parent, long ref = -1);
    virtual ~hkbGeneratorTransitionEffect();
    bool link();
    bool readData(const HkxXmlReader & reader, long index);
    void unlink();
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    QString getName() const;
private:
    hkbGeneratorTransitionEffect& operator=(const hkbGeneratorTransitionEffect&);
    hkbGeneratorTransitionEffect(const hkbGeneratorTransitionEffect &);
private:
    static QStringList SelfTransitionMode;  //SelfTransitionMode (SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC=0;SELF_TRANSITION_MODE_CONTINUE=1;SELF_TRANSITION_MODE_RESET=2;SELF_TRANSITION_MODE_BLEND=3)
    static QStringList EventMode;   //EventMode (EVENT_MODE_DEFAULT=0;EVENT_MODE_PROCESS_ALL=1;EVENT_MODE_IGNORE_FROM_GENERATOR=2;EVENT_MODE_IGNORE_TO_GENERATOR=3)
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    QString selfTransitionMode;
    QString eventMode;
    HkxSharedPtr transitionGenerator;  //This may preserve generators unexpectedly...
    qreal blendInDuration;
    qreal blendOutDuration;
    bool syncToGeneratorStartTime;
};

#endif // HKBGENERATORTRANSITIONEFFECT_H
