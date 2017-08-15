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
#include "src/ui/hkxclassesui/behaviorui/animationsui.h"
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
#include "src/ui/hkxclassesui/behaviorui/generators/bsoffsetanimationgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bsboneswitchgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bsboneswitchgeneratorbonedataui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bscyclicblendtransitiongeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/posematchinggeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/clipgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bssynchronizedclipgeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/behaviorreferencegeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bslimbikmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsdirectatmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/movecharactermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/rotatecharactermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/evaluateexpressionmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/modifierlistui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/eventdrivenmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/gethandleonbonemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/evaluatehandlemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/sensehandlemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsdecomposevectormodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsisactivemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/computedirectionmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bscomputeaddboneanimmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsdisttriggermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsinterpvaluemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/getupmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/getworldfrommodelmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/twistmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/timermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/dampingmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/rigidbodyragdollcontrolsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/poweredragdollcontrolsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/combinetransformsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/computerotationfromaxisanglemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/computerotationtotargetmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/transformvectormodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/lookatmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/keyframebonesmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/footikcontrolsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/mirrormodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/extractragdollposemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bstimermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsgettimestepmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/delayedmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsragdollcontactlistenermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bseventondeactivatemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsspeedsamplermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bspassbytargettriggermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bslookatmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/detectclosetogroundmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bseventeveryneventsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bseventonfalsetotruemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bsmodifyoncemodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/handikcontrolsmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/bstweenermodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bgsgamebryosequencegeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/modifiers/eventsfromrangemodifierui.h"

#include "src/ui/hkxclassesui/behaviorui/expressiondataarrayui.h"

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
    "hkbBehaviorReferenceGenerator",
    "BGSGamebryoSequenceGenerator"
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
    "BSPassByTargetTriggerModifier",
    "hkbHandIKControlsModifier"
};

QStringList HkDataUI::variableTypes = {
    "hkBool",
    "hkInt32",
    "hkReal",
    "hkPointer",
    "hkVector4",
    "hkQuaternion"
};

