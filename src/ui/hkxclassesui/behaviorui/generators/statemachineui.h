#ifndef STATEMACHINEUI_H
#define STATEMACHINEUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class TableWidget;
class QTableWidgetItem;
class SpinBox;
class QLineEdit;
class ComboBox;
class QPushButton;
class QHBoxLayout;
class QSignalMapper;
class hkbStateMachine;
class QVBoxLayout;
class QStackedLayout;
class QCheckBox;
class StateUI;
class TransitionsUI;
class QScrollArea;
class HkTransition;
class hkbStateMachineStateInfo;

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
    void viewState(hkbStateMachineStateInfo *state);
    void viewTransition(hkbStateMachine *parent, HkTransition *transition);
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
    void stateSelected(int row, int column);
    void transitionSelected(int row, int column);
private:
    void addEventToLists(const QString & name);
    void removeEventFromLists(int index);
    void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void loadComboBoxes();
private:
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
    TableWidget *table;
    QPushButton *addStatePB;
    QPushButton *removeStatePB;
    ComboBox *typeSelectorCB;
    QSignalMapper *stateSignalMapper;
    QHBoxLayout *stateButtonLyt;
    QPushButton *addTransitionPB;
    QPushButton *removeTransitionPB;
    ComboBox *typeTransitionCB;
    QSignalMapper *transitionSignalMapper;
    QHBoxLayout *transitionButtonLyt;
    QVBoxLayout *lyt;
    QLineEdit *name;
    ComboBox *id;
    QLineEdit *payload;
    ComboBox *startStateId;
    ComboBox *startStateIdBind;
    ComboBox *returnToPreviousStateEventId;
    ComboBox *randomTransitionEventId;
    ComboBox *transitionToNextHigherStateEventId;
    ComboBox *transitionToNextLowerStateEventId;
    ComboBox *syncVariableIndex;
    QCheckBox *wrapAroundStateId;
    SpinBox *maxSimultaneousTransitions;
    ComboBox *startStateMode;
    ComboBox *selfTransitionMode;
    TableWidget *states;
    TableWidget *wildcardTransitions;
};

#endif // STATEMACHINEUI_H
