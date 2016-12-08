#include "hkdataui.h"

#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorvariablesui.h"
#include "src/ui/hkxclassesui/behaviorui/eventsui.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/modifiergeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/manualselectorgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/statemachineui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/stateui.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QCoreApplication>

QStringList HkDataUI::generatorTypes = {
        "hkbStateMachine",
        "hkbManualSelectorGenerator",
        "hkbBlenderGenerator",
        "BSiStateTaggingGenerator",
        "BSBoneSwitchGenerator",
        "BSCyclicBlendTransitionGenerator",
        "BSSynchronizedClipGenerator",
        "hkbModifierGenerator",
        "BSOffsetAnimationGenerator",
        "hkbPoseMatchingGenerator",
        "hkbClipGenerator",
        "hkbBehaviorReferenceGenerator"
};

QStringList HkDataUI::modifierTypes = {
    "hkbModifierList",
    "hkbTwistModifier",
    "hkbEventDrivenModifier",
    "BSIsActiveModifier",
    "BSLimbIKModifier",
    "BSInterpValueModifier",
    "BSGetTimeStepModifier",
    "hkbFootIkControlsModifier",
    "hkbGetHandleOnBoneModifier",
    "hkbTransformVectorModifier",
    "hkbProxyModifier",
    "hkbLookAtModifier",
    "hkbMirrorModifier",
    "hkbGetWorldFromModelModifier",
    "hkbSenseHandleModifier",
    "hkbEvaluateExpressionModifier",
    "hkbEvaluateHandleModifier",
    "hkbAttachmentModifier",
    "hkbAttributeModifier",
    "hkbCombineTransformsModifier",
    "hkbComputeRotationFromAxisAngleModifier",
    "hkbComputeRotationToTargetModifier",
    "hkbEventsFromRangeModifier",
    "hkbMoveCharacterModifier",
    "hkbExtractRagdollPoseModifier",
    "BSModifyOnceModifier",
    "BSEventOnDeactivateModifier",
    "BSEventEveryNEventsModifier",
    "BSRagdollContactListenerModifier",
    "hkbPoweredRagdollControlsModifier",
    "BSEventOnFalseToTrueModifier",
    "BSDirectAtModifier",
    "BSDistTriggerModifier",
    "BSDecomposeVectorModifier",
    "BSComputeAddBoneAnimModifier",
    "BSTweenerModifier",
    "BSIStateManagerModifier",
    "hkbTimerModifier",
    "hkbRotateCharacterModifier",
    "hkbDampingModifier",
    "hkbDelayedModifier",
    "hkbGetUpModifier",
    "hkbKeyframeBonesModifier",
    "hkbComputeDirectionModifier",
    "hkbRigidBodyRagdollControlsModifier",
    "BSSpeedSamplerModifier",
    "hkbDetectCloseToGroundModifier",
    "BSLookAtModifier",
    "BSTimerModifier"
};

HkDataUI::HkDataUI(const QString &title)
    : behaviorView(NULL),
      verLyt(new QVBoxLayout),
      stack(new QStackedLayout),
      loadedData(NULL),
      generatorsTable(new HkxObjectTableWidget("Select a hkbGenerator!")),
      modifiersTable(new HkxObjectTableWidget("Select a hkbModifier!")),
      noDataL(new QLabel("No Data Selected!")),
      iSTGUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI(NULL, NULL)),
      manSelGenUI(new ManualSelectorGeneratorUI),
      stateMachineUI(new StateMachineUI),
      stateUI(new StateUI),
      transitionUI(new TransitionsUI)
{
    setTitle(title);
    generatorsTable->setTypes(generatorTypes);
    modifiersTable->setTypes(modifierTypes);
    modGenUI->setGeneratorTable(generatorsTable);
    modGenUI->setModifierTable(modifiersTable);
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

    connect(iSTGUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(modGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(manSelGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(stateMachineUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(stateUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));

    connect(generatorsTable, SIGNAL(elementAdded(int)), this, SLOT());
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

void HkDataUI::modifierAdded(const QString & name, const QString & type){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    modifiersTable->addItem(name, type);
    QCoreApplication::processEvents();
}
void HkDataUI::modifierNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    //modifiersTable->renameItem(index, newName);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorAdded(const QString & name, const QString & type){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    manSelGenUI->generatorTable->addItem(name, type);
    stateUI->generatorTable->addItem(name, type);
    //generatorsTable->addItem(name, type);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    manSelGenUI->generatorTable->renameItem(index, newName);
    stateUI->generatorTable->renameItem(index, newName);
    //generatorsTable->renameItem(index, newName);
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
    manSelGenUI->generatorTable->removeItem(index);
    stateUI->generatorTable->removeItem(index);
    //generatorsTable->removeItem(index);
    QCoreApplication::processEvents();
}

void HkDataUI::modifierRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    //modifiersTable->removeItem(index);
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

void HkDataUI::changeCurrentDataWidget(CustomTreeGraphicsViewIcon * icon){
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

BehaviorGraphView *HkDataUI::setBehaviorView(BehaviorGraphView *view){
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
        manSelGenUI->loadComboBoxes();
        stateMachineUI->loadComboBoxes();
        transitionUI->loadComboBoxes();
        generatorsTable->loadTable(behaviorView->behavior);
        modifiersTable->loadTable(behaviorView->behavior);
        connect(behaviorView, SIGNAL(addedGenerator(QString)), this, SLOT(generatorAdded(QString)));
        connect(behaviorView, SIGNAL(addedModifier(QString)), this, SLOT(modifierAdded(QString)));
        connect(behaviorView, SIGNAL(removedGenerator(int)), this, SLOT(generatorRemoved(int)));
        connect(behaviorView, SIGNAL(removedModifier(int)), this, SLOT(modifierRemoved(int)));
    }
    return oldView;
}
