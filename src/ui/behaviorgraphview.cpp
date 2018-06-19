#include "behaviorgraphview.h"
#include "src/ui/mainwindow.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/ui/behaviorgraphview.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/treegraphicsscene.h"
#include "src/ui/hkxclassesui/hkdataui.h"

#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/bscyclicblendtransitiongenerator.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/behavior/generators/bssynchronizedclipgenerator.h"
#include "src/hkxclasses/behavior/generators/bsoffsetanimationgenerator.h"
#include "src/hkxclasses/behavior/generators/bgsgamebryosequencegenerator.h"

#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/generators/hkbmanualselectorgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/hkxclasses/behavior/generators/hkbposematchinggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorreferencegenerator.h"

#include "src/hkxclasses/behavior/modifiers/bsisactivemodifier.h"
#include "src/hkxclasses/behavior/modifiers/bslimbikmodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsspeedsamplermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bslookatmodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsmodifyoncemodifier.h"
#include "src/hkxclasses/behavior/modifiers/bseventonfalsetotruemodifier.h"
#include "src/hkxclasses/behavior/modifiers/bseventondeactivatemodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsragdollcontactlistenermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsdirectatmodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsdecomposevectormodifier.h"
#include "src/hkxclasses/behavior/modifiers/bscomputeaddboneanimmodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsdisttriggermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bseventeveryneventsmodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsinterpvaluemodifier.h"
#include "src/hkxclasses/behavior/modifiers/bspassbytargettriggermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bstimermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bstweenermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsistatemanagermodifier.h"
#include "src/hkxclasses/behavior/modifiers/bsgettimestepmodifier.h"

#include "src/hkxclasses/behavior/modifiers/hkbmodifierlist.h"
#include "src/hkxclasses/behavior/modifiers/hkbtwistmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbeventdrivenmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbfootikcontrolsmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbevaluateexpressionmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbrotatecharactermodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbdampingmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbkeyframebonesmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbpoweredragdollcontrolsmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbtimermodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbrigidbodyragdollcontrolsmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbgetupmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbcomputedirectionmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbevaluatehandlemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbgethandleonbonemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbsensehandlemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbdelayedmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbdetectclosetogroundmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbattachmentmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbattributemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbcombinetransformsmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbcomputerotationfromaxisanglemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbcomputerotationtotargetmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbeventsfromrangemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbextractragdollposemodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbgetworldfrommodelmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkblookatmodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbmirrormodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbmovecharactermodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbtransformvectormodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbproxymodifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbhandikcontrolsmodifier.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbgeneratortransitioneffect.h"
#include "src/hkxclasses/behavior/hkbeventrangedataarray.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/hkxclasses/behavior/hkbexpressiondataarray.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/behavior/hkbexpressioncondition.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"
#include "src/hkxclasses/behavior/hkbcliptriggerarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/behavior/hkbvariablevalueset.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

/**
 * BehaviorGraphView
 */