HkDataUI::HkDataUI(MainWindow *mainui, const QString &title)
    : mainUI(mainui),
      behaviorView(NULL),
      verLyt(new QVBoxLayout),
      stack(new QStackedLayout),
      loadedData(NULL),
      eventsUI(NULL),
      variablesUI(NULL),
      animationsUI(NULL),
      generatorsTable(new GenericTableWidget("Select a hkbGenerator!")),
      modifiersTable(new GenericTableWidget("Select a hkbModifier!")),
      variablesTable(new GenericTableWidget("Select a Variable!")),
      eventsTable(new GenericTableWidget("Select an Event!")),
      characterPropertiesTable(new GenericTableWidget("Select a Character Property!")),
      animationsTable(new GenericTableWidget("Select an Animation!")),
      ragdollBonesTable(new GenericTableWidget("Select a Ragdoll Bone!")),
      noDataL(new QLabel("No Data Selected!")),
      iStateTagGenUI(new BSiStateTaggingGeneratorUI),
      modGenUI(new ModifierGeneratorUI),
      manSelGenUI(new ManualSelectorGeneratorUI),
      stateMachineUI(new StateMachineUI),
      blenderGeneratorUI(new BlenderGeneratorUI),
      behaviorGraphUI(new BehaviorGraphUI),
      limbIKModUI(new BSLimbIKModifierUI),
      boneSwitchUI(new BSBoneSwitchGeneratorUI),
      offsetAnimGenUI(new BSOffsetAnimationGeneratorUI),
      cyclicBlendTransGenUI(new BSCyclicBlendTransitionGeneratorUI),
      poseMatchGenUI(new PoseMatchingGeneratorUI),
      clipGenUI(new ClipGeneratorUI),
      syncClipGenUI(new BSSynchronizedClipGeneratorUI),
      behaviorRefGenUI(new BehaviorReferenceGeneratorUI),
      directAtModUI(new BSDirectAtModifierUI),
      moveCharModUI(new MoveCharacterModifierUI),
      rotateCharModUI(new RotateCharacterModifierUI),
      evaluateExpModUI(new EvaluateExpressionModifierUI),
      modListUI(new ModifierListUI),
      eventDrivenModUI(new EventDrivenModifierUI),
      getHandleOnBoneUI(new GetHandleOnBoneModifierUI),
      evaluateHandleModUI(new EvaluateHandleModifierUI),
      senseHandleModUI(new SenseHandleModifierUI),
      decomposeVectorModUI(new BSDecomposeVectorModifierUI),
      isActiveModUI(new BSIsActiveModifierUI),
      computeDirMod(new ComputeDirectionModifierUI),
      computeAddAnimUI(new BSComputeAddBoneAnimModifierUI),
      distTriggerModUI(new BSDistTriggerModifierUI),
      interpValueModUI(new BSInterpValueModifierUI),
      getUpModUI(new GetUpModifierUI),
      getWorldFromModelModUI(new GetWorldFromModelModifierUI),
      twistModUI(new TwistModifierUI),
      timerModUI(new TimerModifierUI),
      dampingModUI(new DampingModifierUI),
      rigidRagdollControlsModUI(new RigidBodyRagdollControlsModifierUI),
      poweredRagdollControlsModUI(new PoweredRagdollControlsModifierUI),
      combineTransModUI(new CombineTransformsModifierUI),
      computeRotationAxisAngleModUI(new ComputeRotationFromAxisAngleModifierUI),
      computeRotationToTargetModUI(new ComputeRotationToTargetModifierUI),
      transformVectorModUI(new TransformVectorModifierUI),
      lookAtModUI(new LookAtModifierUI),
      keyframeBonesModUI(new KeyframeBonesModifierUI),
      footIKControlsModUI(new FootIkControlsModifierUI),
      mirrorModUI(new MirrorModifierUI),
      extractRagdollPoseModUI(new ExtractRagdollPoseModifierUI),
      bsTimerModUI(new BSTimerModifierUI),
      getTimeStepModUI(new BSGetTimeStepModifierUI),
      delayedModUI(new DelayedModifierUI),
      ragdollContactListenerModUI(new BSRagdollContactListenerModifierUI),
      eventOnDeactivateModUI(new BSEventOnDeactivateModifierUI),
      speedSamplerModUI(new BSSpeedSamplerModifierUI),
      passByTargetTriggerModUI(new BSPassByTargetTriggerModifierUI),
      bsLookAtModUI(new BSLookAtModifierUI),
      detectCloseToGroundModUI(new DetectCloseToGroundModifierUI),
      eventEveryNEventsModUI(new BSEventEveryNEventsModifierUI),
      eventOnFalseToTrueModUI(new BSEventOnFalseToTrueModifierUI),
      modifyOnceModUI(new BSModifyOnceModifierUI),
      handIKControlsModUI(new HandIkControlsModifierUI),
      tweenerModUI(new BSTweenerModifierUI),
      gamebryoSequenceGenUI(new BGSGamebryoSequenceGeneratorUI),
      eventsFromRangeModUI(new EventsFromRangeModifierUI)
{
    setTitle(title);
    stack->addWidget(noDataL);
    stack->addWidget(iStateTagGenUI);
    stack->addWidget(modGenUI);
    stack->addWidget(manSelGenUI);
    stack->addWidget(stateMachineUI);
    stack->addWidget(blenderGeneratorUI);
    stack->addWidget(behaviorGraphUI);
    stack->addWidget(limbIKModUI);
    stack->addWidget(boneSwitchUI);
    stack->addWidget(offsetAnimGenUI);
    stack->addWidget(cyclicBlendTransGenUI);
    stack->addWidget(poseMatchGenUI);
    stack->addWidget(clipGenUI);
    stack->addWidget(syncClipGenUI);
    stack->addWidget(behaviorRefGenUI);
    stack->addWidget(directAtModUI);
    stack->addWidget(moveCharModUI);
    stack->addWidget(rotateCharModUI);
    stack->addWidget(evaluateExpModUI);
    stack->addWidget(modListUI);
    stack->addWidget(eventDrivenModUI);
    stack->addWidget(getHandleOnBoneUI);
    stack->addWidget(evaluateHandleModUI);
    stack->addWidget(senseHandleModUI);
    stack->addWidget(decomposeVectorModUI);
    stack->addWidget(isActiveModUI);
    stack->addWidget(computeDirMod);
    stack->addWidget(computeAddAnimUI);
    stack->addWidget(distTriggerModUI);
    stack->addWidget(interpValueModUI);
    stack->addWidget(getUpModUI);
    stack->addWidget(getWorldFromModelModUI);
    stack->addWidget(twistModUI);
    stack->addWidget(timerModUI);
    stack->addWidget(dampingModUI);
    stack->addWidget(rigidRagdollControlsModUI);
    stack->addWidget(poweredRagdollControlsModUI);
    stack->addWidget(combineTransModUI);
    stack->addWidget(computeRotationAxisAngleModUI);
    stack->addWidget(computeRotationToTargetModUI);
    stack->addWidget(transformVectorModUI);
    stack->addWidget(lookAtModUI);
    stack->addWidget(keyframeBonesModUI);
    stack->addWidget(footIKControlsModUI);
    stack->addWidget(mirrorModUI);
    stack->addWidget(extractRagdollPoseModUI);
    stack->addWidget(bsTimerModUI);
    stack->addWidget(getTimeStepModUI);
    stack->addWidget(delayedModUI);
    stack->addWidget(ragdollContactListenerModUI);
    stack->addWidget(eventOnDeactivateModUI);
    stack->addWidget(speedSamplerModUI);
    stack->addWidget(passByTargetTriggerModUI);
    stack->addWidget(bsLookAtModUI);
    stack->addWidget(detectCloseToGroundModUI);
    stack->addWidget(eventEveryNEventsModUI);
    stack->addWidget(eventOnFalseToTrueModUI);
    stack->addWidget(modifyOnceModUI);
    stack->addWidget(handIKControlsModUI);
    stack->addWidget(tweenerModUI);
    stack->addWidget(gamebryoSequenceGenUI);
    stack->addWidget(eventsFromRangeModUI);
    verLyt->addLayout(stack, 5);
    setLayout(verLyt);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(stateMachineUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(modGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(manSelGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(iStateTagGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(blenderGeneratorUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(behaviorGraphUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(boneSwitchUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(offsetAnimGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(cyclicBlendTransGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(poseMatchGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(clipGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(syncClipGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(behaviorRefGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);
    connect(gamebryoSequenceGenUI, SIGNAL(generatorNameChanged(QString,int)), this, SLOT(generatorNameChanged(QString,int)), Qt::UniqueConnection);

    connect(limbIKModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(directAtModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(moveCharModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(rotateCharModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(evaluateExpModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(modListUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(eventDrivenModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(getHandleOnBoneUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(evaluateHandleModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(senseHandleModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(decomposeVectorModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(isActiveModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(computeDirMod, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(computeAddAnimUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(distTriggerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(interpValueModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(getUpModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(getWorldFromModelModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(twistModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(timerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(dampingModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(rigidRagdollControlsModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(poweredRagdollControlsModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(combineTransModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(computeRotationAxisAngleModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(computeRotationToTargetModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(transformVectorModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(lookAtModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(keyframeBonesModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(footIKControlsModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(mirrorModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(extractRagdollPoseModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(bsTimerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(getTimeStepModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(delayedModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(ragdollContactListenerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(eventOnDeactivateModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(speedSamplerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(passByTargetTriggerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(bsLookAtModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(detectCloseToGroundModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(eventEveryNEventsModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(eventOnFalseToTrueModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(modifyOnceModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(handIKControlsModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(tweenerModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);
    connect(eventsFromRangeModUI, SIGNAL(modifierNameChanged(QString,int)), this, SLOT(modifierNameChanged(QString,int)), Qt::UniqueConnection);

    connect(animationsUI, SIGNAL(animationNameChanged(QString,int)), this, SLOT(animationNameChanged(QString,int)), Qt::UniqueConnection);
    connect(animationsUI, SIGNAL(animationAdded(QString)), this, SLOT(animationAdded(QString)), Qt::UniqueConnection);
    connect(animationsUI, SIGNAL(animationRemoved(int)), this, SLOT(animationRemoved(int)), Qt::UniqueConnection);

    connect(behaviorView, SIGNAL(iconSelected(TreeGraphicsItem*)), this, SLOT(changeCurrentDataWidget(TreeGraphicsItem*)), Qt::UniqueConnection);
}

void HkDataUI::setEventsVariablesAnimationsUI(EventsUI *events, BehaviorVariablesUI *variables, AnimationsUI *animations){
    if (!events || !variables || !animations){
        return;
    }
    eventsUI = events;
    variablesUI = variables;
    animationsUI = animations;
    connect(eventsUI, SIGNAL(eventAdded(QString)), this, SLOT(eventAdded(QString)), Qt::UniqueConnection);
    connect(eventsUI, SIGNAL(eventRemoved(int)), this, SLOT(eventRemoved(int)), Qt::UniqueConnection);
    connect(eventsUI, SIGNAL(eventNameChanged(QString,int)), this, SLOT(eventNameChanged(QString,int)), Qt::UniqueConnection);
    connect(variablesUI, SIGNAL(variableAdded(QString,QString)), this, SLOT(variableAdded(QString,QString)), Qt::UniqueConnection);
    connect(variablesUI, SIGNAL(variableRemoved(int)), this, SLOT(variableRemoved(int)), Qt::UniqueConnection);
    connect(variablesUI, SIGNAL(variableNameChanged(QString,int)), this, SLOT(variableNameChanged(QString,int)), Qt::UniqueConnection);
    connect(animationsUI, SIGNAL(animationAdded(QString)), this, SLOT(animationAdded(QString)), Qt::UniqueConnection);
    connect(animationsUI, SIGNAL(animationRemoved(int)), this, SLOT(animationRemoved(int)), Qt::UniqueConnection);
    connect(animationsUI, SIGNAL(animationNameChanged(QString,int)), this, SLOT(animationNameChanged(QString,int)), Qt::UniqueConnection);
}

void HkDataUI::unloadDataWidget(){
    disconnect(variablesTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(animationsTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(characterPropertiesTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(generatorsTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(modifiersTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(eventsTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    disconnect(ragdollBonesTable, SIGNAL(elementSelected(int,QString)), 0, 0);
    loadedData = NULL;
    stack->setCurrentIndex(DATA_TYPE_LOADED::NO_DATA_SELECTED);
}

void HkDataUI::modifierAdded(const QString & name, const QString & type){
    modifiersTable->addItem(name, type);
}
void HkDataUI::modifierNameChanged(const QString & newName, int index){
    index++;
    modifiersTable->renameItem(index, newName);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::MODIFIER_GENERATOR:
        modGenUI->modifierRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MODIFIER_LIST:
        modListUI->modifierRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->modifierRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DELAYED_MODIFIER:
        delayedModUI->modifierRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_MODIFY_ONCE_MODIFIER:
        modifyOnceModUI->modifierRenamed(newName, index);
        break;
    }
}

void HkDataUI::modifierRemoved(int index){
    //index++;
    modifiersTable->removeItem(index);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::MODIFIER_GENERATOR:
        modGenUI->modifierRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::MODIFIER_LIST:
        //reload active widget table for multi-modifier child class...
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->modifierRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DELAYED_MODIFIER:
        delayedModUI->modifierRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_MODIFY_ONCE_MODIFIER:
        modifyOnceModUI->modifierRenamed("NONE", index);
        break;
    }
}

void HkDataUI::generatorAdded(const QString & name, const QString & type){
    generatorsTable->addItem(name, type);
}

void HkDataUI::generatorNameChanged(const QString & newName, int index){
    index++;
    generatorsTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case DATA_TYPE_LOADED::BLENDER_GENERATOR:
        blenderGeneratorUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MODIFIER_GENERATOR:
        modGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_I_STATE_TAG_GEN:
        iStateTagGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_BONE_SWITCH_GENERATOR:
        boneSwitchUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_OFFSET_ANIMATION_GENERATOR:
        offsetAnimGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        cyclicBlendTransGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->generatorRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::SYNCHRONIZED_CLIP_GENERATOR:
        syncClipGenUI->generatorRenamed(newName, index);
        break;
    }
}

void HkDataUI::generatorRemoved(int index){
    //index++;
    generatorsTable->removeItem(index);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::BLENDER_GENERATOR:
        blenderGeneratorUI->loadData(loadedData);
        //blenderGeneratorUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case DATA_TYPE_LOADED::BS_BONE_SWITCH_GENERATOR:
        boneSwitchUI->loadData(loadedData);
        //boneSwitchUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case DATA_TYPE_LOADED::MODIFIER_GENERATOR:
        modGenUI->loadData(loadedData);
        //modGenUI->connectToTables(modifiersTable, generatorsTable);
        break;
    case DATA_TYPE_LOADED::MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->loadData(loadedData);
        //manSelGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->loadData(loadedData);
        //stateMachineUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
        break;
    case DATA_TYPE_LOADED::BS_I_STATE_TAG_GEN:
        iStateTagGenUI->loadData(loadedData);
        //iStateTagGenUI->connectToTables(variablesTable, characterPropertiesTable, generatorsTable);
        break;
    case DATA_TYPE_LOADED::BS_OFFSET_ANIMATION_GENERATOR:
        offsetAnimGenUI->loadData(loadedData);
        //iStateTagGenUI->connectToTables(variablesTable, characterPropertiesTable, generatorsTable);
        break;
    case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        cyclicBlendTransGenUI->loadData(loadedData);
        //cyclicBlendTransGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
        break;
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->loadData(loadedData);
        //iStateTagGenUI->connectToTables(variablesTable, characterPropertiesTable, generatorsTable);
        break;
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->loadData(loadedData);
        //clipGenUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable, animationsTables);
        break;
    case DATA_TYPE_LOADED::SYNCHRONIZED_CLIP_GENERATOR:
        syncClipGenUI->loadData(loadedData);
        //syncClipGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
        break;
    case DATA_TYPE_LOADED::BEHAVIOR_GRAPH:
        behaviorGraphUI->loadData(loadedData);
        //behaviorGraphUI->connectToTables(generatorsTable);
        break;
    }
}

void HkDataUI::eventNameChanged(const QString & newName, int index){
    index++;
    eventsTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->eventRenamed(newName, index);
        break;
    /*case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        cyclicBlendTransGenUI->eventRenamed(newName, index);
        break;*/
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_EXPRESSION_MODIFIER:
        evaluateExpModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::SENSE_HANDLE_MODIFIER:
        senseHandleModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_DIST_TRIGGER_MODIFER:
        distTriggerModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::TIMER_MODIFIER:
        timerModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_TIMER_MODIFIER:
        bsTimerModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER:
        ragdollContactListenerModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_DEACTIVATE_MODIFIER:
        eventOnDeactivateModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_PASS_BY_TARGET_TRIGGER_MODIFIER:
        passByTargetTriggerModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_LOOK_AT_MODIFIER:
        bsLookAtModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DETECT_CLOSE_TO_GROUND_MODIFIER:
        detectCloseToGroundModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_EVERY_N_EVENTS_MODIFIER:
        eventEveryNEventsModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER:
        eventOnFalseToTrueModUI->eventRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVENTS_FROM_RANGE_MODIFIER:
        eventsFromRangeModUI->eventRenamed(newName, index);
        break;
    }
}

void HkDataUI::eventAdded(const QString & name){
    eventsTable->addItem(name, "hkEvent");
}

void HkDataUI::eventRemoved(int index){
    index++;
    eventsTable->removeItem(index);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->eventRenamed("NONE", index);
        break;
    /*case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
            cyclicBlendTransGenUI->eventRenamed(newName, index);
            break;*/
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_EXPRESSION_MODIFIER:
        evaluateExpModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::SENSE_HANDLE_MODIFIER:
        senseHandleModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_DIST_TRIGGER_MODIFER:
        distTriggerModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::TIMER_MODIFIER:
        timerModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_TIMER_MODIFIER:
        bsTimerModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER:
        ragdollContactListenerModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_DEACTIVATE_MODIFIER:
        eventOnDeactivateModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_PASS_BY_TARGET_TRIGGER_MODIFIER:
        passByTargetTriggerModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_LOOK_AT_MODIFIER:
        bsLookAtModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DETECT_CLOSE_TO_GROUND_MODIFIER:
        detectCloseToGroundModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_EVERY_N_EVENTS_MODIFIER:
        eventEveryNEventsModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER:
        eventOnFalseToTrueModUI->eventRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVENTS_FROM_RANGE_MODIFIER:
        eventsFromRangeModUI->eventRenamed("NONE", index);
        break;
    }
}

void HkDataUI::animationNameChanged(const QString &newName, int index){
    index++;
    animationsTable->renameItem(index, newName);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->animationRenamed(newName, index);
        break;
    }
}

void HkDataUI::animationAdded(const QString &name){
    animationsTable->addItem(name, "hkStringPtr");
}

void HkDataUI::animationRemoved(int index){
    index++;
    eventsTable->removeItem(index);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->animationRenamed("/", index);
        break;
    }
}

void HkDataUI::variableNameChanged(const QString & newName, int index){
    index++;
    variablesTable->renameItem(index, newName);
    switch (stack->currentIndex()) {
    case DATA_TYPE_LOADED::BLENDER_GENERATOR:
        blenderGeneratorUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_I_STATE_TAG_GEN:
        iStateTagGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_LIMB_IK_MOD:
        limbIKModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_BONE_SWITCH_GENERATOR:
        boneSwitchUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_OFFSET_ANIMATION_GENERATOR:
        offsetAnimGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        cyclicBlendTransGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::SYNCHRONIZED_CLIP_GENERATOR:
        syncClipGenUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DIRECT_AT_MODIFIER:
        directAtModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MOVE_CHARACTER_MODIFIER:
        moveCharModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::ROTATE_CHARACTER_MODIFIER:
        rotateCharModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_EXPRESSION_MODIFIER:
        evaluateExpModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MODIFIER_LIST:
        modListUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::GET_HANDLE_ON_BONE_MODIFIER:
        getHandleOnBoneUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_HANDLE_MODIFIER:
        evaluateHandleModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::SENSE_HANDLE_MODIFIER:
        senseHandleModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_DECOMPOSE_VECTOR_MODIFIER:
        decomposeVectorModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_IS_ACTIVE_MODIFIER:
        isActiveModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_DIRECTION_MODIFIER:
        computeDirMod->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_COMPUTE_ADD_BONE_ANIM_MODIFIER:
        computeAddAnimUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_DIST_TRIGGER_MODIFER:
        distTriggerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_INTERP_VALUE_MODIFIER:
        interpValueModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::GET_UP_MODIFIER:
        getUpModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::GET_WORLD_FROM_MODEL_MODIFIER:
        getWorldFromModelModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::TWIST_MODIFIER:
        twistModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::TIMER_MODIFIER:
        timerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DAMPING_MODIFIER:
        dampingModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER:
        rigidRagdollControlsModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::POWERED_RAGDOLL_CONTROLS_MODIFIER:
        poweredRagdollControlsModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::COMBINE_TRANSFORMS_MODIFIER:
        combineTransModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER:
        computeRotationAxisAngleModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_ROTATION_TO_TARGET_MODIFIER:
        computeRotationToTargetModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::TRANSFORM_VECTOR_MODIFIER:
        transformVectorModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::LOOK_AT_MODIFIER:
        lookAtModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::KEY_FRAME_BONES_MODIFIER:
        keyframeBonesModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::FOOT_IK_CONTROLS_MODIFIER:
        footIKControlsModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::MIRROR_MODIFIER:
        mirrorModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EXTRACT_RAGDOLL_POSE_MODIFIER:
        extractRagdollPoseModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_TIMER_MODIFIER:
        bsTimerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_GET_TIME_STEP_MODIFIER:
        getTimeStepModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DELAYED_MODIFIER:
        delayedModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER:
        ragdollContactListenerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_DEACTIVATE_MODIFIER:
        eventOnDeactivateModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_SPEED_SAMPLER_MODIFIER:
        speedSamplerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_PASS_BY_TARGET_TRIGGER_MODIFIER:
        passByTargetTriggerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_LOOK_AT_MODIFIER:
        bsLookAtModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::DETECT_CLOSE_TO_GROUND_MODIFIER:
        detectCloseToGroundModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_EVERY_N_EVENTS_MODIFIER:
        eventEveryNEventsModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER:
        eventOnFalseToTrueModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_MODIFY_ONCE_MODIFIER:
        modifyOnceModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::HAND_IK_CONTROLS_MODIFIER:
        handIKControlsModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::BS_TWEENER_MODIFIER:
        tweenerModUI->variableRenamed(newName, index);
        break;
    case DATA_TYPE_LOADED::EVENTS_FROM_RANGE_MODIFIER:
        eventsFromRangeModUI->variableRenamed(newName, index);
        break;
    }
}

void HkDataUI::variableAdded(const QString & name, const QString & type){
    variablesTable->addItem(name, type);
}

void HkDataUI::variableRemoved(int index){
    index++;
    variablesTable->removeItem(index);
    switch (stack->currentIndex()){
    case DATA_TYPE_LOADED::BLENDER_GENERATOR:
        blenderGeneratorUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_BONE_SWITCH_GENERATOR:
        boneSwitchUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::STATE_MACHINE:
        stateMachineUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::MANUAL_SELECTOR_GENERATOR:
        manSelGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_I_STATE_TAG_GEN:
        iStateTagGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_LIMB_IK_MOD:
        limbIKModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_OFFSET_ANIMATION_GENERATOR:
        offsetAnimGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        cyclicBlendTransGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR:
        poseMatchGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::CLIP_GENERATOR:
        clipGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::SYNCHRONIZED_CLIP_GENERATOR:
        syncClipGenUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DIRECT_AT_MODIFIER:
        directAtModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::MOVE_CHARACTER_MODIFIER:
        moveCharModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::ROTATE_CHARACTER_MODIFIER:
        rotateCharModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_EXPRESSION_MODIFIER:
        evaluateExpModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::MODIFIER_LIST:
        modListUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER:
        eventDrivenModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::GET_HANDLE_ON_BONE_MODIFIER:
        getHandleOnBoneUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVALUATE_HANDLE_MODIFIER:
        evaluateHandleModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::SENSE_HANDLE_MODIFIER:
        senseHandleModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_DECOMPOSE_VECTOR_MODIFIER:
        decomposeVectorModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_IS_ACTIVE_MODIFIER:
        isActiveModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_DIRECTION_MODIFIER:
        computeDirMod->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_COMPUTE_ADD_BONE_ANIM_MODIFIER:
        computeAddAnimUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_DIST_TRIGGER_MODIFER:
        distTriggerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_INTERP_VALUE_MODIFIER:
        interpValueModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::GET_UP_MODIFIER:
        getUpModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::GET_WORLD_FROM_MODEL_MODIFIER:
        getWorldFromModelModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::TWIST_MODIFIER:
        twistModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::TIMER_MODIFIER:
        timerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DAMPING_MODIFIER:
        dampingModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER:
        rigidRagdollControlsModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::POWERED_RAGDOLL_CONTROLS_MODIFIER:
        poweredRagdollControlsModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::COMBINE_TRANSFORMS_MODIFIER:
        combineTransModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER:
        computeRotationAxisAngleModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::COMPUTE_ROTATION_TO_TARGET_MODIFIER:
        computeRotationToTargetModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::TRANSFORM_VECTOR_MODIFIER:
        transformVectorModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::LOOK_AT_MODIFIER:
        lookAtModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::KEY_FRAME_BONES_MODIFIER:
        keyframeBonesModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::FOOT_IK_CONTROLS_MODIFIER:
        footIKControlsModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::MIRROR_MODIFIER:
        mirrorModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EXTRACT_RAGDOLL_POSE_MODIFIER:
        extractRagdollPoseModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_TIMER_MODIFIER:
        bsTimerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_GET_TIME_STEP_MODIFIER:
        getTimeStepModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DELAYED_MODIFIER:
        delayedModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER:
        ragdollContactListenerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_DEACTIVATE_MODIFIER:
        eventOnDeactivateModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_SPEED_SAMPLER_MODIFIER:
        speedSamplerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_PASS_BY_TARGET_TRIGGER_MODIFIER:
        passByTargetTriggerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_LOOK_AT_MODIFIER:
        bsLookAtModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::DETECT_CLOSE_TO_GROUND_MODIFIER:
        detectCloseToGroundModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_EVERY_N_EVENTS_MODIFIER:
        eventEveryNEventsModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER:
        eventOnFalseToTrueModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_MODIFY_ONCE_MODIFIER:
        modifyOnceModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::HAND_IK_CONTROLS_MODIFIER:
        handIKControlsModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::BS_TWEENER_MODIFIER:
        tweenerModUI->variableRenamed("NONE", index);
        break;
    case DATA_TYPE_LOADED::EVENTS_FROM_RANGE_MODIFIER:
        eventsFromRangeModUI->variableRenamed("NONE", index);
        break;
    }
    behaviorView->behavior->removeBindings(index);
    behaviorView->removeOtherData();
}

void HkDataUI::changeCurrentDataWidget(TreeGraphicsItem * icon){
    HkxSignature sig;
    if (icon && (icon->itemData)){
        HkxObject *oldData = loadedData;
        sig = ((HkxObject *)((icon->itemData)))->getSignature();
        loadedData = ((HkxObject *)((icon->itemData)));
        switch (sig){
        case HkxSignature::HKB_BLENDER_GENERATOR:
            if (loadedData != oldData){
                blenderGeneratorUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BLENDER_GENERATOR);
            blenderGeneratorUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_BONE_SWITCH_GENERATOR:
            if (loadedData != oldData){
                boneSwitchUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_BONE_SWITCH_GENERATOR);
            boneSwitchUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_MODIFIER_GENERATOR:
            if (loadedData != oldData){
                modGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::MODIFIER_GENERATOR);
            modGenUI->connectToTables(modifiersTable, generatorsTable);
            break;
        case HkxSignature::BS_I_STATE_TAGGING_GENERATOR:
            if (loadedData != oldData){
                iStateTagGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_I_STATE_TAG_GEN);
            iStateTagGenUI->connectToTables(variablesTable, characterPropertiesTable, generatorsTable);
            break;
        case HkxSignature::BS_LIMB_IK_MODIFIER:
            if (loadedData != oldData){
                limbIKModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_LIMB_IK_MOD);
            limbIKModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_MANUAL_SELECTOR_GENERATOR:
            if (loadedData != oldData){
                manSelGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::MANUAL_SELECTOR_GENERATOR);
            manSelGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_OFFSET_ANIMATION_GENERATOR:
            if (loadedData != oldData){
                offsetAnimGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_OFFSET_ANIMATION_GENERATOR);
            offsetAnimGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_STATE_MACHINE:
            if (loadedData != oldData){
                stateMachineUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::STATE_MACHINE);
            stateMachineUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
            if (loadedData != oldData){
                cyclicBlendTransGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_CYCLIC_BLEND_TRANSITION_GENERATOR);
            cyclicBlendTransGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_POSE_MATCHING_GENERATOR:
            if (loadedData != oldData){
                poseMatchGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::POSE_MATCHING_GENERATOR);
            poseMatchGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_CLIP_GENERATOR:
            if (loadedData != oldData){
                clipGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::CLIP_GENERATOR);
            clipGenUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable, animationsTable);
            break;
        case HkxSignature::BS_SYNCHRONIZED_CLIP_GENERATOR:
            if (loadedData != oldData){
                syncClipGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::SYNCHRONIZED_CLIP_GENERATOR);
            syncClipGenUI->connectToTables(generatorsTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_BEHAVIOR_REFERENCE_GENERATOR:
            if (loadedData != oldData){
                behaviorRefGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BEHAVIOR_REFERENCE_GENERATOR);
            break;
        case HkxSignature::HKB_BEHAVIOR_GRAPH:
            if (loadedData != oldData){
                behaviorGraphUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BEHAVIOR_GRAPH);
            behaviorGraphUI->connectToTables(generatorsTable);
            break;
        case HkxSignature::BS_DIRECT_AT_MODIFIER:
            if (loadedData != oldData){
                directAtModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::DIRECT_AT_MODIFIER);
            directAtModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_MOVE_CHARACTER_MODIFIER:
            if (loadedData != oldData){
                moveCharModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::MOVE_CHARACTER_MODIFIER);
            moveCharModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_ROTATE_CHARACTER_MODIFIER:
            if (loadedData != oldData){
                rotateCharModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::ROTATE_CHARACTER_MODIFIER);
            rotateCharModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_EVALUATE_EXPRESSION_MODIFIER:
            if (loadedData != oldData){
                evaluateExpModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::EVALUATE_EXPRESSION_MODIFIER);
            evaluateExpModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_MODIFIER_LIST:
            if (loadedData != oldData){
                modListUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::MODIFIER_LIST);
            modListUI->connectToTables(modifiersTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_EVENT_DRIVEN_MODIFIER:
            if (loadedData != oldData){
                eventDrivenModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::EVENT_DRIVEN_MODIFIER);
            eventDrivenModUI->connectToTables(modifiersTable, variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_GET_HANDLE_ON_BONE_MODIFIER:
            if (loadedData != oldData){
                getHandleOnBoneUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::GET_HANDLE_ON_BONE_MODIFIER);
            getHandleOnBoneUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_EVALUATE_HANDLE_MODIFIER:
            if (loadedData != oldData){
                evaluateHandleModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::EVALUATE_HANDLE_MODIFIER);
            evaluateHandleModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_SENSE_HANDLE_MODIFIER:
            if (loadedData != oldData){
                senseHandleModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::SENSE_HANDLE_MODIFIER);
            senseHandleModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_DECOMPOSE_VECTOR_MODIFIER:
            if (loadedData != oldData){
                decomposeVectorModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_DECOMPOSE_VECTOR_MODIFIER);
            decomposeVectorModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_IS_ACTIVE_MODIFIER:
            if (loadedData != oldData){
                isActiveModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_IS_ACTIVE_MODIFIER);
            isActiveModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_COMPUTE_DIRECTION_MODIFIER:
            if (loadedData != oldData){
                computeDirMod->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::COMPUTE_DIRECTION_MODIFIER);
            computeDirMod->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_COMPUTE_ADD_BONE_ANIM_MODIFIER:
            if (loadedData != oldData){
                computeAddAnimUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_COMPUTE_ADD_BONE_ANIM_MODIFIER);
            computeAddAnimUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_DIST_TRIGGER_MODIFER:
            if (loadedData != oldData){
                distTriggerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_DIST_TRIGGER_MODIFER);
            distTriggerModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_INTERP_VALUE_MODIFIER:
            if (loadedData != oldData){
                interpValueModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_INTERP_VALUE_MODIFIER);
            interpValueModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_GET_UP_MODIFIER:
            if (loadedData != oldData){
                getUpModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::GET_UP_MODIFIER);
            getUpModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_GET_WORLD_FROM_MODEL_MODIFIER:
            if (loadedData != oldData){
                getWorldFromModelModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::GET_WORLD_FROM_MODEL_MODIFIER);
            getWorldFromModelModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_TWIST_MODIFIER:
            if (loadedData != oldData){
                twistModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::TWIST_MODIFIER);
            twistModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_TIMER_MODIFIER:
            if (loadedData != oldData){
                timerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::TIMER_MODIFIER);
            timerModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_DAMPING_MODIFIER:
            if (loadedData != oldData){
                dampingModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::DAMPING_MODIFIER);
            dampingModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER:
            if (loadedData != oldData){
                rigidRagdollControlsModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER);
            rigidRagdollControlsModUI->connectToTables(variablesTable, characterPropertiesTable, ragdollBonesTable);
            break;
        case HkxSignature::HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER:
            if (loadedData != oldData){
                poweredRagdollControlsModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::POWERED_RAGDOLL_CONTROLS_MODIFIER);
            poweredRagdollControlsModUI->connectToTables(variablesTable, characterPropertiesTable, ragdollBonesTable);
            break;
        case HkxSignature::HKB_COMBINE_TRANSFORMS_MODIFIER:
            if (loadedData != oldData){
                combineTransModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::COMBINE_TRANSFORMS_MODIFIER);
            combineTransModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER:
            if (loadedData != oldData){
                computeRotationAxisAngleModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER);
            computeRotationAxisAngleModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_COMPUTE_ROTATION_TO_TARGET_MODIFIER:
            if (loadedData != oldData){
                computeRotationToTargetModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::COMPUTE_ROTATION_TO_TARGET_MODIFIER);
            computeRotationToTargetModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_TRANSFORM_VECTOR_MODIFIER:
            if (loadedData != oldData){
                transformVectorModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::TRANSFORM_VECTOR_MODIFIER);
            transformVectorModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_LOOK_AT_MODIFIER:
            if (loadedData != oldData){
                lookAtModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::LOOK_AT_MODIFIER);
            lookAtModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_KEY_FRAME_BONES_MODIFIER:
            if (loadedData != oldData){
                keyframeBonesModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::KEY_FRAME_BONES_MODIFIER);
            keyframeBonesModUI->connectToTables(variablesTable, characterPropertiesTable, ragdollBonesTable);
            break;
        case HkxSignature::HKB_FOOT_IK_CONTROLS_MODIFIER:
            if (loadedData != oldData){
                footIKControlsModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::FOOT_IK_CONTROLS_MODIFIER);
            footIKControlsModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_MIRROR_MODIFIER:
            if (loadedData != oldData){
                mirrorModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::MIRROR_MODIFIER);
            mirrorModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_EXTRACT_RAGDOLL_POSE_MODIFIER:
            if (loadedData != oldData){
                extractRagdollPoseModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::EXTRACT_RAGDOLL_POSE_MODIFIER);
            extractRagdollPoseModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_TIMER_MODIFIER:
            if (loadedData != oldData){
                bsTimerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_TIMER_MODIFIER);
            bsTimerModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_GET_TIME_STEP_MODIFIER:
            if (loadedData != oldData){
                getTimeStepModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_GET_TIME_STEP_MODIFIER);
            getTimeStepModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_DELAYED_MODIFIER:
            if (loadedData != oldData){
                delayedModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::DELAYED_MODIFIER);
            delayedModUI->connectToTables(modifiersTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER:
            if (loadedData != oldData){
                ragdollContactListenerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_RAGDOLL_CONTACT_LISTENER_MODIFIER);
            ragdollContactListenerModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable, ragdollBonesTable);
            break;
        case HkxSignature::BS_EVENT_ON_DEACTIVATE_MODIFIER:
            if (loadedData != oldData){
                eventOnDeactivateModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_EVENT_ON_DEACTIVATE_MODIFIER);
            eventOnDeactivateModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_SPEED_SAMPLER_MODIFIER:
            if (loadedData != oldData){
                speedSamplerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_SPEED_SAMPLER_MODIFIER);
            speedSamplerModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_PASS_BY_TARGET_TRIGGER_MODIFIER:
            if (loadedData != oldData){
                passByTargetTriggerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_PASS_BY_TARGET_TRIGGER_MODIFIER);
            passByTargetTriggerModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_LOOK_AT_MODIFIER:
            if (loadedData != oldData){
                bsLookAtModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_LOOK_AT_MODIFIER);
            bsLookAtModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::HKB_DETECT_CLOSE_TO_GROUND_MODIFIER:
            if (loadedData != oldData){
                detectCloseToGroundModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::DETECT_CLOSE_TO_GROUND_MODIFIER);
            detectCloseToGroundModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_EVENT_EVERY_N_EVENTS_MODIFIER:
            if (loadedData != oldData){
                eventEveryNEventsModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_EVENT_EVERY_N_EVENTS_MODIFIER);
            eventEveryNEventsModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER:
            if (loadedData != oldData){
                eventOnFalseToTrueModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER);
            eventOnFalseToTrueModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        case HkxSignature::BS_MODIFY_ONCE_MODIFIER:
            if (loadedData != oldData){
                modifyOnceModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_MODIFY_ONCE_MODIFIER);
            modifyOnceModUI->connectToTables(modifiersTable, variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_HAND_IK_CONTROLS_MODIFIER:
            if (loadedData != oldData){
                handIKControlsModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::HAND_IK_CONTROLS_MODIFIER);
            handIKControlsModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BS_TWEENER_MODIFIER:
            if (loadedData != oldData){
                tweenerModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BS_TWEENER_MODIFIER);
            tweenerModUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::BGS_GAMEBYRO_SEQUENCE_GENERATOR:
            if (loadedData != oldData){
                gamebryoSequenceGenUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::BGS_GAMEBYRO_SEQUENCE_GENERATOR);
            //gamebryoSequenceGenUI->connectToTables(variablesTable, characterPropertiesTable);
            break;
        case HkxSignature::HKB_EVENTS_FROM_RANGE_MODIFIER:
            if (loadedData != oldData){
                eventsFromRangeModUI->loadData(loadedData);
            }
            stack->setCurrentIndex(DATA_TYPE_LOADED::EVENTS_FROM_RANGE_MODIFIER);
            eventsFromRangeModUI->connectToTables(variablesTable, characterPropertiesTable, eventsTable);
            break;
        default:
            unloadDataWidget();
        }
    }else{
        unloadDataWidget();
    }
}

BehaviorGraphView *HkDataUI::loadBehaviorView(BehaviorGraphView *view){
    BehaviorGraphView *oldView = behaviorView;
    if (oldView){
        //disconnect(oldView, 0, this, 0);
    }
    if (parentWidget()){
        setMinimumSize(parentWidget()->size()*0.99);
    }
    behaviorView = view;
    iStateTagGenUI->setBehaviorView(view);
    modGenUI->setBehaviorView(view);
    manSelGenUI->setBehaviorView(view);
    stateMachineUI->setBehaviorView(view);
    blenderGeneratorUI->setBehaviorView(view);
    behaviorGraphUI->setBehaviorView(view);
    boneSwitchUI->setBehaviorView(view);
    offsetAnimGenUI->setBehaviorView(view);
    cyclicBlendTransGenUI->setBehaviorView(view);
    poseMatchGenUI->setBehaviorView(view);
    syncClipGenUI->setBehaviorView(view);
    modListUI->setBehaviorView(view);
    eventDrivenModUI->setBehaviorView(view);
    delayedModUI->setBehaviorView(view);
    modifyOnceModUI->setBehaviorView(view);
    if (behaviorView){
        generatorsTable->loadTable(behaviorView->behavior->getGeneratorNames(), behaviorView->behavior->getGeneratorTypeNames(), "NULL");
        modifiersTable->loadTable(behaviorView->behavior->getModifierNames(), behaviorView->behavior->getModifierTypeNames(), "NULL");
        variablesTable->loadTable(behaviorView->behavior->getVariableNames(), behaviorView->behavior->getVariableTypenames(), "NONE");
        animationsTable->loadTable(behaviorView->behavior->getAnimationNames(), "hkStringPtr", "NONE");//inefficient...
        eventsTable->loadTable(behaviorView->behavior->getEventNames(), "hkEvent", "NONE");
        ragdollBonesTable->loadTable(behaviorView->behavior->getRagdollBoneNames(), "hkInt32", "NONE");//inefficient...
        characterPropertiesTable->loadTable(behaviorView->behavior->getCharacterPropertyNames(), behaviorView->behavior->getCharacterPropertyTypenames(), "NONE");//inefficient...
        connect(behaviorView, SIGNAL(addedGenerator(QString,QString)), this, SLOT(generatorAdded(QString,QString)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(addedModifier(QString,QString)), this, SLOT(modifierAdded(QString,QString)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(removedGenerator(int)), this, SLOT(generatorRemoved(int)), Qt::UniqueConnection);
        connect(behaviorView, SIGNAL(removedModifier(int)), this, SLOT(modifierRemoved(int)), Qt::UniqueConnection);
    }
    return oldView;
}
