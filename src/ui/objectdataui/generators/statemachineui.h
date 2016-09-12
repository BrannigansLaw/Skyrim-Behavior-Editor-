#ifndef STATEMACHINEUI_H
#define STATEMACHINEUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class QTableWidget;
class QTableWidgetItem;
class QSpinBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class QSignalMapper;
class hkbStateMachine;
class QVBoxLayout;

class StateMachineUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    StateMachineUI();
    virtual ~StateMachineUI(){}
    void loadData(HkxObject *data);
private slots:
    void setName();
    void setEventToSendWhenStateOrTransitionChanges();
    void setPayload();
    void setReturnToPreviousStateEventId();
    void setRandomTransitionEventId();
    void setTransitionToNextHigherStateEventId();
    void setTransitionToNextLowerStateEventId();
    void setSyncVariableIndex();
    void setWrapAroundStateId();
    void setMaxSimultaneousTransitions();
    void setStartStateMode();
    void setSelfTransitionMode();
    void addNewState();
    void removeState(int index);
    void addNewTransition();
    void removeTransition(int index);
private:
    static QStringList headerLabels1;
    static QStringList headerLabels2;
    BehaviorGraphView *behaviorView;
    hkbStateMachine *bsData;
    QTableWidget *table;
    QPushButton *addStatePB;
    QPushButton *removeStatePB;
    QComboBox *typeStateCB;
    QSignalMapper *stateSignalMapper;
    QHBoxLayout *stateButtonLyt;
    QPushButton *addTransitionPB;
    QPushButton *removeTransitionPB;
    QComboBox *typeTransitionCB;
    QSignalMapper *transitionSignalMapper;
    QHBoxLayout *transitionButtonLyt;
    QVBoxLayout *lyt;
    QLineEdit *name;
    QComboBox *id;
    QLineEdit *payload;
    QComboBox *startStateId;
    QComboBox *startStateIdBind;
    QComboBox *returnToPreviousStateEventId;
    QComboBox *randomTransitionEventId;
    QComboBox *transitionToNextHigherStateEventId;
    QComboBox *transitionToNextLowerStateEventId;
    QComboBox *syncVariableIndex;
    QSpinBox *maxSimultaneousTransitions;
    QComboBox *startStateMode;
    QComboBox *selfTransitionMode;
    QTableWidget *states;
    QTableWidget *wildcardTransitions;
};

#endif // STATEMACHINEUI_H
