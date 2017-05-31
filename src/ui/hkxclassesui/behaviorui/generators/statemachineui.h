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
class hkbStateMachine;
class QCheckBox;
class StateUI;
class TransitionsUI;
class QGroupBox;
class hkbStateMachineStateInfo;
class GenericTableWidget;
class hkbStateMachineTransitionInfoArray;
class EventUI;
class hkbVariableBindingSet;
class CheckButtonCombo;

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
    void viewProperties(int index);
    void viewEvents(int index);
    void viewGenerators(int index);
private slots:
    void setName();
    void setStartStateId(int index);
    void setReturnToPreviousStateEventId(int index, const QString &name);
    void setRandomTransitionEventId(int index, const QString &name);
    void setTransitionToNextHigherStateEventId(int index, const QString &name);
    void setTransitionToNextLowerStateEventId(int index, const QString &name);
    void setSyncVariableIndex(int index);
    void setWrapAroundStateId(bool checked);
    void setMaxSimultaneousTransitions();
    void setStartStateMode(int index);
    void setSelfTransitionMode(int index);
    void viewSelectedChild(int row, int column);
    void eventTableElementSelected(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
    void generatorTableElementSelected(int index, const QString &name);
    void returnToWidget(bool reloadData);
    void returnToWidget();
    void viewEventToSendWhenStateOrTransitionChanges();
    void toggleEventToSendWhenStateOrTransitionChanges(bool enable);
    void stateRenamed(const QString & name, int stateIndex);
    void transitionRenamed(const QString &name, int index);
private:
    void connectSignals();
    void disconnectSignals();
    void addStateWithGenerator();
    void addTransition();
    void loadDynamicTableRows();
    void setBindingVariable(int index, const QString & name);
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString &tip1, const QString &tip2);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void generatorRenamed(const QString & name, int index);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
    void loadTableValue(int row, const QString & value);
    void removeState(int index);
    void removeTransition(int index);
    void selectTableToView(bool properties, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        STATE_WIDGET = 1,
        TRANSITION_WIDGET = 2,
        EVENT_PAYLOAD_WIDGET = 3
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
    int transitionsButtonRow;
    BehaviorGraphView *behaviorView;
    hkbStateMachine *bsData;
    QGroupBox *groupBox;
    EventUI *eventUI;
    StateUI *stateUI;
    TransitionsUI *transitionUI;
    QGridLayout *topLyt;
    TableWidget *table;
    ComboBox *typeSelectorCB;
    LineEdit *name;
    CheckButtonCombo *eventToSendWhenStateOrTransitionChanges;
    ComboBox *startStateId;
    QCheckBox *wrapAroundStateId;
    SpinBox *maxSimultaneousTransitions;
    ComboBox *startStateMode;
    ComboBox *selfTransitionMode;
};

#endif // STATEMACHINEUI_H
