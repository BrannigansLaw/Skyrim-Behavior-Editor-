#include "hkdataui.h"

#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorvariablesui.h"
#include "src/ui/eventsui.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/objectdataui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/objectdataui/generators/modifiergeneratorui.h"
#include "src/ui/objectdataui/generators/manualselectorgeneratorui.h"
#include "src/ui/objectdataui/generators/statemachineui.h"
#include "src/ui/objectdataui/generators/stateui.h"
#include "src/ui/transitionsui.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QCoreApplication>

HkDataUI::HkDataUI(const QString &title)
    : behaviorView(NULL),
      verLyt(new QVBoxLayout),
      stack(new QStackedLayout),
      loadedData(NULL),
      noDataL(new QLabel("No Data Selected!")),
      iSTGUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI),
      manSelGenUI(new ManualSelectorGeneratorUI),
      stateMachineUI(new StateMachineUI),
      stateUI(new StateUI),
      transitionUI(new TransitionsUI)
{
    setTitle(title);
    stack->addWidget(noDataL);
    stack->addWidget(iSTGUI);
    stack->addWidget(modGenUI);
    stack->addWidget(manSelGenUI);
    stack->addWidget(stateMachineUI);
    stack->addWidget(stateUI);
    stack->addWidget(transitionUI);
    verLyt->addLayout(stack, 5);
    setLayout(verLyt);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(stateMachineUI, SIGNAL(viewState(hkbStateMachineStateInfo*)), this, SLOT(viewState(hkbStateMachineStateInfo*)));
    connect(stateMachineUI, SIGNAL(viewTransition(hkbStateMachine*,HkTransition*)), this, SLOT(viewTransition(hkbStateMachine*,HkTransition*)));
    connect(stateUI, SIGNAL(viewTransition(hkbStateMachine*,HkTransition*)), this, SLOT(viewTransition(hkbStateMachine*,HkTransition*)));
    connect(transitionUI, SIGNAL(returnToParent()), this, SLOT(viewStateMachine()));
    connect(stateUI, SIGNAL(toParentStateMachine()), this, SLOT(viewStateMachine()));
}

void HkDataUI::viewState(hkbStateMachineStateInfo *state){
    if (state){
        stateUI->loadData(state);
        stack->setCurrentIndex(STATE);
    }
}

void HkDataUI::viewTransition(hkbStateMachine *parent, HkTransition * transition){
    transitionUI->loadData(parent, transition);
    stack->setCurrentIndex(TRANSITION);
}

void HkDataUI::viewStateMachine(){
    stack->setCurrentIndex(SM);
}

void HkDataUI::setEventsVariablesUI(EventsUI *events, BehaviorVariablesUI *variables){
    eventsUI = events;
    variablesUI = variables;
    connect(eventsUI, SIGNAL(eventAdded(QString)), this, SLOT(eventAdded(QString)));
    connect(eventsUI, SIGNAL(eventRemoved(int)), this, SLOT(eventRemoved(int)));
    connect(eventsUI, SIGNAL(eventNameChanged(QString,int)), this, SLOT(eventNameChanged(QString,int)));
    connect(variablesUI, SIGNAL(variableAdded(QString)), this, SLOT(variableAdded(QString)));
    connect(variablesUI, SIGNAL(variableRemoved(int)), this, SLOT(variableRemoved(int)));
    connect(variablesUI, SIGNAL(variableNameChanged(QString,int)), this, SLOT(variableNameChanged(QString,int)));
}

void HkDataUI::modifierAdded(const QString & name){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    modGenUI->addModifierToLists(name);
    QCoreApplication::processEvents();
}
void HkDataUI::modifierNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    modGenUI->renameModifierInLists(newName, index);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorAdded(const QString & name){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->addGeneratorToLists(name);
    modGenUI->addGeneratorToLists(name);
    manSelGenUI->addGeneratorToLists(name);
    stateUI->addGeneratorToLists(name);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->renameGeneratorInLists(newName, index);
    modGenUI->renameGeneratorInLists(newName, index);
    manSelGenUI->renameGeneratorInLists(newName, index);
    stateUI->renameGeneratorInLists(newName, index);
    QCoreApplication::processEvents();
}

void HkDataUI::eventNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    stateMachineUI->renameEventInLists(newName, index);
    QCoreApplication::processEvents();
}

void HkDataUI::eventAdded(const QString & name){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    stateMachineUI->addEventToLists(name);
    QCoreApplication::processEvents();
}

void HkDataUI::eventRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    stateMachineUI->removeEventFromLists(index);
    QCoreApplication::processEvents();
}

