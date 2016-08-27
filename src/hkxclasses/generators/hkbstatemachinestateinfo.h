#ifndef HKBSTATEMACHINESTATEINFO_H
#define HKBSTATEMACHINESTATEINFO_H

#include "hkbgenerator.h"

class hkbStateMachineStateInfo: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbStateMachineStateInfo(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbStateMachineStateInfo();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
private:
    hkbStateMachineStateInfo& operator=(const hkbStateMachineStateInfo&);
    hkbStateMachineStateInfo(const hkbStateMachineStateInfo &);
private:
    static uint refCount;
    HkxObjectExpSharedPtr enterNotifyEvents;
    HkxObjectExpSharedPtr exitNotifyEvents;
    HkxObjectExpSharedPtr transitions;
    HkxObjectExpSharedPtr generator;
    QString name;
    ushort stateId;
    qreal probability;
    bool enable;
};

#endif // HKBSTATEMACHINESTATEINFO_H
