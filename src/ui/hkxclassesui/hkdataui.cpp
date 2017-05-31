#include "hkdataui.h"

#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorvariablesui.h"
#include "src/ui/hkxclassesui/behaviorui/eventsui.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/modifiergeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/blendergeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/blendergeneratorchildui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/manualselectorgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/statemachineui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/stateui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/behaviorgraphui.h"
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
    "BSTimerModifier",
    "BSPassByTargetTriggerModifier"
};

QStringList HkDataUI::variableTypes = {
    "hkBool",
    "hkInt32",
    "hkReal",
    "hkPointer",
    "hkVector4",
    "hkQuaternion"
};

HkDataUI::HkDataUI(const QString &title)
    : behaviorView(NULL),
      verLyt(new QVBoxLayout),
      stack(new QStackedLayout),
      loadedData(NULL),
      generatorsTable(new GenericTableWidget("Select a hkbGenerator!")),
      modifiersTable(new GenericTableWidget("Select a hkbModifier!")),
      variablesTable(new GenericTableWidget("Select a Variable!")),
      eventsTable(new GenericTableWidget("Select an Event!")),
      characterPropertiesTable(new GenericTableWidget("Select a Character Property!")),
      noDataL(new QLabel("No Data Selected!")),
      iSTGUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI(NULL, NULL)),
      manSelGenUI(new ManualSelectorGeneratorUI),
      stateMachineUI(new StateMachineUI),
      blenderGeneratorUI(new BlenderGeneratorUI),
      behaviorGraphUI(new BehaviorGraphUI)
{
    setTitle(title);
    //generatorsTable->setTypes(generatorTypes);
    //modifiersTable->setTypes(modifierTypes);
    //variablesTable->setTypes(variableTypes);
    //eventsTable->setTypes(QStringList("hkEvents"));
    //characterPropertiesTable->setTypes(variableTypes);
    stack->addWidget(noDataL);
    stack->addWidget(iSTGUI);
    stack->addWidget(modGenUI);
    stack->addWidget(manSelGenUI);
    stack->addWidget(stateMachineUI);
    stack->addWidget(blenderGeneratorUI);
    stack->addWidget(behaviorGraphUI);
    verLyt->addLayout(stack, 5);
    setLayout(verLyt);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(iSTGUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(modGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(manSelGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(iSTGUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));
    connect(blenderGeneratorUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)));

    /*connect(blenderGeneratorUI, SIGNAL(viewVariables(int)), variablesTable, SLOT(showTable(int)));
    connect(blenderGeneratorUI, SIGNAL(viewProperties(int)), characterPropertiesTable, SLOT(showTable(int)));
    connect(blenderGeneratorUI, SIGNAL(viewGenerators(int)), generatorsTable, SLOT(showTable(int)));*/

    connect(behaviorView, SIGNAL(iconSelected(TreeGraphicsItem*)), this, SLOT(changeCurrentDataWidget(TreeGraphicsItem*)));
}