BehaviorGraphView::BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file)
    : TreeGraphicsView(0),
      ui(mainUI),
      behavior(file),
      contextMenu(new QMenu(this)),
      appendGeneratorMenu(new QMenu("Append Generator:", contextMenu)),
      appendStateMachineAct(new QAction("State Machine", appendGeneratorMenu)),
      appendStateAct(new QAction("State", appendGeneratorMenu)),
      appendManualSelectorGeneratorAct(new QAction("Manual Selector Generator", appendGeneratorMenu)),
      appendModifierGeneratorAct(new QAction("Modifier Generator", appendGeneratorMenu)),
      appendBSIStateTaggingGeneratorAct(new QAction("BS iState Tagging Generator", appendGeneratorMenu)),
      appendBSSynchronizedClipGeneratorAct(new QAction("BS Synchronized Clip Generator", appendGeneratorMenu)),
      appendBSOffsetAnimationGeneratorAct(new QAction("BS Offset Animation Generator", appendGeneratorMenu)),
      appendBSCyclicBlendTransitionGeneratorAct(new QAction("BS Cyclic Blend Transition Generator", appendGeneratorMenu)),
      appendPoseMatchingGeneratorAct(new QAction("Pose Matching Generator", appendGeneratorMenu)),
      appendClipGeneratorAct(new QAction("Clip Generator", appendGeneratorMenu)),
      appendBehaviorReferenceGeneratorAct(new QAction("Behavior Reference Generator", appendGeneratorMenu)),
      appendBGSGamebryoSequenceGeneratorAct(new QAction("BGS Gamebryo Sequence Generator", appendGeneratorMenu)),
      appendBlenderMenu(new QMenu("Append Blend:", contextMenu)),
      appendBlenderGeneratorAct(new QAction("Blender Generator", appendBlenderMenu)),
      appendBlenderGeneratorChildAct(new QAction("Blender Generator Child", appendBlenderMenu)),
      appendBSBoneSwitchGeneratorAct(new QAction("BS Bone Switch Generator", appendBlenderMenu)),
      appendBSBoneSwitchGeneratorChildAct(new QAction("BS Bone Switch Generator Child", appendBlenderMenu)),
      wrapGeneratorMenu(new QMenu("Wrap inside Generator:", contextMenu)),
      wrapStateMachineAct(new QAction("State Machine", wrapGeneratorMenu)),
      wrapManualSelectorGeneratorAct(new QAction("Manual Selector Generator", wrapGeneratorMenu)),
      wrapModifierGeneratorAct(new QAction("Modifier Generator", wrapGeneratorMenu)),
      wrapBSIStateTaggingGeneratorAct(new QAction("BS iState Tagging Generator", wrapGeneratorMenu)),
      wrapBSSynchronizedClipGeneratorAct(new QAction("BS Synchronized Clip Generator", wrapGeneratorMenu)),
      wrapBSOffsetAnimationGeneratorAct(new QAction("BS Offset Animation Generator", wrapGeneratorMenu)),
      wrapBSCyclicBlendTransitionGeneratorAct(new QAction("BS Cyclic Blend Transition Generator", wrapGeneratorMenu)),
      wrapPoseMatchingGeneratorAct(new QAction("Pose Matching Generator", wrapGeneratorMenu)),
      wrapBlenderMenu(new QMenu("Wrap inside Blend:", contextMenu)),
      wrapBlenderGeneratorAct(new QAction("Blender Generator", wrapBlenderMenu)),
      wrapBSBoneSwitchGeneratorAct(new QAction("BS Bone Switch Generator", wrapBlenderMenu)),
      appendModifierMenu(new QMenu("Append Modifier:", contextMenu)),
      appendModifierListAct(new QAction("Modifier List", appendModifierMenu)),
      appendTwistModifierAct(new QAction("Twist Modifier", appendModifierMenu)),
      appendEventDrivenModifierAct(new QAction("Event Driven Modifier", appendModifierMenu)),
      appendIsActiveModifierAct(new QAction("BS Is Active Modifier", appendModifierMenu)),
      appendLimbIKModifierAct(new QAction("BS Limb IK Modifier", appendModifierMenu)),
      appendInterpValueModifierAct(new QAction("BS Interp Value Modifier", appendModifierMenu)),
      appendGetTimeStepModifierAct(new QAction("BS Get Time Step Modifier", appendModifierMenu)),
      appendFootIkControlsModifierAct(new QAction("Foot IK Controls Modifier", appendModifierMenu)),
      appendGetHandleOnBoneModifierAct(new QAction("Get Handle On Bone Modifier", appendModifierMenu)),
      appendTransformVectorModifierAct(new QAction("Transform Vector Modifier", appendModifierMenu)),
      appendProxyModifierAct(new QAction("Proxy Modifier", appendModifierMenu)),
      appendLookAtModifierAct(new QAction("Look At Modifier", appendModifierMenu)),
      appendMirrorModifierAct(new QAction("Mirror Modifier", appendModifierMenu)),
      appendGetWorldFromModelModifierAct(new QAction("Get World From Model Modifier", appendModifierMenu)),
      appendSenseHandleModifierAct(new QAction("Sense Handle Modifier", appendModifierMenu)),
      appendEvaluateExpressionModifierAct(new QAction("Evaluate Expression Modifier", appendModifierMenu)),
      appendEvaluateHandleModifierAct(new QAction("Evaluate Handle Modifier", appendModifierMenu)),
      appendAttachmentModifierAct(new QAction("Attachment Modifier", appendModifierMenu)),
      appendAttributeModifierAct(new QAction("AttributeModifier", appendModifierMenu)),
      appendCombineTransformsModifierAct(new QAction("Combine Transforms Modifier", appendModifierMenu)),
      appendComputeRotationFromAxisAngleModifierAct(new QAction("Compute Rotation From Axis Angle Modifier", appendModifierMenu)),
      appendComputeRotationToTargetModifierAct(new QAction("Compute Rotation To Target Modifier", appendModifierMenu)),
      appendEventsFromRangeModifierAct(new QAction("Events From Range Modifier", appendModifierMenu)),
      appendMoveCharacterModifierAct(new QAction("Move Character Modifier", appendModifierMenu)),
      appendExtractRagdollPoseModifierAct(new QAction("Extract Ragdoll Pose Modifier", appendModifierMenu)),
      appendModifyOnceModifierAct(new QAction("BS Modify Once Modifier", appendModifierMenu)),
      appendEventOnDeactivateModifierAct(new QAction("BS Event On Deactivate Modifier", appendModifierMenu)),
      appendEventEveryNEventsModifierAct(new QAction("BS Event Every N Events Modifier", appendModifierMenu)),
      appendRagdollContactListenerModifierAct(new QAction("Ragdoll Contact Listener Modifier", appendModifierMenu)),
      appendPoweredRagdollControlsModifierAct(new QAction("Powered Ragdoll Controls Modifier", appendModifierMenu)),
      appendEventOnFalseToTrueModifierAct(new QAction("BS Event On False To True Modifier", appendModifierMenu)),
      appendDirectAtModifierAct(new QAction("BS Direct At Modifier", appendModifierMenu)),
      appendDistTriggerModifierAct(new QAction("BS Dist Trigger Modifier", appendModifierMenu)),
      appendDecomposeVectorModifierAct(new QAction("BS Decompose Vector Modifier", appendModifierMenu)),
      appendComputeAddBoneAnimModifierAct(new QAction("BS Compute Add Bone Anim Modifier", appendModifierMenu)),
      appendTweenerModifierAct(new QAction("BS Tweener Modifier", appendModifierMenu)),
      appendBSIStateManagerModifierAct(new QAction("BS I State Manager Modifier", appendModifierMenu)),
      appendTimerModifierAct(new QAction("Timer Modifier", appendModifierMenu)),
      appendRotateCharacterModifierAct(new QAction("Rotate Character Modifier", appendModifierMenu)),
      appendDampingModifierAct(new QAction("Damping Modifier", appendModifierMenu)),
      appendDelayedModifierAct(new QAction("Delayed Modifier", appendModifierMenu)),
      appendGetUpModifierAct(new QAction("Get Up Modifier", appendModifierMenu)),
      appendKeyframeBonesModifierAct(new QAction("Keyframe Bones Modifier", appendModifierMenu)),
      appendComputeDirectionModifierAct(new QAction("Compute Direction Modifier", appendModifierMenu)),
      appendRigidBodyRagdollControlsModifierAct(new QAction("Rigid Body Ragdoll Controls Modifier", appendModifierMenu)),
      appendBSSpeedSamplerModifierAct(new QAction("BS Speed Sampler Modifier", appendModifierMenu)),
      appendDetectCloseToGroundModifierAct(new QAction("Detect Close To Ground Modifier", appendModifierMenu)),
      appendBSTimerModifierAct(new QAction("BS Timer Modifier", appendModifierMenu)),
      appendBSLookAtModifierAct(new QAction("BS Look At Modifier", appendModifierMenu)),
      appendBSPassByTargetTriggerModifierAct(new QAction("BS Pass By Target Trigger Modifier", appendModifierMenu)),
      appendHandIKControlsModifierAct(new QAction("Hand IK Controls Modifier", appendModifierMenu)),
      removeObjBranchAct(new QAction("Remove Selected Object and Children", contextMenu))
{
    contextMenu->addMenu(appendGeneratorMenu);
    appendGeneratorMenu->addAction(appendStateMachineAct);
    appendGeneratorMenu->addAction(appendStateAct);
    appendGeneratorMenu->addAction(appendManualSelectorGeneratorAct);
    appendGeneratorMenu->addAction(appendModifierGeneratorAct);
    appendGeneratorMenu->addAction(appendBSIStateTaggingGeneratorAct);
    appendGeneratorMenu->addAction(appendBSSynchronizedClipGeneratorAct);
    appendGeneratorMenu->addAction(appendBSOffsetAnimationGeneratorAct);
    appendGeneratorMenu->addAction(appendBSCyclicBlendTransitionGeneratorAct);
    appendGeneratorMenu->addAction(appendPoseMatchingGeneratorAct);
    appendGeneratorMenu->addAction(appendClipGeneratorAct);
    appendGeneratorMenu->addAction(appendBehaviorReferenceGeneratorAct);
    appendGeneratorMenu->addAction(appendBGSGamebryoSequenceGeneratorAct);
    contextMenu->addMenu(appendBlenderMenu);
    appendBlenderMenu->addAction(appendBlenderGeneratorAct);
    appendBlenderMenu->addAction(appendBlenderGeneratorChildAct);
    appendBlenderMenu->addAction(appendBSBoneSwitchGeneratorAct);
    appendBlenderMenu->addAction(appendBSBoneSwitchGeneratorChildAct);
    contextMenu->addMenu(wrapGeneratorMenu);
    wrapGeneratorMenu->addAction(wrapStateMachineAct);
    wrapGeneratorMenu->addAction(wrapManualSelectorGeneratorAct);
    wrapGeneratorMenu->addAction(wrapModifierGeneratorAct);
    wrapGeneratorMenu->addAction(wrapBSIStateTaggingGeneratorAct);
    wrapGeneratorMenu->addAction(wrapBSSynchronizedClipGeneratorAct);
    wrapGeneratorMenu->addAction(wrapBSOffsetAnimationGeneratorAct);
    wrapGeneratorMenu->addAction(wrapBSCyclicBlendTransitionGeneratorAct);
    wrapGeneratorMenu->addAction(wrapPoseMatchingGeneratorAct);
    contextMenu->addMenu(wrapBlenderMenu);
    wrapBlenderMenu->addAction(wrapBlenderGeneratorAct);
    wrapBlenderMenu->addAction(wrapBSBoneSwitchGeneratorAct);
    contextMenu->addMenu(appendModifierMenu);
    appendModifierMenu->addAction(appendModifierListAct);
    appendModifierMenu->addAction(appendTwistModifierAct);
    appendModifierMenu->addAction(appendEventDrivenModifierAct);
    appendModifierMenu->addAction(appendIsActiveModifierAct);
    appendModifierMenu->addAction(appendLimbIKModifierAct);
    appendModifierMenu->addAction(appendInterpValueModifierAct);
    appendModifierMenu->addAction(appendGetTimeStepModifierAct);
    appendModifierMenu->addAction(appendFootIkControlsModifierAct);
    appendModifierMenu->addAction(appendGetHandleOnBoneModifierAct);
    appendModifierMenu->addAction(appendTransformVectorModifierAct);
    appendModifierMenu->addAction(appendProxyModifierAct);
    appendModifierMenu->addAction(appendLookAtModifierAct);
    appendModifierMenu->addAction(appendMirrorModifierAct);
    appendModifierMenu->addAction(appendGetWorldFromModelModifierAct);
    appendModifierMenu->addAction(appendSenseHandleModifierAct);
    appendModifierMenu->addAction(appendEvaluateExpressionModifierAct);
    appendModifierMenu->addAction(appendEvaluateHandleModifierAct);
    appendModifierMenu->addAction(appendAttachmentModifierAct);
    appendModifierMenu->addAction(appendAttributeModifierAct);
    appendModifierMenu->addAction(appendCombineTransformsModifierAct);
    appendModifierMenu->addAction(appendComputeRotationFromAxisAngleModifierAct);
    appendModifierMenu->addAction(appendComputeRotationToTargetModifierAct);
    appendModifierMenu->addAction(appendEventsFromRangeModifierAct);
    appendModifierMenu->addAction(appendMoveCharacterModifierAct);
    appendModifierMenu->addAction(appendExtractRagdollPoseModifierAct);
    appendModifierMenu->addAction(appendModifyOnceModifierAct);
    appendModifierMenu->addAction(appendEventOnDeactivateModifierAct);
    appendModifierMenu->addAction(appendEventEveryNEventsModifierAct);
    appendModifierMenu->addAction(appendRagdollContactListenerModifierAct);
    appendModifierMenu->addAction(appendPoweredRagdollControlsModifierAct);
    appendModifierMenu->addAction(appendEventOnFalseToTrueModifierAct);
    appendModifierMenu->addAction(appendDirectAtModifierAct);
    appendModifierMenu->addAction(appendDistTriggerModifierAct);
    appendModifierMenu->addAction(appendDecomposeVectorModifierAct);
    appendModifierMenu->addAction(appendComputeAddBoneAnimModifierAct);
    appendModifierMenu->addAction(appendTweenerModifierAct);
    appendModifierMenu->addAction(appendBSIStateManagerModifierAct);
    appendModifierMenu->addAction(appendTimerModifierAct);
    appendModifierMenu->addAction(appendRotateCharacterModifierAct);
    appendModifierMenu->addAction(appendDampingModifierAct);
    appendModifierMenu->addAction(appendDelayedModifierAct);
    appendModifierMenu->addAction(appendGetUpModifierAct);
    appendModifierMenu->addAction(appendKeyframeBonesModifierAct);
    appendModifierMenu->addAction(appendComputeDirectionModifierAct);
    appendModifierMenu->addAction(appendRigidBodyRagdollControlsModifierAct);
    appendModifierMenu->addAction(appendBSSpeedSamplerModifierAct);
    appendModifierMenu->addAction(appendDetectCloseToGroundModifierAct);
    appendModifierMenu->addAction(appendBSTimerModifierAct);
    appendModifierMenu->addAction(appendBSLookAtModifierAct);
    appendModifierMenu->addAction(appendBSPassByTargetTriggerModifierAct);
    appendModifierMenu->addAction(appendHandIKControlsModifierAct);
    contextMenu->addAction(removeObjBranchAct);
    //setContextMenu(contextMenu);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(appendStateMachineAct, SIGNAL(triggered()), this, SLOT(appendStateMachine()));
    connect(appendStateAct, SIGNAL(triggered()), this, SLOT(appendState()));
    connect(appendManualSelectorGeneratorAct, SIGNAL(triggered()), this, SLOT(appendManualSelectorGenerator()));
    connect(appendModifierGeneratorAct, SIGNAL(triggered()), this, SLOT(appendModifierGenerator()));
    connect(appendBSIStateTaggingGeneratorAct, SIGNAL(triggered()), this, SLOT(appendIStateTaggingGenerator()));
    connect(appendBSSynchronizedClipGeneratorAct, SIGNAL(triggered()), this, SLOT(appendSynchronizedClipGenerator()));
    connect(appendBSOffsetAnimationGeneratorAct, SIGNAL(triggered()), this, SLOT(appendOffsetAnimationGenerator()));
    connect(appendBSCyclicBlendTransitionGeneratorAct, SIGNAL(triggered()), this, SLOT(appendCyclicBlendTransitionGenerator()));
    connect(appendPoseMatchingGeneratorAct, SIGNAL(triggered()), this, SLOT(appendPoseMatchingGenerator()));
    connect(appendBlenderGeneratorAct, SIGNAL(triggered()), this, SLOT(appendBlenderGenerator()));
    connect(appendBlenderGeneratorChildAct, SIGNAL(triggered()), this, SLOT(appendBlenderGeneratorChild()));
    connect(appendBSBoneSwitchGeneratorAct, SIGNAL(triggered()), this, SLOT(appendBoneSwitchGenerator()));
    connect(appendBSBoneSwitchGeneratorChildAct, SIGNAL(triggered()), this, SLOT(appendBoneSwitchGeneratorChild()));
    connect(appendClipGeneratorAct, SIGNAL(triggered()), this, SLOT(appendClipGenerator()));
    connect(appendBehaviorReferenceGeneratorAct, SIGNAL(triggered()), this, SLOT(appendBehaviorReferenceGenerator()));
    connect(appendBGSGamebryoSequenceGeneratorAct, SIGNAL(triggered()), this, SLOT(appendBGSGamebryoSequenceGenerator()));
    connect(wrapManualSelectorGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapManualSelectorGenerator()));
    connect(wrapModifierGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapModifierGenerator()));
    connect(wrapBSIStateTaggingGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapIStateTaggingGenerator()));
    connect(wrapBSSynchronizedClipGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapSynchronizedClipGenerator()));
    connect(wrapBSOffsetAnimationGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapOffsetAnimationGenerator()));
    connect(wrapBSCyclicBlendTransitionGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapCyclicBlendTransitionGenerator()));
    connect(wrapBSBoneSwitchGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapBoneSwitchGenerator()));
    connect(wrapStateMachineAct, SIGNAL(triggered()), this, SLOT(wrapStateMachine()));
    connect(wrapBlenderGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapBlenderGenerator()));
    connect(wrapPoseMatchingGeneratorAct, SIGNAL(triggered()), this, SLOT(wrapPoseMatchingGenerator()));
    connect(appendModifierListAct, SIGNAL(triggered()), this, SLOT(appendModifierList()));
    connect(appendTwistModifierAct, SIGNAL(triggered()), this, SLOT(appendTwistModifier()));
    connect(appendEventDrivenModifierAct, SIGNAL(triggered()), this, SLOT(appendEventDrivenModifier()));
    connect(appendIsActiveModifierAct, SIGNAL(triggered()), this, SLOT(appendIsActiveModifier()));
    connect(appendLimbIKModifierAct, SIGNAL(triggered()), this, SLOT(appendLimbIKModifier()));
    connect(appendInterpValueModifierAct, SIGNAL(triggered()), this, SLOT(appendInterpValueModifier()));
    connect(appendGetTimeStepModifierAct, SIGNAL(triggered()), this, SLOT(appendGetTimeStepModifier()));
    connect(appendFootIkControlsModifierAct, SIGNAL(triggered()), this, SLOT(appendFootIkControlsModifier()));
    connect(appendGetHandleOnBoneModifierAct, SIGNAL(triggered()), this, SLOT(appendGetHandleOnBoneModifier()));
    connect(appendTransformVectorModifierAct, SIGNAL(triggered()), this, SLOT(appendTransformVectorModifier()));
    connect(appendProxyModifierAct, SIGNAL(triggered()), this, SLOT(appendProxyModifier()));
    connect(appendLookAtModifierAct, SIGNAL(triggered()), this, SLOT(appendLookAtModifier()));
    connect(appendMirrorModifierAct, SIGNAL(triggered()), this, SLOT(appendMirrorModifier()));
    connect(appendGetWorldFromModelModifierAct, SIGNAL(triggered()), this, SLOT(appendGetWorldFromModelModifier()));
    connect(appendSenseHandleModifierAct, SIGNAL(triggered()), this, SLOT(appendSenseHandleModifier()));
    connect(appendEvaluateExpressionModifierAct, SIGNAL(triggered()), this, SLOT(appendEvaluateExpressionModifier()));
    connect(appendEvaluateHandleModifierAct, SIGNAL(triggered()), this, SLOT(appendEvaluateHandleModifier()));
    connect(appendAttachmentModifierAct, SIGNAL(triggered()), this, SLOT(appendAttachmentModifier()));
    connect(appendAttributeModifierAct, SIGNAL(triggered()), this, SLOT(appendAttributeModifier()));
    connect(appendCombineTransformsModifierAct, SIGNAL(triggered()), this, SLOT(appendCombineTransformsModifier()));
    connect(appendComputeRotationFromAxisAngleModifierAct, SIGNAL(triggered()), this, SLOT(appendComputeRotationFromAxisAngleModifier()));
    connect(appendComputeRotationToTargetModifierAct, SIGNAL(triggered()), this, SLOT(appendComputeRotationToTargetModifier()));
    connect(appendEventsFromRangeModifierAct, SIGNAL(triggered()), this, SLOT(appendEventsFromRangeModifier()));
    connect(appendMoveCharacterModifierAct, SIGNAL(triggered()), this, SLOT(appendMoveCharacterModifier()));
    connect(appendExtractRagdollPoseModifierAct, SIGNAL(triggered()), this, SLOT(appendExtractRagdollPoseModifier()));
    connect(appendModifyOnceModifierAct, SIGNAL(triggered()), this, SLOT(appendModifyOnceModifier()));
    connect(appendEventOnDeactivateModifierAct, SIGNAL(triggered()), this, SLOT(appendEventOnDeactivateModifier()));
    connect(appendEventEveryNEventsModifierAct, SIGNAL(triggered()), this, SLOT(appendEventEveryNEventsModifier()));
    connect(appendRagdollContactListenerModifierAct, SIGNAL(triggered()), this, SLOT(appendRagdollContactListenerModifier()));
    connect(appendPoweredRagdollControlsModifierAct, SIGNAL(triggered()), this, SLOT(appendPoweredRagdollControlsModifier()));
    connect(appendEventOnFalseToTrueModifierAct, SIGNAL(triggered()), this, SLOT(appendEventOnFalseToTrueModifier()));
    connect(appendDirectAtModifierAct, SIGNAL(triggered()), this, SLOT(appendDirectAtModifier()));
    connect(appendDistTriggerModifierAct, SIGNAL(triggered()), this, SLOT(appendDistTriggerModifier()));
    connect(appendDecomposeVectorModifierAct, SIGNAL(triggered()), this, SLOT(appendDecomposeVectorModifier()));
    connect(appendComputeAddBoneAnimModifierAct, SIGNAL(triggered()), this, SLOT(appendComputeAddBoneAnimModifier()));
    connect(appendTweenerModifierAct, SIGNAL(triggered()), this, SLOT(appendTweenerModifier()));
    connect(appendBSIStateManagerModifierAct, SIGNAL(triggered()), this, SLOT(appendBSIStateManagerModifier()));
    connect(appendTimerModifierAct, SIGNAL(triggered()), this, SLOT(appendTimerModifier()));
    connect(appendRotateCharacterModifierAct, SIGNAL(triggered()), this, SLOT(appendRotateCharacterModifier()));
    connect(appendDampingModifierAct, SIGNAL(triggered()), this, SLOT(appendDampingModifier()));
    connect(appendDelayedModifierAct, SIGNAL(triggered()), this, SLOT(appendDelayedModifier()));
    connect(appendGetUpModifierAct, SIGNAL(triggered()), this, SLOT(appendGetUpModifier()));
    connect(appendKeyframeBonesModifierAct, SIGNAL(triggered()), this, SLOT(appendKeyframeBonesModifier()));
    connect(appendComputeDirectionModifierAct, SIGNAL(triggered()), this, SLOT(appendComputeDirectionModifier()));
    connect(appendRigidBodyRagdollControlsModifierAct, SIGNAL(triggered()), this, SLOT(appendRigidBodyRagdollControlsModifier()));
    connect(appendBSSpeedSamplerModifierAct, SIGNAL(triggered()), this, SLOT(appendBSSpeedSamplerModifier()));
    connect(appendDetectCloseToGroundModifierAct, SIGNAL(triggered()), this, SLOT(appendDetectCloseToGroundModifier()));
    connect(appendBSTimerModifierAct, SIGNAL(triggered()), this, SLOT(appendBSTimerModifier()));
    connect(appendBSLookAtModifierAct, SIGNAL(triggered()), this, SLOT(appendBSLookAtModifier()));
    connect(appendHandIKControlsModifierAct, SIGNAL(triggered()), this, SLOT(appendHandIKControlsModifier()));
    connect(appendBSPassByTargetTriggerModifierAct, SIGNAL(triggered()), this, SLOT(appendBSPassByTargetTriggerModifier()));
    connect(removeObjBranchAct, SIGNAL(triggered()), this, SLOT(deleteSelectedObjectBranchSlot()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popUpMenuRequested(QPoint)));
}

