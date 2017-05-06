#ifndef STATEUI_H
#define STATEUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class TransitionsUI;
class hkbStateMachineStateInfo;
class QGridLayout;
class TableWidget;
class QTableWidgetItem;
class SpinBox;
class LineEdit;
class ComboBox;
class QPushButton;
class QHBoxLayout;
class QSignalMapper;
class DoubleSpinBox;
class QCheckBox;
class EventUI;
class HkTransition;
class hkbStateMachineEventPropertyArray;
class hkbStateMachine;
class QGroupBox;

class StateUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
    friend class StateMachineUI;
public:
    StateUI();
    virtual ~StateUI(){}
    void loadData(HkxObject *data);
signals:
    void stateNameChanged(const QString & newName, int index);
    void viewGenerators(int index);
    void returnToParent();
    void viewEvents(int index);
private slots:
    void setName();
    void setGenerator(int index, const QString &name);
    void setStateId();
    void setProbability();
    void setEnable();
    void setEvent(int index, const QString & name);
    void addEnterEvent();
    void addExitEvent();
    void addTransition();
    void removeObjectChild();
    void viewSelectedChild(int row, int column);
    void returnToWidget();
private:
    void loadDynamicTableRows();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value);
    void eventRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void generatorRenamed(const QString & name, int index);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        EVENT_PAYLOAD_WIDGET = 1,
        TRANSITION_WIDGET = 2
    };
    static QStringList headerLabels;
    int exitEventsButtonRow;
    int transitionsButtonRow;
    int rowToRemove;
    BehaviorGraphView *behaviorView;
    hkbStateMachineStateInfo *bsData;
    QGroupBox *groupBox;
    EventUI *eventUI;
    TransitionsUI *transitionUI;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *returnPB;
    LineEdit *name;
    QPushButton *generator;
    SpinBox *stateId;
    DoubleSpinBox *probability;
    QCheckBox *enable;
    QCheckBox *enableTransitions;
    QPushButton *addEnterEventPB;
    QPushButton *removeEnterEventPB;
    QPushButton *addExitEventPB;
    QPushButton *removeExitEventPB;
    QPushButton *addTransitionPB;
    QPushButton *removeTransitionPB;
};

#endif // STATEUI_H
