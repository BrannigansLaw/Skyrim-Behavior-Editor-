#ifndef STATEUI_H
#define STATEUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class TransitionsUI;
class hkbStateMachineStateInfo;
class QGridLayout;
class TableWidget;
class SpinBox;
class LineEdit;
class ComboBox;
class QPushButton;
class DoubleSpinBox;
class CheckBox;
class EventUI;
class QGroupBox;

class StateUI: public QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
    friend class StateMachineUI;
public:
    StateUI();
    virtual ~StateUI(){}
    void loadData(HkxObject *data, int stateindex);
signals:
    void stateNameChanged(const QString & newName, int index);
    void viewGenerators(int index);
    void returnToParent(bool reloadData);
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
private slots:
    void setName();
    void setStateId();
    void setProbability();
    void setEnable();
    void eventTableElementSelected(int index, const QString & name);
    void viewSelectedChild(int row, int column);
    void returnToWidget();
    void transitionRenamed(const QString &name, int index);
private:
    void connectSignals();
    void disconnectSignals();
    void setGenerator(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
    void addEnterEvent();
    void removeEnterEvent(int index);
    void addExitEvent();
    void removeExitEvent(int index);
    void addTransition();
    void removeTransition(int index);
    void loadDynamicTableRows();
    void setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
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
    BehaviorGraphView *behaviorView;
    hkbStateMachineStateInfo *bsData;
    int stateIndex;
    QGroupBox *groupBox;
    EventUI *eventUI;
    TransitionsUI *transitionUI;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *returnPB;
    LineEdit *name;
    SpinBox *stateId;
    DoubleSpinBox *probability;
    CheckBox *enable;
    CheckBox *enableTransitions;
};

#endif // STATEUI_H