bool BehaviorGraphView::confirmationDialogue(const QString & message, QWidget *parent){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(parent, "Skyrim Behavior Tool", message, QMessageBox::Yes | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes){
        return true;
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return false;
}

TreeGraphicsItem * BehaviorGraphView::getSelectedIconsChildIcon(HkxObject *child){
    if (getSelectedItem()){
        return getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(child));
    }
    return nullptr;
}

QStringList BehaviorGraphView::getEventNames() const{
    return behavior->getEventNames();
}

void BehaviorGraphView::removeGeneratorData(){
    QVector <int> removedIndices;
    removedIndices = behavior->removeGeneratorData();
    for (int i = 0; i < removedIndices.size(); i++){
        emit removedGenerator(removedIndices.at(i) + 1);
    }
}

void BehaviorGraphView::removeModifierData(){
    QVector <int> removedIndices = behavior->removeModifierData();
    for (int i = 0; i < removedIndices.size(); i++){
        emit removedModifier(removedIndices.at(i) + 1);
    }
}

void BehaviorGraphView::removeOtherData(){
    behavior->removeOtherData();
}

bool BehaviorGraphView::refocus(){
    if (getSelectedItem()){
        //resetMatrix();
        //scale(1, 1);
        centerOn(getSelectedItem());
        return true;
    }else{
        return false;
    }
}

