#ifndef HKDATAUI_H
#define HKDATAUI_H

#include <QGroupBox>

class BehaviorGraphView;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QStackedLayout;
class HkxObject;
class QLabel;
class BSiStateTaggingGeneratorUI;
class ModifierGeneratorUI;
class ManualSelectorGeneratorUI;
class StateMachineUI;
class StateUI;
class CustomTreeGraphicsViewIcon;
class BehaviorVariablesUI;
class EventsUI;
class MainWindow;
class TransitionsUI;
class HkTransition;
class hkbStateMachineStateInfo;
class hkbStateMachine;
class HkxObjectTableWidget;

class HkDataUI: public QGroupBox
{
    Q_OBJECT
public:
    HkDataUI(const QString & title);
    virtual ~HkDataUI(){}
    BehaviorGraphView * setBehaviorView(BehaviorGraphView *view);
    void setEventsVariablesUI(EventsUI *events, BehaviorVariablesUI *variables);
public slots:
    void viewState(hkbStateMachineStateInfo *state);
    void viewTransition(hkbStateMachine *parent, HkTransition *transition);
    void viewStateMachine();
    void changeCurrentDataWidget(CustomTreeGraphicsViewIcon *icon);
    void modifierAdded(const QString & name, const QString & type);
    void modifierNameChanged(const QString & newName, int index);
    void modifierRemoved(int index);
    void generatorAdded(const QString & name, const QString & type);
    void generatorNameChanged(const QString & newName, int index);
    void generatorRemoved(int index);
    void eventNameChanged(const QString & newName, int index);
    void eventAdded(const QString & name);
    void eventRemoved(int index);
    void variableNameChanged(const QString & newName, int index);
    void variableAdded(const QString & name);
    void variableRemoved(int index);
private:
    HkDataUI& operator=(const HkDataUI&);
    HkDataUI(const HkDataUI &);
private:
    enum {
        NO_DATA_SELECTED = 0,
        BS_I_STATE_TAG_GEN = 1,
        MODIFIER_GENERATOR = 2,
        MANUAL_SELECTOR_GENERATOR = 3,
        STATE_MACHINE = 4,
        STATE = 5,
        TRANSITION = 6
    };
    static QStringList generatorTypes;
    static QStringList modifierTypes;
    EventsUI *eventsUI;
    BehaviorVariablesUI *variablesUI;
    BehaviorGraphView *behaviorView;
    QVBoxLayout *verLyt;
    QStackedLayout *stack;
    HkxObject *loadedData;
    HkxObjectTableWidget *generatorsTable;
    HkxObjectTableWidget *modifiersTable;
    QLabel *noDataL;
    BSiStateTaggingGeneratorUI *iSTGUI;
    ModifierGeneratorUI *modGenUI;
    ManualSelectorGeneratorUI *manSelGenUI;
    StateMachineUI *stateMachineUI;
    StateUI *stateUI;
    TransitionsUI *transitionUI;
};

#endif // HKDATAUI_H