void HkDataUI::setEventsVariablesUI(EventsUI *events, BehaviorVariablesUI *variables){
    if (!events || !variables){
        return;
    }
    eventsUI = events;
    variablesUI = variables;
    connect(eventsUI, SIGNAL(eventAdded(QString)), this, SLOT(eventAdded(QString)));
    connect(eventsUI, SIGNAL(eventRemoved(int)), this, SLOT(eventRemoved(int)));
    connect(eventsUI, SIGNAL(eventNameChanged(QString,int)), this, SLOT(eventNameChanged(QString,int)));
    connect(variablesUI, SIGNAL(variableAdded(QString,QString)), this, SLOT(variableAdded(QString,QString)));
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
    modifiersTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case MODIFIER_GENERATOR:
        //rename modifier in table...
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::generatorAdded(const QString & name, const QString & type){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    generatorsTable->addItem(name, type);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    generatorsTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case BLENDER_GENERATOR:
        blenderGeneratorUI->generatorRenamed(newName, index);
        break;
    case STATE_MACHINE:
        stateMachineUI->generatorRenamed(newName, index);
        break;
    case MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->generatorRenamed(newName, index);
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::eventNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    eventsTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case STATE_MACHINE:
        stateMachineUI->eventRenamed(newName, index);
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::eventAdded(const QString & name){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    eventsTable->addItem(name, "hkEvent");
    QCoreApplication::processEvents();
}

void HkDataUI::eventRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    eventsTable->removeItem(index);
    switch (stack->currentIndex()) {
    case STATE_MACHINE:
        stateMachineUI->eventRenamed("NONE", index);
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::variableNameChanged(const QString & newName, int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    variablesTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case BLENDER_GENERATOR:
        blenderGeneratorUI->variableRenamed(newName, index);
        break;
    case STATE_MACHINE:
        stateMachineUI->variableRenamed(newName, index);
        break;
    case MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->variableRenamed(newName, index);
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::variableAdded(const QString & name, const QString & type){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    variablesTable->addItem(name, type);
    QCoreApplication::processEvents();
}

void HkDataUI::generatorRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    generatorsTable->removeItem(index);
    switch (stack->currentIndex()){
    case HKB_BLENDER_GENERATOR:
        blenderGeneratorUI->loadData(loadedData);
        blenderGeneratorUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case BS_I_STATE_TAGGING_GENERATOR:
        //
        break;
    case HKB_MODIFIER_GENERATOR:
        //
        break;
    case HKB_MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->loadData(loadedData);
        manSelGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case HKB_STATE_MACHINE:
        stateMachineUI->loadData(loadedData);
        stateMachineUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
        break;
    case HKB_BEHAVIOR_GRAPH:
        behaviorGraphUI->loadData(loadedData);
        behaviorGraphUI->connectToTables(generatorsTable);
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::modifierRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    modifiersTable->removeItem(index);
    switch (stack->currentIndex()) {
    case MODIFIER_GENERATOR:
        //reload active widget table for multi-modifier child class...
        break;
    default:
        break;
    }
    QCoreApplication::processEvents();
}

void HkDataUI::variableRemoved(int index){
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 5);
    variablesTable->removeItem(index);
    switch (stack->currentIndex()){
    case BLENDER_GENERATOR:
        blenderGeneratorUI->variableRenamed("NONE", index);
        break;
    case STATE_MACHINE:
        stateMachineUI->variableRenamed("NONE", index);
        break;
    case MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->variableRenamed("NONE", index);
        break;
    default:
        break;
    }
    behaviorView->behavior->removeBindings(index);
    behaviorView->removeOtherData();
    QCoreApplication::processEvents();
}

void HkDataUI::changeCurrentDataWidget(TreeGraphicsItem * icon){
    HkxSignature sig;
    if (icon && (icon->itemData)){
        HkxObject *oldData = loadedData;
        sig = ((HkxObject *)((icon->itemData)))->getSignature();
        loadedData = ((HkxObject *)((icon->itemData)));
        switch (sig){
        case HKB_BLENDER_GENERATOR:
            if (loadedData != oldData){
                blenderGeneratorUI->loadData(loadedData);
            }
            stack->setCurrentIndex(BLENDER_GENERATOR);
            blenderGeneratorUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
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
            stack->setCurrentIndex(MODIFIER_GENERATOR);
            break;
        case HKB_MANUAL_SELECTOR_GENERATOR:
            if (loadedData != oldData){
                manSelGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(MANUAL_SELECTOR_GENERATOR);
            manSelGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HKB_STATE_MACHINE:
            if (loadedData != oldData){
                stateMachineUI->loadData(loadedData);
            }
            stack->setCurrentIndex(STATE_MACHINE);
            stateMachineUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HKB_BEHAVIOR_GRAPH:
            if (loadedData != oldData){
                behaviorGraphUI->loadData(loadedData);
            }
            stack->setCurrentIndex(BEHAVIOR_GRAPH);
            behaviorGraphUI->connectToTables(generatorsTable);
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
        //disconnect(oldView, 0, this, 0);
    }
    if (parentWidget()){
        setMinimumSize(parentWidget()->size()*0.99);
    }
    behaviorView = view;
    iSTGUI->behaviorView = view;
    modGenUI->behaviorView = view;
    manSelGenUI->setBehaviorView(view);
    stateMachineUI->setBehaviorView(view);
    blenderGeneratorUI->setBehaviorView(view);
    behaviorGraphUI->setBehaviorView(view);
    if (behaviorView){
        generatorsTable->loadTable(behaviorView->behavior->getGeneratorNames(), behaviorView->behavior->getGeneratorTypeNames(), "NULL");
        modifiersTable->loadTable(behaviorView->behavior->getModifierNames(), behaviorView->behavior->getModifierTypeNames(), "NULL");
        variablesTable->loadTable(behaviorView->behavior->getVariableNames(), behaviorView->behavior->getVariableTypenames(), "NONE");
        eventsTable->loadTable(behaviorView->behavior->getEventNames(), "hkEvent", "NONE");
        characterPropertiesTable->loadTable(behaviorView->behavior->getCharacterPropertyNames(), behaviorView->behavior->getCharacterPropertyTypenames(), "NONE");//inefficient...
        connect(behaviorView, SIGNAL(addedGenerator(QString,QString)), this, SLOT(generatorAdded(QString,QString)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(addedModifier(QString,QString)), this, SLOT(modifierAdded(QString,QString)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(removedGenerator(int)), this, SLOT(generatorRemoved(int)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(removedModifier(int)), this, SLOT(modifierRemoved(int)), Qt::UniqueConnection);
    }
    return oldView;
}