void BehaviorGraphView::setSelectedItem(TreeGraphicsItem *item){
    if (/*ui && */scene()){
        static_cast<TreeGraphicsScene *>(scene())->selectIcon(item, TreeGraphicsScene::EXPAND_CONTRACT_ONE);
        //ui->changeCurrentDataWidget(item);
    }
}

QString BehaviorGraphView::getBehaviorFilename() const{
    if (behavior){
        return behavior->fileName();
    }
    return "";
}

void BehaviorGraphView::focusOnGeneratorIcon(int index, const QString &name){
    if (behavior){
        behavior->setFocusGeneratorIcon(index);
    }else{
        LogFile::writeToLog("BehaviorGraphView::focusOnGeneratorIcon(): No behavior file!");
    }
    emit disconnectTablesFromHkDataUI();
}

void BehaviorGraphView::focusOnModifierIcon(int index, const QString &name){
    if (behavior){
        behavior->setFocusModifierIcon(index);
    }else{
        LogFile::writeToLog("BehaviorGraphView::focusOnModifierIcon(): No behavior file!");
    }
    emit disconnectTablesFromHkDataUI();
}

void BehaviorGraphView::deleteSelectedObjectBranchSlot(){
    if (getSelectedItem()){
        getSelectedItem()->unselect();
        removeItemFromGraph(getSelectedItem(), 0, true, true);
        ui->changeCurrentDataWidget(nullptr);
        removeObjects();
    }
}

