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
class QStackedLayout;
class QCheckBox;
class StateUI;
class TransitionsUI;

class StateMachineUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    StateMachineUI();
    virtual ~StateMachineUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEventToSendWhenStateOrTransitionChanges(int index);
    void setPayload();
    void setStartStateId(int index);
    void setStartStateIdBind(int index);
    void setReturnToPreviousStateEventId(int index);
    void setRandomTransitionEventId(int index);
    void setTransitionToNextHigherStateEventId(int index);
    void setTransitionToNextLowerStateEventId(int index);
    void setSyncVariableIndex(int index);
    void setWrapAroundStateId(bool checked);
    void setMaxSimultaneousTransitions();
    void setStartStateMode(int index);
    void setSelfTransitionMode(int index);
    void addNewStateWithGenerator();
    void removeStateWithGenerator();
    void addNewTransition();
    void removeTransition();
    void viewState(int row, int column);
    void viewStateTable();
    void viewTransition(int row, int column);
    void viewTransitionTable();
private:
    void addEventToLists(const QString & name);
    void removeEventFromLists(int index);
    void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
private:
    enum StateView {
        STATE_TABLE = 0,
        STATE_WIDGET = 1
    };
    enum TransitionView {
        TRANSITION_TABLE = 0,
        TRANSITION_WIDGET = 1
    };
    enum Type {
        STATE_MACHINE = 0,
        MANUAL_SELECTOR_GENERATOR = 1,
        BLENDER_GENERATOR = 2,
        I_STATE_TAGGING_GENERATOR = 3,
        BONE_SWITCH_GENERATOR = 4,
        CYCLIC_BLEND_TRANSITION_GENERATOR = 5,
        SYNCHRONIZED_CLIP_GENERATOR = 6,
        MODIFIER_GENERATOR = 7,
        OFFSET_ANIMATION_GENERATOR = 8,
        POSE_MATCHING_GENERATOR = 9,
        CLIP_GENERATOR = 10,
        BEHAVIOR_REFERENCE_GENERATOR = 11
    };
    static QStringList types;
    static QStringList headerLabels1;
    static QStringList headerLabels2;
    BehaviorGraphView *behaviorView;
    hkbStateMachine *bsData;
    QTableWidget *table;
    QPushButton *addStatePB;
    QPushButton *removeStatePB;
    QComboBox *typeSelectorCB;
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
    QCheckBox *wrapAroundStateId;
    QSpinBox *maxSimultaneousTransitions;
    QComboBox *startStateMode;
    QComboBox *selfTransitionMode;
    QTableWidget *states;
    QTableWidget *wildcardTransitions;
    StateUI *stateWidget;
    TransitionsUI *transitionWidget;
    QStackedLayout *stateStackLyt;
    QStackedLayout *transitionStackLyt;
};

#endif // STATEMACHINEUI_H
