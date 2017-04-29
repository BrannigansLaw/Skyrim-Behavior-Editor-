#ifndef TRANSITIONSUI_H
#define TRANSITIONSUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"

class BehaviorGraphView;
class TableWidget;
class LineEdit;
class ComboBox;
class hkbStateMachineTransitionInfoArray;
class QVBoxLayout;
class HkxObject;
class SpinBox;
class DoubleSpinBox;
class QCheckBox;
class BlendingTransitionEffectUI;
class QStackedLayout;
class QPushButton;
class HkTransition;
class hkbGenerator;

class TransitionsUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
    friend class StateMachineUI;
    friend class StateUI;
public:
    TransitionsUI();
    virtual ~TransitionsUI(){}
    void loadData(hkbStateMachine *parent, HkTransition *data);
signals:
    void returnToParent();
private slots:
    void setTriggerIntervalEnterEventId();
    void setTriggerIntervalExitEventId();
    void setTriggerIntervalEnterTime();
    void setTriggerIntervalExitTime();
    void setInitiateIntervalEnterEventId();
    void setInitiateIntervalExitEventId();
    void setInitiateIntervalEnterTime();
    void setInitiateIntervalExitTime();
    void viewTransition();
    void setCondition();
    void setEventId();
    void setToStateId();
    void setFromNestedStateId();
    void setToNestedStateId();
    void setPriority();
    void toggleWildcardFlag();
    void toggleLocalFlag();
    void toggleUseNestedStateFlag();
    void toggleDisallowRandomTransitionFlag();
    void toggleDisallowReturnToStateFlag();
    void toggleAbutEndStateFlag();
private:
    void addEventToLists(const QString & name);
    void removeEventFromLists(int index);
    void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void loadComboBoxes();
private:
    enum TransitionEffectView {
        TRANSITION_WIDGET = 0,
        TRANSITION_EFFECT_WIDGET = 1
    };
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    hkbStateMachine *parentObj;
    hkbStateMachineTransitionInfoArray::HkTransition *bsData;
    QPushButton *returnToParentPB;
    QVBoxLayout *lyt;
    TableWidget *table;
    QPushButton *returnPB;
    ComboBox *enterEventIdTI;
    ComboBox *exitEventIdTI;
    DoubleSpinBox *enterTimeTI;
    DoubleSpinBox *exitTimeTI;
    ComboBox *enterEventIdII;
    ComboBox *exitEventIdII;
    DoubleSpinBox *enterTimeII;
    DoubleSpinBox *exitTimeII;
    LineEdit *condition;
    ComboBox *eventId;
    ComboBox *toStateId;
    ComboBox *fromNestedStateId;
    ComboBox *toNestedStateId;
    SpinBox *priority;
    QCheckBox *flagWildcard;
    QCheckBox *flagLocal;
    QCheckBox *flagUseNestedState;
    QCheckBox *flagDisallowRandomTransition;
    QCheckBox *flagDisallowReturnToState;
    QCheckBox *flagAbutEndState;
    QStackedLayout *stackLyt;
};

#endif // TRANSITIONSUI_H