void BehaviorGraphView::removeObjects(){
    removeGeneratorData();
    //behavior->removeGeneratorChildrenData();
    //removeGeneratorData();//Inefficient...
    removeModifierData();
    behavior->removeOtherData();
    behavior->setIsChanged(true);
}

void BehaviorGraphView::deleteAllObjectBranches(){
    hkbBehaviorGraph *graph = static_cast<hkbBehaviorGraph *>(behavior->behaviorGraph.data());
    if (!graph->icons.isEmpty()){
        QList <QGraphicsItem *> list = graph->icons.first()->childItems();
        if (!list.isEmpty()){
            TreeGraphicsItem *root = static_cast<TreeGraphicsItem *>(list.first());
            root->unselect();
            removeItemFromGraph(root, 0, true, true);
            ui->changeCurrentDataWidget(nullptr);
            removeObjects();
        }
    }
}

template <typename T>
void BehaviorGraphView::append(T *obj){
    if (getSelectedItem()){
        TreeGraphicsItem *newIcon = nullptr;
        DataIconManager *selectedItemData = static_cast<DataIconManager *>(getSelectedItem()->itemData);
        if (selectedItemData){
            HkxSignature sig = selectedItemData->getSignature();
            if (selectedItemData->hasChildren() && sig != HKB_STATE_MACHINE && sig != HKB_MANUAL_SELECTOR_GENERATOR && sig != HKB_BLENDER_GENERATOR && sig != BS_BONE_SWITCH_GENERATOR && sig != HKB_POSE_MATCHING_GENERATOR && sig != HKB_MODIFIER_LIST){
                /*if (sig == HKB_MODIFIER_GENERATOR){
                    if (selectedItemData->getIndexOfObj(nullptr) == 0 && obj->getType() == HkxObject::TYPE_MODIFIER && !confirmationDialogue("WARNING! THIS WILL REPLACE THE CURRENT MODIFIER!!!\n\nARE YOU SURE YOU WANT TO DO THIS?", this)){
                        behavior->removeModifierData();
                    }else if (selectedItemData->getIndexOfObj(nullptr) == 1 && obj->getType() == HkxObject::TYPE_GENERATOR && !confirmationDialogue("WARNING! THIS WILL REPLACE THE CURRENT MODIFIER!!!\n\nARE YOU SURE YOU WANT TO DO THIS?", this)){
                        behavior->removeGeneratorData();
                    }
                    return;
                }else{*/
                    if (!confirmationDialogue("WARNING! THIS WILL REPLACE THE CURRENT GENERATOR/MODIFIER!!!\n\nARE YOU SURE YOU WANT TO DO THIS?", this)){
                        if (obj->getType() == HkxObject::TYPE_GENERATOR){
                            behavior->removeGeneratorData();
                        }else if (obj->getType() == HkxObject::TYPE_MODIFIER){
                            behavior->removeModifierData();
                        }/*else if (obj->getType() == HkxObject::TYPE_OTHER){
                            behavior->removeOtherData();
                        }*/
                        return;
                    //}
                }
                if (!getSelectedItem()->childItems().isEmpty()){
                    if (sig == HKB_MODIFIER_GENERATOR){
                        if (getSelectedItem()->childItems().size() == 2 && obj->getType() == HkxObject::TYPE_GENERATOR){
                            removeItemFromGraph(((TreeGraphicsItem *)getSelectedItem()->childItems()[1]), 1);
                        }else if (obj->getType() == HkxObject::TYPE_MODIFIER){
                            if (((TreeGraphicsItem *)getSelectedItem()->childItems().first())->itemData->getType() == HkxObject::TYPE_MODIFIER){
                                removeItemFromGraph(((TreeGraphicsItem *)getSelectedItem()->childItems().first()), 0);
                                newIcon = addItemToGraph(getSelectedItem(), obj, 0);
                            }else{
                                newIcon = addItemToGraph(getSelectedItem(), obj, 0);
                            }
                        }
                    }else{
                        removeItemFromGraph(((TreeGraphicsItem *)getSelectedItem()->childItems().first()), 0);
                    }
                }
            }
            if (!newIcon){
                newIcon = addItemToGraph(getSelectedItem(), obj, -1);
            }
            if (((TreeGraphicsItem *)getSelectedItem()->parentItem())){
                ((HkxObject *)((TreeGraphicsItem *)getSelectedItem()->parentItem())->itemData)->evaluateDataValidity();
            }
            selectedItemData->evaluateDataValidity();
            behavior->setIsChanged(true);
            getSelectedItem()->reposition();
            treeScene->selectIcon(newIcon, TreeGraphicsScene::EXPAND_CONTRACT_ZERO);    //Removing this breaks adding child with generator!!!
            QRectF rect = sceneRect();
            setSceneRect(rect.marginsAdded(QMarginsF(0, 0, newIcon->boundingRect().width()*2, newIcon->boundingRect().height()*2)));
            if (obj->getType() == HkxObject::TYPE_GENERATOR){
                emit addedGenerator(obj->getName(), obj->getClassname());
            }else if (obj->getType() == HkxObject::TYPE_MODIFIER){
                emit addedModifier(obj->getName(), obj->getClassname());
            }
        }else{
            delete obj;
            CRITICAL_ERROR_MESSAGE("BehaviorGraphView::append(): \n The selected icon has no data!!");
        }
    }
}

