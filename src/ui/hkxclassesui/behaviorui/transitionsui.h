#ifndef TRANSITIONSUI_H
#define TRANSITIONSUI_H

#include <QStackedWidget>

#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"

class TableWidget;
class LineEdit;
class ComboBox;
class hkbStateMachineTransitionInfoArray;
class QGridLayout;
class HkxObject;
class SpinBox;
class DoubleSpinBox;
class CheckBox;
class BlendingTransitionEffectUI;
class QStackedLayout;
class QPushButton;
class HkTransition;
class ConditionLineEdit;
class QGroupBox;
class BehaviorFile;
class CheckButtonCombo;
class GenericTableWidget;

class TransitionsUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
    friend class StateMachineUI;
    friend class StateUI;
public:
    TransitionsUI();
    virtual ~TransitionsUI(){}
    void loadData(BehaviorFile *parentfile, hkbStateMachine *parent, hkbStateMachineTransitionInfoArray::HkTransition *data, int index);
signals:
    void transitionNamChanged(const QString & newName, int index);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setTriggerIntervalEnterTime();
    void setTriggerIntervalExitTime();
    void setInitiateIntervalEnterTime();
    void setInitiateIntervalExitTime();
    void viewTransitionEffect();
    void toggleTransitionEffect(bool enable);
    void setCondition();
    void setToStateId(const QString &name);
    void setFromNestedStateId(const QString &name);
    void setToNestedStateId(const QString &name);
    void setPriority();
    void toggleGlobalWildcardFlag();
    void toggleUseNestedStateFlag();
    void toggleDisallowRandomTransitionFlag();
    void toggleDisallowReturnToStateFlag();
    void toggleAbutEndStateFlag();
    void returnToWidget();
    void viewSelectedChild(int row, int column);
    void transitionEffectRenamed(const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void eventTableElementSelected(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
    void setTriggerIntervalEnterEventId(int index, const QString &name);
    void setTriggerIntervalExitEventId(int index, const QString &name);
    void setInitiateIntervalEnterEventId(int index, const QString &name);
    void setInitiateIntervalExitEventId(int index, const QString &name);
    void setEventId(int index, const QString &name);
    void loadTableValue(int row, const QString &value);
    void eventRenamed(const QString & name, int index);
    void stateRenamed(const QString & name, int index);
private:
    enum TransitionEffectView {
        TRANSITION_WIDGET = 0,
        TRANSITION_EFFECT_WIDGET = 1
    };
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbStateMachine *parentObj;
    hkbStateMachineTransitionInfoArray::HkTransition *bsData;
    int transitionIndex;
    QPushButton *returnPB;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    TableWidget *table;
    DoubleSpinBox *enterTimeTI;
    DoubleSpinBox *exitTimeTI;
    DoubleSpinBox *enterTimeII;
    DoubleSpinBox *exitTimeII;
    CheckButtonCombo *transition;
    BlendingTransitionEffectUI *transitionUI; //Need Generator transition effect too???
    ConditionLineEdit *condition;
    ComboBox *toStateId;
    ComboBox *fromNestedStateId;
    ComboBox *toNestedStateId;
    SpinBox *priority;
    CheckBox *flagGlobalWildcard;
    CheckBox *flagUseNestedState;
    CheckBox *flagDisallowRandomTransition;
    CheckBox *flagDisallowReturnToState;
    CheckBox *flagAbutEndState;
};

#endif // TRANSITIONSUI_H
