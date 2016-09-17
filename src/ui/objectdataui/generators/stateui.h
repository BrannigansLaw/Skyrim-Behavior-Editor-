#ifndef STATEUI_H
#define STATEUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class PointerWidget;
class StringWidget;
class IntWidget;
class TransitionsUI;
class hkbStateMachineStateInfo;
class QVBoxLayout;
class QTableWidget;
class QTableWidgetItem;
class QSpinBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class QSignalMapper;
class QDoubleSpinBox;
class QCheckBox;
class QStackedLayout;

class StateUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    StateUI();
    virtual ~StateUI(){}
    void loadData(HkxObject *data);
signals:
    void stateNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setGenerator(int index);
    void setStateId();
    void setProbability();
    void setEnable();
    void addEnterEvent();
    void setEnterEventAt(int index);
    void removeEnterEvent(int index);
    void addExitEvent();
    void setExitEventAt(int index);
    void removeExitEvent(int index);
    void addTransition();
    void removeTransition(int index);
    void viewTransition(int row, int column);
    void viewTransitionTable();
private:
    void addGeneratorToLists(const QString & name);
    void removeGeneratorFromLists(int index);
    void renameGeneratorInLists(const QString & name, int index);
    void addEventToLists(const QString & name);
    void removeEventFromLists(int index);
    void renameEventInLists(const QString & newName, int index);
private:
    enum TransitionView {
        TRANSITION_TABLE = 0,
        TRANSITION_WIDGET = 1
    };
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    hkbStateMachineStateInfo *bsData;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QLineEdit *name;
    QComboBox *generator;
    QSpinBox *stateId;
    QDoubleSpinBox *probability;
    QCheckBox *enable;
    QTableWidget *enterNotifyEvents;
    QTableWidget *exitNotifyEvents;
    QTableWidget *transitions;
    QStackedLayout *stackLyt;
    QPushButton *addEnterEventPB;
    QPushButton *removeEnterEventPB;
    QComboBox *typeEnterEventCB;
    QSignalMapper *enterEventSignalMapper;
    QHBoxLayout *enterEventButtonLyt;
    QPushButton *addExitEventPB;
    QPushButton *removeExitEventPB;
    QComboBox *typeExitEventCB;
    QSignalMapper *exitEventSignalMapper;
    QHBoxLayout *exitEventButtonLyt;
    QPushButton *addTransitionPB;
    QPushButton *removeTransitionPB;
    QComboBox *typeTransitionCB;
    //QSignalMapper *transitionSignalMapper;
    QHBoxLayout *transitionButtonLyt;
    TransitionsUI *transitionWidget;
};

#endif // STATEUI_H