void BehaviorGraphView::appendStateMachine(){
    append(new hkbStateMachine(behavior));
}

void BehaviorGraphView::appendState(){
    append(new hkbStateMachineStateInfo(behavior, (hkbStateMachine *)getSelectedData()));
}

void BehaviorGraphView::appendManualSelectorGenerator(){
    append(new hkbManualSelectorGenerator(behavior));
}

void BehaviorGraphView::appendModifierGenerator(){
    append(new hkbModifierGenerator(behavior));
}

void BehaviorGraphView::appendIStateTaggingGenerator(){
    append(new BSiStateTaggingGenerator(behavior));
}

void BehaviorGraphView::appendSynchronizedClipGenerator(){
    append(new BSSynchronizedClipGenerator(behavior));
}

void BehaviorGraphView::appendOffsetAnimationGenerator(){
    append(new BSOffsetAnimationGenerator(behavior));
}

void BehaviorGraphView::appendCyclicBlendTransitionGenerator(){
    append(new BSCyclicBlendTransitionGenerator(behavior));
}

void BehaviorGraphView::appendPoseMatchingGenerator(){
    append(new hkbPoseMatchingGenerator(behavior));
}

void BehaviorGraphView::appendBlenderGenerator(){
    append(new hkbBlenderGenerator(behavior));
}

void BehaviorGraphView::appendBlenderGeneratorChild(){
    append(new hkbBlenderGeneratorChild(behavior, (hkbBlenderGenerator *)getSelectedData()));
}

void BehaviorGraphView::appendBoneSwitchGenerator(){
    append(new BSBoneSwitchGenerator(behavior));
}

void BehaviorGraphView::appendBoneSwitchGeneratorChild(){
    append(new BSBoneSwitchGeneratorBoneData(behavior, (BSBoneSwitchGenerator *)getSelectedData()));
}

void BehaviorGraphView::appendClipGenerator(){
    append(new hkbClipGenerator(behavior, 0, true));
}

void BehaviorGraphView::appendBehaviorReferenceGenerator(){
    append(new hkbBehaviorReferenceGenerator(behavior));
}

void BehaviorGraphView::appendBGSGamebryoSequenceGenerator(){
    append(new BGSGamebryoSequenceGenerator(behavior));
}

void BehaviorGraphView::appendModifierList(){
    append(new hkbModifierList(behavior));
}

void BehaviorGraphView::appendTwistModifier(){
    append(new hkbTwistModifier(behavior));
}

void BehaviorGraphView::appendEventDrivenModifier(){
    append(new hkbEventDrivenModifier(behavior));
}

void BehaviorGraphView::appendIsActiveModifier(){
    append(new BSIsActiveModifier(behavior));
}

void BehaviorGraphView::appendLimbIKModifier(){
    append(new BSLimbIKModifier(behavior));
}

void BehaviorGraphView::appendInterpValueModifier(){
    append(new BSInterpValueModifier(behavior));
}

void BehaviorGraphView::appendGetTimeStepModifier(){
    append(new BSGetTimeStepModifier(behavior));
}

void BehaviorGraphView::appendFootIkControlsModifier(){
    append(new hkbFootIkControlsModifier(behavior));
}

void BehaviorGraphView::appendGetHandleOnBoneModifier(){
    append(new hkbGetHandleOnBoneModifier(behavior));
}

void BehaviorGraphView::appendTransformVectorModifier(){
    append(new hkbTransformVectorModifier(behavior));
}

void BehaviorGraphView::appendProxyModifier(){
    append(new hkbProxyModifier(behavior));
}

void BehaviorGraphView::appendLookAtModifier(){
    append(new hkbLookAtModifier(behavior));
}

void BehaviorGraphView::appendMirrorModifier(){
    append(new hkbMirrorModifier(behavior));
}

void BehaviorGraphView::appendGetWorldFromModelModifier(){
    append(new hkbGetWorldFromModelModifier(behavior));
}

void BehaviorGraphView::appendSenseHandleModifier(){
    append(new hkbSenseHandleModifier(behavior));
}

void BehaviorGraphView::appendEvaluateExpressionModifier(){
    append(new hkbEvaluateExpressionModifier(behavior));
}