void HkDataUI::variableNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->renameVariableInLists(newName, index);
    manSelGenUI->renameVariableInLists(newName, index);
    stateMachineUI->renameVariableInLists(newName, index);
    QCoreApplication::processEvents();
}

void HkDataUI::variableAdded(const QString & name){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->addVariableToLists(name);
    manSelGenUI->addVariableToLists(name);
    stateMachineUI->addVariableToLists(name);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->removeGeneratorFromLists(index);
    manSelGenUI->removeGeneratorFromLists(index);
    modGenUI->removeGeneratorFromLists(index);
    stateUI->removeGeneratorFromLists(index);
    switch (stack->currentIndex()) {
    case BS_I_STATE_TAGGING_GENERATOR:
        iSTGUI->loadData(loadedData);
        break;
    case HKB_MODIFIER_GENERATOR:
        modGenUI->loadData(loadedData);
        break;
    case HKB_MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->loadData(loadedData);
        break;
    case HKB_STATE_MACHINE:
        stateMachineUI->loadData(loadedData);
        break;
    default:
        stack->setCurrentIndex(NO_DATA_SELECTED);
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::modifierRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    modGenUI->removeModifierFromLists(index);
    /*switch (stack->currentIndex()) {
    case BS_I_STATE_TAGGING_GENERATOR:
        iSTGUI->loadData(loadedData);
        break;
    case HKB_MODIFIER_GENERATOR:
        modGenUI->loadData(loadedData);
        break;
    case HKB_MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->loadData(loadedData);
        break;
    case HKB_STATE_MACHINE:
        stateMachineUI->loadData(loadedData);
        break;
    default:
        stack->setCurrentIndex(NO_DATA_SELECTED);
        break;
    }*/
    QCoreApplication::processEvents();
}

void HkDataUI::variableRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    iSTGUI->removeVariableFromLists(index);
    manSelGenUI->removeVariableFromLists(index);
    stateMachineUI->removeVariableFromLists(index);
    behaviorView->behavior->removeBindings(index);
    behaviorView->removeOtherData();
    QCoreApplication::processEvents();
}

void HkDataUI::changeCurrentDataWidget(GeneratorIcon * icon){
    qulonglong sig;
    if (icon && icon->data.constData()){
        HkxObject *oldData = loadedData;
        sig = icon->data.constData()->getSignature();
        loadedData = icon->data.data();
        switch (sig) {
        case BS_I_STATE_TAGGING_GENERATOR:
            if (loadedData != oldData){
                iSTGUI->loadData(loadedData);
            }
            stack->setCurrentIndex(BS_I_STATE_TAG_GEN);
            break;
        case HKB_MODIFIER_GENERATOR:
            if (loadedData != oldData){
                modGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(MOD_GEN);
            break;
        case HKB_MANUAL_SELECTOR_GENERATOR:
            if (loadedData != oldData){
                manSelGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(MSG);
            break;
        case HKB_STATE_MACHINE:
            if (loadedData != oldData){
                stateMachineUI->loadData(loadedData);
            }
            stack->setCurrentIndex(SM);
            break;
        default:
            stack->setCurrentIndex(NO_DATA_SELECTED);
            break;
        }
    }else{
        stack->setCurrentIndex(NO_DATA_SELECTED);
        loadedData = NULL;
    }
}

BehaviorGraphView * HkDataUI::setBehaviorView(BehaviorGraphView *view){
    BehaviorGraphView *oldView = behaviorView;
    if (oldView){
        disconnect(oldView, 0, this, 0);
    }
    setMinimumSize(parentWidget()->size()*0.99);
    behaviorView = view;
    iSTGUI->behaviorView = view;
    modGenUI->behaviorView = view;
    manSelGenUI->behaviorView = view;
    stateMachineUI->behaviorView = view;
    stateUI->behaviorView = view;
    transitionUI->behaviorView = view;
    if (behaviorView){
        iSTGUI->loadComboBoxes();//Implement and change ->count() == 0){ code in all uis
        modGenUI->loadComboBoxes();
        manSelGenUI->loadComboBoxes();
        stateMachineUI->loadComboBoxes();
        stateUI->loadComboBoxes();
        transitionUI->loadComboBoxes();
        connect(behaviorView, SIGNAL(addedGenerator(QString)), this, SLOT(generatorAdded(QString)));
        connect(behaviorView, SIGNAL(addedModifier(QString)), this, SLOT(modifierAdded(QString)));
        connect(behaviorView, SIGNAL(removedGenerator(int)), this, SLOT(generatorRemoved(int)));
        connect(behaviorView, SIGNAL(removedModifier(int)), this, SLOT(modifierRemoved(int)));
    }
    connect(iSTGUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(modGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(manSelGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(stateMachineUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(stateUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    return oldView;
}