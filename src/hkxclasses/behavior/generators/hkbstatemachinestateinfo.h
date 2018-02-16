#ifndef HKBSTATEMACHINESTATEINFO_H
#define HKBSTATEMACHINESTATEINFO_H

#include "hkbgenerator.h"

class hkbStateMachine;

class hkbStateMachineStateInfo: public hkbGenerator
{
    friend class HkDataUI;
    friend class BehaviorGraphView;
    friend class hkbStateMachine;
    friend class hkbStateMachineTransitionInfoArray;
    friend class StateMachineUI;
    friend class StateUI;
public:
    hkbStateMachineStateInfo(HkxFile *parent, hkbStateMachine *parentsm, long ref = 0);
    virtual ~hkbStateMachineStateInfo();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    bool setStateId(ushort id);
    hkbStateMachine * getParentStateMachine() const;
    hkbStateMachineStateInfo& operator=(const hkbStateMachineStateInfo&);
    hkbStateMachineStateInfo(const hkbStateMachineStateInfo &);
private:
    hkbStateMachine *parentSM;
    static uint refCount;
    static QString classname;
    HkxSharedPtr enterNotifyEvents;
    HkxSharedPtr exitNotifyEvents;
    HkxSharedPtr transitions;
    HkxSharedPtr generator;
    QString name;
    ushort stateId;
    qreal probability;
    bool enable;
};

#endif // HKBSTATEMACHINESTATEINFO_H