void BehaviorGraphView::appendEvaluateHandleModifier(){
    append(new hkbEvaluateHandleModifier(behavior));
}

void BehaviorGraphView::appendAttachmentModifier(){
    append(new hkbAttachmentModifier(behavior));
}

void BehaviorGraphView::appendAttributeModifier(){
    append(new hkbAttributeModifier(behavior));
}

void BehaviorGraphView::appendCombineTransformsModifier(){
    append(new hkbCombineTransformsModifier(behavior));
}

void BehaviorGraphView::appendComputeRotationFromAxisAngleModifier(){
    append(new hkbComputeRotationFromAxisAngleModifier(behavior));
}

void BehaviorGraphView::appendComputeRotationToTargetModifier(){
    append(new hkbComputeRotationToTargetModifier(behavior));
}

void BehaviorGraphView::appendEventsFromRangeModifier(){
    append(new hkbEventsFromRangeModifier(behavior));
}

void BehaviorGraphView::appendMoveCharacterModifier(){
    append(new hkbMoveCharacterModifier(behavior));
}

void BehaviorGraphView::appendExtractRagdollPoseModifier(){
    append(new hkbExtractRagdollPoseModifier(behavior));
}

void BehaviorGraphView::appendModifyOnceModifier(){
    append(new BSModifyOnceModifier(behavior));
}

void BehaviorGraphView::appendEventOnDeactivateModifier(){
    append(new BSEventOnDeactivateModifier(behavior));
}

void BehaviorGraphView::appendEventEveryNEventsModifier(){
    append(new BSEventEveryNEventsModifier(behavior));
}

void BehaviorGraphView::appendRagdollContactListenerModifier(){
    append(new BSRagdollContactListenerModifier(behavior));
}

void BehaviorGraphView::appendPoweredRagdollControlsModifier(){
    append(new hkbPoweredRagdollControlsModifier(behavior));
}

void BehaviorGraphView::appendEventOnFalseToTrueModifier(){
    append(new BSEventOnFalseToTrueModifier(behavior));
}

void BehaviorGraphView::appendDirectAtModifier(){
    append(new BSDirectAtModifier(behavior));
}

void BehaviorGraphView::appendDistTriggerModifier(){
    append(new BSDistTriggerModifier(behavior));
}

void BehaviorGraphView::appendDecomposeVectorModifier(){
    append(new BSDecomposeVectorModifier(behavior));
}

void BehaviorGraphView::appendComputeAddBoneAnimModifier(){
    append(new BSComputeAddBoneAnimModifier(behavior));
}

void BehaviorGraphView::appendTweenerModifier(){
    append(new BSTweenerModifier(behavior));
}

void BehaviorGraphView::appendBSIStateManagerModifier(){
    append(new BSIStateManagerModifier(behavior));
}

void BehaviorGraphView::appendTimerModifier(){
    append(new hkbTimerModifier(behavior));
}

void BehaviorGraphView::appendRotateCharacterModifier(){
    append(new hkbRotateCharacterModifier(behavior));
}

void BehaviorGraphView::appendDampingModifier(){
    append(new hkbDampingModifier(behavior));
}

void BehaviorGraphView::appendDelayedModifier(){
    append(new hkbDelayedModifier(behavior));
}

void BehaviorGraphView::appendGetUpModifier(){
    append(new hkbGetUpModifier(behavior));
}

void BehaviorGraphView::appendKeyframeBonesModifier(){
    append(new hkbKeyframeBonesModifier(behavior));
}

void BehaviorGraphView::appendComputeDirectionModifier(){
    append(new hkbComputeDirectionModifier(behavior));
}

void BehaviorGraphView::appendRigidBodyRagdollControlsModifier(){
    append(new hkbRigidBodyRagdollControlsModifier(behavior));
}

void BehaviorGraphView::appendBSSpeedSamplerModifier(){
    append(new BSSpeedSamplerModifier(behavior));
}

void BehaviorGraphView::appendDetectCloseToGroundModifier(){
    append(new hkbDetectCloseToGroundModifier(behavior));
}

void BehaviorGraphView::appendBSTimerModifier(){
    append(new BSTimerModifier(behavior));
}

void BehaviorGraphView::appendBSLookAtModifier(){
    append(new BSLookAtModifier(behavior));
}

void BehaviorGraphView::appendBSPassByTargetTriggerModifier(){
    append(new BSPassByTargetTriggerModifier(behavior));
}

void BehaviorGraphView::appendHandIKControlsModifier(){
    append(new hkbHandIkControlsModifier(behavior));
}

template <typename T>
void BehaviorGraphView::wrap(T *obj){
    if (getSelectedItem() && ((TreeGraphicsItem *)getSelectedItem()->parentItem()) && ((TreeGraphicsItem *)getSelectedItem()->parentItem())->itemData){
        behavior->setIsChanged(true);
        TreeGraphicsItem *newIcon = addItemToGraph(getSelectedItem(), static_cast<DataIconManager*>((obj)), -1, true);
        behavior->setIsChanged(true);
        getSelectedItem()->reposition();
        treeScene->selectIcon(newIcon, TreeGraphicsScene::EXPAND_CONTRACT_ZERO);
        emit addedGenerator(obj->getName(), obj->getClassname());
    }
}

void BehaviorGraphView::wrapManualSelectorGenerator(){
    wrap(new hkbManualSelectorGenerator(behavior));
}

void BehaviorGraphView::wrapModifierGenerator(){
    wrap(new hkbModifierGenerator(behavior));
}

void BehaviorGraphView::wrapIStateTaggingGenerator(){
    wrap(new BSiStateTaggingGenerator(behavior));
}

void BehaviorGraphView::wrapSynchronizedClipGenerator(){
    wrap(new BSSynchronizedClipGenerator(behavior));
}

void BehaviorGraphView::wrapOffsetAnimationGenerator(){
    wrap(new BSOffsetAnimationGenerator(behavior));
}

void BehaviorGraphView::wrapCyclicBlendTransitionGenerator(){
    wrap(new BSCyclicBlendTransitionGenerator(behavior));
}

void BehaviorGraphView::wrapBoneSwitchGenerator(){
    BSBoneSwitchGenerator *bonegen = new BSBoneSwitchGenerator(behavior);
    wrap(new BSBoneSwitchGeneratorBoneData(behavior, bonegen));
    wrap(bonegen);
}

void BehaviorGraphView::wrapStateMachine(){
    hkbStateMachine *statemachine = new hkbStateMachine(behavior);
    wrap(new hkbStateMachineStateInfo(behavior, statemachine));
    wrap(statemachine);
}

void BehaviorGraphView::wrapBlenderGenerator(){
    hkbBlenderGenerator *blender = new hkbBlenderGenerator(behavior);
    wrap(new hkbBlenderGeneratorChild(behavior, blender));
    wrap(blender);
}

