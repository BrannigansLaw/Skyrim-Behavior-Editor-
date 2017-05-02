#ifndef STATEMACHINEUI_H
#define STATEMACHINEUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class TableWidget;
class QGridLayout;
class SpinBox;
class LineEdit;
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
class QGroupBox;
class HkTransition;
class hkbStateMachineStateInfo;
class GenericTableWidget;

class StateMachineUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    StateMachineUI();
    virtual ~StateMachineUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index);
    void viewEvents(int index);
    void viewGenerators(int index);
private slots:
    void setBindingVariable(int index, const QString & name);
    void setName();
    void setEventToSendWhenStateOrTransitionChanges(int index);
    void setPayload();
    void setStartStateId(int index);
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
    void removeObjectChild();
    void addNewTransition();
    void viewSelectedChild(int row, int column);
    void returnToWidget();
private:
    void connectChildUI(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators);
    void renameVariable(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type);
    void moveRowWidgets(int targetRow);
    int moveRowItems(int oldRow, int targetRow);
    void generatorRenamed(const QString & name, int index);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        STATE_WIDGET = 1,
        TRANSITION_WIDGET = 2
    };
    enum Generator_Type {
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
    static QStringList headerLabels;
    int rowIndexOfTransitionButtonPanel;
    int rowIndexOfChildToRemove;
    BehaviorGraphView *behaviorView;
    hkbStateMachine *bsData;
    QGroupBox *groupBox;
    StateUI *stateUI;
    TransitionsUI *transitionUI;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *addStatePB;
    QPushButton *removeStatePB;
    ComboBox *typeSelectorCB;
    //QPushButton *addTransitionPB;
    //QPushButton *removeTransitionPB;
    LineEdit *name;
    LineEdit *payload;
    ComboBox *startStateId;
    QCheckBox *wrapAroundStateId;
    SpinBox *maxSimultaneousTransitions;
    ComboBox *startStateMode;
    ComboBox *selfTransitionMode;
};

#endif // STATEMACHINEUI_H
