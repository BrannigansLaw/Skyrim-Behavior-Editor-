#ifndef TRANSITIONSUI_H
#define TRANSITIONSUI_H

#include <QStackedWidget>

#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"

class BehaviorGraphView;
class TableWidget;
class LineEdit;
class ComboBox;
class hkbStateMachineTransitionInfoArray;
class QGridLayout;
class HkxObject;
class SpinBox;
class DoubleSpinBox;
class QCheckBox;
class BlendingTransitionEffectUI;
class QStackedLayout;
class QPushButton;
class HkTransition;
class hkbGenerator;
class ConditionLineEdit;
class QGroupBox;
class BehaviorFile;

class TransitionsUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
    friend class StateMachineUI;
    friend class StateUI;
public:
    TransitionsUI();
    virtual ~TransitionsUI(){}
    void loadData(BehaviorFile *parentfile, hkbStateMachine *parent, hkbStateMachineTransitionInfoArray::HkTransition *data);
signals:
    void viewEvents(int index);
    void viewVariables(int index);
    void returnToParent();
private slots:
    void setTriggerIntervalEnterEventId(int index, const QString &name);
    void setTriggerIntervalExitEventId(int index, const QString &name);
    void setTriggerIntervalEnterTime();
    void setTriggerIntervalExitTime();
    void setInitiateIntervalEnterEventId(int index, const QString &name);
    void setInitiateIntervalExitEventId(int index, const QString &name);
    void setInitiateIntervalEnterTime();
    void setInitiateIntervalExitTime();
    void viewTransitionEffect();
    void setCondition();
    void setEventId(int index, const QString &name);
    void setToStateId(const QString &name);
    void setFromNestedStateId(const QString &name);
    void setToNestedStateId(const QString &name);
    void setPriority();
    void toggleGlobalWildcardFlag();
    void toggleUseNestedStateFlag();
    void toggleDisallowRandomTransitionFlag();
    void toggleDisallowReturnToStateFlag();
    void toggleAbutEndStateFlag();
    void eventTableElementSelected(int index, const QString &name);
    void returnToWidget();
    void viewSelectedChild(int row, int column);
private:
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
    QPushButton *returnPB;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    TableWidget *table;
    DoubleSpinBox *enterTimeTI;
    DoubleSpinBox *exitTimeTI;
    DoubleSpinBox *enterTimeII;
    DoubleSpinBox *exitTimeII;
    BlendingTransitionEffectUI *transition; //Need Generator transition effect too???
    ConditionLineEdit *condition;
    ComboBox *toStateId;
    ComboBox *fromNestedStateId;
    ComboBox *toNestedStateId;
    SpinBox *priority;
    QCheckBox *flagGlobalWildcard;
    QCheckBox *flagUseNestedState;
    QCheckBox *flagDisallowRandomTransition;
    QCheckBox *flagDisallowReturnToState;
    QCheckBox *flagAbutEndState;
};

#endif // TRANSITIONSUI_H