void BehaviorGraphView::wrapPoseMatchingGenerator(){
    hkbPoseMatchingGenerator *pose = new hkbPoseMatchingGenerator(behavior);
    wrap(new hkbBlenderGeneratorChild(behavior, pose));
    wrap(pose);
}

void BehaviorGraphView::enableAllMenuActions(QMenu *menu){
    QList <QAction *> actions = menu->actions();
    for (int i = 0; i < actions.size(); i++){
        actions.at(i)->setDisabled(false);
    }
}

void BehaviorGraphView::disableAllMenuActions(QMenu *menu){
    QList <QAction *> actions = menu->actions();
    for (int i = 0; i < actions.size(); i++){
        actions.at(i)->setDisabled(true);
    }
}

void BehaviorGraphView::popUpMenuRequested(const QPoint &pos){
    if (!getSelectedItem() || !getSelectedItem()->itemData){
        return;
    }
    HkxSignature sig = ((HkxObject *)(getSelectedItem()->itemData))->getSignature();
    HkxSignature parentSig = NULL_SIGNATURE;
    if (getSelectedItem()->parentItem()){
        parentSig = ((HkxObject *)(((TreeGraphicsItem *)(getSelectedItem()->parentItem()))->itemData))->getSignature();
    }
    HkxObject::HkxType type = ((HkxObject *)(getSelectedItem()->itemData))->getType();
    removeObjBranchAct->setDisabled(false);
    appendStateAct->setDisabled(true);
    if (type == HkxObject::TYPE_MODIFIER){
        appendGeneratorMenu->menuAction()->setDisabled(true);
        appendBlenderMenu->menuAction()->setDisabled(true);
        wrapGeneratorMenu->menuAction()->setDisabled(true);
        wrapBlenderMenu->menuAction()->setDisabled(true);
        if (sig == HKB_MODIFIER_LIST || sig == HKB_EVENT_DRIVEN_MODIFIER){
            appendModifierMenu->menuAction()->setDisabled(false);
        }else{
            appendModifierMenu->menuAction()->setDisabled(true);
        }/*else if (sig == HKB_MODIFIER_LIST){
            //enable action
        }*/
    }else if (sig == HKB_STATE_MACHINE){
        disableAllMenuActions(appendGeneratorMenu);
        appendBlenderMenu->menuAction()->setDisabled(true);
        appendModifierMenu->menuAction()->setDisabled(true);
        wrapGeneratorMenu->menuAction()->setDisabled(false);
        wrapBlenderMenu->menuAction()->setDisabled(false);
        appendStateAct->setDisabled(false);
    }else if (type == HkxObject::TYPE_GENERATOR){
        appendGeneratorMenu->menuAction()->setDisabled(false);
        enableAllMenuActions(appendGeneratorMenu);
        appendBlenderMenu->menuAction()->setDisabled(false);
        enableAllMenuActions(appendBlenderMenu);
        wrapGeneratorMenu->menuAction()->setDisabled(false);
        enableAllMenuActions(wrapGeneratorMenu);
        wrapBlenderMenu->menuAction()->setDisabled(false);
        enableAllMenuActions(wrapBlenderMenu);
        appendModifierMenu->menuAction()->setDisabled(true);
        appendStateAct->setDisabled(true);
        appendBlenderGeneratorChildAct->setDisabled(true);
        appendBSBoneSwitchGeneratorChildAct->setDisabled(true);
        if (sig == HKB_BLENDER_GENERATOR || sig == HKB_POSE_MATCHING_GENERATOR){
            disableAllMenuActions(appendGeneratorMenu);
            disableAllMenuActions(appendBlenderMenu);
            enableAllMenuActions(wrapGeneratorMenu);
            appendBlenderGeneratorChildAct->setDisabled(false);
        }else if (sig == BS_BONE_SWITCH_GENERATOR){
            enableAllMenuActions(wrapGeneratorMenu);
            if (static_cast<BSBoneSwitchGenerator *>(getSelectedItem()->itemData)->hasGenerator()){
                disableAllMenuActions(appendBlenderMenu);
                disableAllMenuActions(appendGeneratorMenu);
                appendBSBoneSwitchGeneratorChildAct->setDisabled(false);
            }else{
                enableAllMenuActions(appendGeneratorMenu);
            }
        }else if (sig == HKB_CLIP_GENERATOR || sig == HKB_BEHAVIOR_REFERENCE_GENERATOR || sig == BGS_GAMEBYRO_SEQUENCE_GENERATOR){
            appendGeneratorMenu->menuAction()->setDisabled(true);
            appendBlenderMenu->menuAction()->setDisabled(true);
            enableAllMenuActions(wrapGeneratorMenu);
        }else if (sig == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
            disableAllMenuActions(appendGeneratorMenu);
            appendBlenderGeneratorAct->setDisabled(false);
        }else if (sig == BS_SYNCHRONIZED_CLIP_GENERATOR){
            disableAllMenuActions(appendGeneratorMenu);
            appendClipGeneratorAct->setDisabled(false);
        }else if (sig == HKB_MODIFIER_GENERATOR){
            appendModifierMenu->menuAction()->setDisabled(false);
        }else if (sig == HKB_BEHAVIOR_GRAPH){
            disableAllMenuActions(appendGeneratorMenu);
            wrapGeneratorMenu->menuAction()->setDisabled(true);
            appendBlenderMenu->menuAction()->setDisabled(true);
            wrapBlenderMenu->menuAction()->setDisabled(true);
            appendStateMachineAct->setDisabled(false);
            removeObjBranchAct->setDisabled(true);
        }else if (sig == HKB_STATE_MACHINE_STATE_INFO || sig == HKB_BLENDER_GENERATOR_CHILD || sig == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            wrapGeneratorMenu->menuAction()->setDisabled(true);
            wrapBlenderMenu->menuAction()->setDisabled(true);
        }
        if (parentSig == BS_SYNCHRONIZED_CLIP_GENERATOR || parentSig == BS_OFFSET_ANIMATION_GENERATOR){
            wrapGeneratorMenu->menuAction()->setDisabled(true);
            wrapBlenderMenu->menuAction()->setDisabled(true);
        }else if (parentSig == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
            wrapGeneratorMenu->menuAction()->setDisabled(true);
            wrapBlenderGeneratorAct->setDisabled(false);
            wrapBSBoneSwitchGeneratorAct->setDisabled(true);
        }else if (parentSig == HKB_BEHAVIOR_GRAPH){
            wrapGeneratorMenu->menuAction()->setDisabled(true);
            wrapBlenderMenu->menuAction()->setDisabled(true);
        }
    }
    contextMenu->popup(viewport()->mapToGlobal(pos));
}

BehaviorGraphView::~BehaviorGraphView(){
    ui->loadBehaviorView(nullptr);
}
