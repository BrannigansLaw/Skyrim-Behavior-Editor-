#ifndef HKBSTATEMACHINE_H
#define HKBSTATEMACHINE_H

#include "hkbgenerator.h"

class hkbStateMachine: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class StateMachineUI;
    friend class StateUI;
    friend class hkbStateMachineStateInfo;
    friend class BehaviorFile;
public:
    hkbStateMachine(HkxFile *parent, long ref = 0);
    virtual ~hkbStateMachine();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    QString getStateName(int stateId) const;
    int getStateId(const QString & statename) const;
    int getNestedStateId(const QString & statename, int stateId) const;
    QStringList getStateNames() const;
    int getNumberOfStates() const;
    QString getNestedStateName(int stateId, int nestedStateId) const;
    QStringList getNestedStateNames(int stateId) const;
    int getNumberOfNestedStates(int stateId) const;
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    int generateValidStateId();
    bool hasChildren() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    bool merge(HkxObject *recessiveObject);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbStateMachine * getNestedStateMachine(int stateId) const;
    hkbStateMachine& operator=(const hkbStateMachine&);
    hkbStateMachine(const hkbStateMachine &);
private:
    static QStringList StartStateMode;  //{START_STATE_MODE_DEFAULT=0, START_STATE_MODE_SYNC=1, START_STATE_MODE_RANDOM=2, START_STATE_MODE_CHOOSER=3};
    static QStringList SelfTransitionMode;  //{SELF_TRANSITION_MODE_NO_TRANSITION=0, SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE=1, SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE=2};
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    //startStateChooser
    hkEventPayload eventToSendWhenStateOrTransitionChanges;
    int startStateId;
    int returnToPreviousStateEventId;
    int randomTransitionEventId;
    int transitionToNextHigherStateEventId;
    int transitionToNextLowerStateEventId;
    int syncVariableIndex;
    bool wrapAroundStateId;
    ushort maxSimultaneousTransitions;  //Max 32, min 0.
    QString startStateMode;
    QString selfTransitionMode;
    QList <HkxSharedPtr> states;
    HkxSharedPtr wildcardTransitions;
};

#endif // HKBSTATEMACHINE_H
