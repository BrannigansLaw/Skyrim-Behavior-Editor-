#include "behaviorfile.h"
#include "characterfile.h"
#include "skeletonfile.h"
#include "projectfile.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/ui/mainwindow.h"
#include "src/ui/treegraphicsitem.h"

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
#include "src/hkxclasses/behavior/hkbcharacterstringdata.h"
#include "src/hkxclasses/behavior/hkbgeneratortransitioneffect.h"
#include "src/hkxclasses/behavior/hkbeventrangedataarray.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/hkxclasses/behavior/hkbexpressiondataarray.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/behavior/hkbexpressioncondition.h"
#include "src/hkxclasses/behavior/hkbstringcondition.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"
#include "src/hkxclasses/behavior/hkbcliptriggerarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/behavior/hkbvariablevalueset.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

#include <QStringList>

/**
 * Class: BehaviorFile
 *
 */

BehaviorFile::BehaviorFile(MainWindow *window, ProjectFile *projectfile, CharacterFile *characterData, const QString & name)
    : HkxFile(window, name),
      project(projectfile),
      character(characterData),
      largestRef(0)
{
    getReader().setFile(this);
    if (!project){
        FATAL_RUNTIME_ERROR("BehaviorFile::BehaviorFile(): The project pointer is nullptr!");
    }
    if (!character){
        FATAL_RUNTIME_ERROR("BehaviorFile::BehaviorFile(): The character pointer is nullptr!");
    }
}

void BehaviorFile::generateNewBehavior(){
    hkRootLevelContainer *root = new hkRootLevelContainer(this);
    hkbBehaviorGraph *graph = new hkbBehaviorGraph(this);
    graph->name = fileName().section("/", -1, -1);
    graph->name.replace(".hkx", ".hkb");
    behaviorGraph = HkxSharedPtr(graph);
    stringData = HkxSharedPtr(new hkbBehaviorGraphStringData(this));
    variableValues = HkxSharedPtr(new hkbVariableValueSet(this));
    hkbBehaviorGraphData *data = new hkbBehaviorGraphData(this);
    graphData = HkxSharedPtr(data);
    graph->data = graphData;
    data->stringData = stringData;
    data->variableInitialValues = variableValues;
    root->namedVariants.append(hkRootLevelContainer::hkRootLevelContainerNamedVariant());
    root->namedVariants.last().variant = behaviorGraph;
    setRootObject(HkxSharedPtr(root));
    setIsChanged(true);
}

void BehaviorFile::generateDefaultCharacterData(){
    generateNewBehavior();
    hkbBehaviorGraphData *data = static_cast<hkbBehaviorGraphData *>(graphData.data());
    if (data){
        //Add common variables...
        data->addVariable(VARIABLE_TYPE_BOOL, "bEquipOk");   //Needs to be initialized to one or custom creatures don't work...
        data->addVariable(VARIABLE_TYPE_REAL, "Speed"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_REAL, "Direction"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_INT32, "iState");
        data->addVariable(VARIABLE_TYPE_REAL, "TurnDelta"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_REAL, "SpeedSampled");
        data->addVariable(VARIABLE_TYPE_REAL, "SpeedDamped");
        data->addVariable(VARIABLE_TYPE_REAL, "TurnDeltaDamped");
        data->addVariable(VARIABLE_TYPE_REAL, "weaponSpeedMult"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_BOOL, "bAnimationDriven");
        data->addVariable(VARIABLE_TYPE_BOOL, "bAllowRotation");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsAttacking");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsStaggering");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsRecoiling");
        data->addVariable(VARIABLE_TYPE_BOOL, "bFootIKEnable");   //Creatures use this, humans don't...
        data->addVariable(VARIABLE_TYPE_BOOL, "bHumanoidFootIKEnable");   //Humans use this, creatures don't...
        data->addVariable(VARIABLE_TYPE_REAL, "staggerMagnitude");
        data->addVariable(VARIABLE_TYPE_REAL, "staggerDirection");
        data->addVariable(VARIABLE_TYPE_BOOL, "bHeadTrackingOn");   //Creatures use this, humans don't...
        data->addVariable(VARIABLE_TYPE_BOOL, "bHeadTracking"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_VECTOR4, "TargetLocation");
        data->addVariable(VARIABLE_TYPE_REAL, "turnSpeedMult");   //Creatures use this, humans???
        //Add common events...
        data->addEvent("moveStart");
        data->addEvent("moveStop");
        data->addEvent("bleedOutStart");
        data->addEvent("bleedOutStop");
        data->addEvent("blockAnticipateStart");
        data->addEvent("blockHitStart");
        //Listed in playercharacterresponse.txt
        data->addEvent("BackupResponse");	//ActorResponse
        data->addEvent("idleChairSitting");	//PlayerChairEnterHandler
        data->addEvent("idleBedSleeping");	//PlayerBedEnterHandler
        data->addEvent("idleChairGetUp");	//PlayerFurnitureExitHandler
        data->addEvent("idleBedGetUp");	//PlayerFurnitureExitHandler
        data->addEvent("BowZoomStart");	//BowZoomStartHandler
        data->addEvent("BowZoomStop");	//BowZoomStopHandler
        data->addEvent("StartAnimatedCamera");	//AnimatedCameraStartHandler
        data->addEvent("StartAnimatedCameraDelta");	//AnimatedCameraDeltaStartHandler
        data->addEvent("EndAnimatedCamera");	//AnimatedCameraEndHandler
        data->addEvent("GraphDeleting");	//AnimatedCameraEndHandler
        data->addEvent("PitchOverrideStart");	//PitchOverrideStartHandler
        data->addEvent("PitchOverrideEnd");	//PitchOverrideEndHandler
        data->addEvent("ZeroOutCameraPitch");	//ZeroPitchHandler
        //Listed in actorresponse.txt
        data->addEvent("weaponSwing");	//WeaponRightSwingHandler
        data->addEvent("weaponLeftSwing");	//WeaponLeftSwingHandler
        data->addEvent("AttackWinStart");	//AttackWinStartHandler
        data->addEvent("AttackWinStartLeft");	//AttackWinStartHandler
        data->addEvent("AttackWinEnd");	//AttackWinEndHandler
        data->addEvent("AttackWinEndLeft");	//AttackWinEndHandler
        data->addEvent("attackStop");	//AttackStopHandler
        data->addEvent("bashStop");	//AttackStopHandler
        data->addEvent("recoilStop");	//RecoilStopHandler
        data->addEvent("MLh_SpellFire_Event");	//LeftHandSpellFireHandler
        data->addEvent("MRh_SpellFire_Event");	//RightHandSpellFireHandler
        data->addEvent("Voice_SpellFire_Event");	//VoiceSpellFireHandler
        data->addEvent("BeginCastLeft");	//LeftHandSpellCastHandler
        data->addEvent("BeginCastRight");	//RightHandSpellCastHandler
        data->addEvent("BeginCastVoice");	//VoiceSpellCastHandler
        data->addEvent("BeginWeaponDraw");	//WeaponBeginDrawRightHandler
        data->addEvent("BeginWeaponSheathe");	//WeaponBeginSheatheRightHandler
        data->addEvent("CameraOverrideStart");	//CameraOverrideStartHandler
        data->addEvent("CameraOverrideStop");	//CameraOverrideStopHandler
        data->addEvent("weaponDraw");	//RightHandWeaponDrawHandler
        data->addEvent("weaponSheathe");	//RightHandWeaponSheatheHandler
        data->addEvent("HitFrame");	//HitFrameHandler
        data->addEvent("preHitFrame");	//AnticipateAttackHandler
        data->addEvent("staggerStop");	//StaggeredStopHandler
        data->addEvent("idleChairSitting");	//ChairEnterHandler
        data->addEvent("idleBedSleeping");	//BedEnterHandler
        data->addEvent("idleChairGetUp");	//ChairFurnitureExitHandler
        data->addEvent("idleBedGetUp");	//BedFurnitureExitHandler
        data->addEvent("idleSleepGetUp");	//BedFurnitureExitHandler
        data->addEvent("KillActor");	//KillActorHandler
        data->addEvent("Decapitate");	//DecapitateHandler
        data->addEvent("FlightTakeOff");	//FlightTakeOffHandler
        data->addEvent("FlightCruising");	//FlightCruisingHandler
        data->addEvent("FlightHovering");	//FlightHoveringHandler
        data->addEvent("FlightLanding");	//FlightLandingHandler
        data->addEvent("FlightPerching");	//FlightPerchingHandler
        data->addEvent("FlightLanded");	//FlightLandHandler
        data->addEvent("FlightLandEnd");	//FlightLandEndHandler
        data->addEvent("FlightAction");	//FlightActionHandler
        data->addEvent("FlightActionEntryEnd");	//FlightActionEntryEndHandler
        data->addEvent("FlightActionEnd");	//FlightActionEndHandler
        data->addEvent("FlightActionGrab");	//FlightActionGrabHandler
        data->addEvent("FlightActionRelease");	//FlightActionReleaseHandler
        data->addEvent("FlightCrashLandStart");	//FlightCrashLandStartHandler
        data->addEvent("HeadTrackingOn");	//HeadTrackingOnHandler
        data->addEvent("HeadTrackingOff");	//HeadTrackingOffHandler
        data->addEvent("BowDrawn");	//BowDrawnHandler
        data->addEvent("BowRelease");	//BowReleaseHandler
        data->addEvent("arrowAttach");	//ArrowAttachHandler
        data->addEvent("arrowDetach");	//ArrowDetachHandler
        data->addEvent("bowReset");	//ArrowDetachHandler
        data->addEvent("arrowRelease");	//ArrowReleaseHandler
        data->addEvent("InterruptCast");	//InterruptCastHandler
        data->addEvent("summonStop");	//EndSummonAnimationHandler
        data->addEvent("PickNewIdle");	//PickNewIdleHandler
        data->addEvent("DeathStop");	//DeathStopHandler
        data->addEvent("ActivationDone");	//ActionActivateDoneHandler
        data->addEvent("StopHorseCamera");	//StopHorseCameraHandler
        data->addEvent("KillMoveStart");	//KillMoveStartHandler
        data->addEvent("KillMoveEnd");	//KillMoveEndHandler
        data->addEvent("pairedStop");	//PairedStopHandler
        data->addEvent("CameraShake");	//CameraShakeHandler
        data->addEvent("DeathEmote");	//DeathEmoteHandler
        data->addEvent("StartMotionDriven");	//MotionDrivenHandler
        data->addEvent("StartAnimationDriven");	//AnimationDrivenHandler
        data->addEvent("StartAllowRotation");	//AllowRotationHandler
        data->addEvent("AddRagdollToWorld");	//AddRagdollHandler
        data->addEvent("RemoveRagdollFromWorld");	//RemoveRagdollHandler
        data->addEvent("RemoveCharacterControllerFromWorld");	//RemoveCharacterControllerHandler
        data->addEvent("GetUpStart");	//GetUpStartHandler
        data->addEvent("GetUpEnd");	//GetUpEndHandler
        data->addEvent("MountEnd");	//MountDismountEndHandler
        data->addEvent("DismountEnd");	//MountDismountEndHandler
        data->addEvent("ExitCartBegin");	//ExitCartBeginHandler
        data->addEvent("ExitCartEnd");	//ExitCartEndHandler
        data->addEvent("EnableBumper");	//EnableCharacterBumperHandler
        data->addEvent("DisableBumper");	//DisableCharacterBumperHandler
        data->addEvent("AnimObjLoad");	//AnimationObjectLoadHandler
        data->addEvent("AnimObjDraw");	//AnimationObjectDrawHandler
        data->addEvent("EnableCharacterPitch");	//EnableCharacterPitchHandler
        data->addEvent("DisableCharacterPitch");	//DisableCharacterPitchHandler
        data->addEvent("JumpBegin");	//JumpAnimEventHandler
        data->addEvent("IdleDialogueLock");	//IdleDialogueEnterHandler
        data->addEvent("IdleDialogueUnlock");	//IdleDialogueExitHandler
        data->addEvent("NPCAttach");	//NPCAttachHandler
        data->addEvent("NPCDetach");	//NPCDetachHandler
        data->addEvent("MTState");	//MTStateHandler
        data->addEvent("VampireFeedEnd");	//VampireFeedEndHandler
    }else{
        FATAL_RUNTIME_ERROR("BehaviorFile::generateDefaultCharacterData(): The behavior graph data failed to construct!");
    }
}

/*
void BehaviorFile::generateDefaultCharacterData(){
    generateNewBehavior();
    hkbBehaviorGraphData *data = static_cast<hkbBehaviorGraphData *>(graphData.data());
    if (data){
        //Add common variables...
        data->addVariable(VARIABLE_TYPE_BOOL, "bEquipOk");   //Needs to be initialized to one or custom creatures don't work...
        data->addVariable(VARIABLE_TYPE_REAL, "Speed"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_REAL, "Direction"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_INT32, "iState");
        data->addVariable(VARIABLE_TYPE_REAL, "TurnDelta"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_REAL, "SpeedSampled");
        data->addVariable(VARIABLE_TYPE_REAL, "SpeedDamped");
        data->addVariable(VARIABLE_TYPE_REAL, "TurnDeltaDamped");
        data->addVariable(VARIABLE_TYPE_REAL, "weaponSpeedMult"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_BOOL, "bAnimationDriven");
        data->addVariable(VARIABLE_TYPE_BOOL, "bAllowRotation");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsAttacking");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsStaggering");
        data->addVariable(VARIABLE_TYPE_BOOL, "IsRecoiling");
        data->addVariable(VARIABLE_TYPE_BOOL, "bFootIKEnable");   //Creatures use this, humans don't...
        data->addVariable(VARIABLE_TYPE_BOOL, "bHumanoidFootIKEnable");   //Humans use this, creatures don't...
        data->addVariable(VARIABLE_TYPE_REAL, "staggerMagnitude");
        data->addVariable(VARIABLE_TYPE_REAL, "staggerDirection");
        data->addVariable(VARIABLE_TYPE_BOOL, "bHeadTrackingOn");   //Creatures use this, humans don't...
        data->addVariable(VARIABLE_TYPE_BOOL, "bHeadTracking"); //Protected variable...
        data->addVariable(VARIABLE_TYPE_VECTOR4, "TargetLocation");
        data->addVariable(VARIABLE_TYPE_REAL, "turnSpeedMult");   //Creatures use this, humans???
        //Add common events...
        data->addEvent("moveStart");
        data->addEvent("moveStop");
        data->addEvent("bleedOutStart");
        data->addEvent("bleedOutStop");
        data->addEvent("blockAnticipateStart");
        data->addEvent("blockHitStart");
        data->addEvent("NPC_BumpedFromRight");
        data->addEvent("NPC_BumpedFromLeft");
        data->addEvent("NPC_BumpedFromBack");
        data->addEvent("NPC_BumpedFromFront");
        data->addEvent("Ragdoll");
        data->addEvent("DeathAnim");
        data->addEvent("WeapEquip");
        data->addEvent("WeapSoloEquip");
        data->addEvent("Magic_Equip");
        data->addEvent("attackStartDualWield");
        data->addEvent("attackPowerStartH2HCombo");
        data->addEvent("attackPowerStartDualWield");
        data->addEvent("StaffBashRelease");
        data->addEvent("JumpFallDirectional");
        data->addEvent("JumpFall");
        data->addEvent("MC_WeapOutRightReplaceForceEquip");
        data->addEvent("WeapSoloEquip");
        data->addEvent("torchEquip");
        data->addEvent("ShdEquip");
        data->addEvent("swimForceEquip");
        data->addEvent("WeapOutLeftReplaceForceEquip");
        data->addEvent("torchForceEquip");
        data->addEvent("MagicWeap_ForceEquip");
        data->addEvent("WeapOutRightReplaceForceEquip");
        data->addEvent("Magic_Solo_Equip");
        data->addEvent("MagicForceEquipLeft");
        data->addEvent("MagicForceEquipRight");
        data->addEvent("MagicForceEquip");
        data->addEvent("GetUpBegin");
        data->addEvent("IdlePlayer");
        data->addEvent("IdleStop");
        data->addEvent("IdleStopInstant");
        data->addEvent("JumpDirectionalStart");
        data->addEvent("JumpStandingStart");
        data->addEvent("JumpLandDirectional");
        data->addEvent("JumpLandEnd");
        data->addEvent("JumpLand");
        data->addEvent("blockStart");
        data->addEvent("attackStart_MC_1HMLeft");
        data->addEvent("attackStartSprintLeftHand");
        data->addEvent("attackStartH2HLeft");
        data->addEvent("MLh_SpellSelfStart");
        data->addEvent("MLh_SpellAimedStart");
        data->addEvent("MLh_WardStart");
        data->addEvent("MLh_SpellSelfConcentrationStart");
        data->addEvent("MLh_SpellAimedConcentrationStart");
        data->addEvent("RitualSpellStart");
        data->addEvent("RitualSpellAimConcentrationStart");
        data->addEvent("DualMagic_SpellAimedStart");
        data->addEvent("DualMagic_SpellSelfStart");
        data->addEvent("DualMagic_SpellAimedStart");
        data->addEvent("DualMagic_SpellAimedConcentrationStart");
        data->addEvent("DualMagic_WardStart");
        data->addEvent("DualMagic_SpellSelfConcentrationStart");
        data->addEvent("MLh_SpellTelekinesisStart");
        data->addEvent("bashStart");
        data->addEvent("attackStartLeftHand");
        data->addEvent("RitualSpellOut");
        data->addEvent("MLh_Equipped_Event");
        data->addEvent("attackPowerStart_MC_1HMLeft");
        data->addEvent("attackPowerStart_SprintLeftHand");
        data->addEvent("attackPowerStartForwardLeftHand");
        data->addEvent("attackPowerStartRightLeftHand");
        data->addEvent("attackPowerStartLeftLeftHand");
        data->addEvent("attackPowerStartBackLeftHand");
        data->addEvent("attackPowerStartInPlaceLeftHand");
        data->addEvent("attackPowerStartForwardH2HLeftHand");
        data->addEvent("MLh_SpellReady_event");
        data->addEvent("blockStop");
        data->addEvent("bashRelease");
        data->addEvent("attackReleaseL");
        data->addEvent("MLH_SpellRelease_event");
        data->addEvent("NPC_TurnRight90");
        data->addEvent("NPC_TurnRight180");
        data->addEvent("NPC_TurnLeft90");
        data->addEvent("NPC_TurnLeft180");
        data->addEvent("NPC_TurnToWalkRight90");
        data->addEvent("NPC_TurnToWalkRight180");
        data->addEvent("NPC_TurnToWalkLeft90");
        data->addEvent("NPC_TurnToWalkLeft180");
        data->addEvent("recoilStart");
        data->addEvent("recoilLargeStart");
        data->addEvent("IdleForceDefaultState");
        data->addEvent("attackStart_MC_1HMRight");
        data->addEvent("bowAttackStart");
        data->addEvent("attackStartSprint");
        data->addEvent("attackStartH2HRight");
        data->addEvent("MRh_SpellSelfStart");
        data->addEvent("MRh_SpellAimedStart");
        data->addEvent("MRh_WardStart");
        data->addEvent("MRh_SpellSelfConcentrationStart");
        data->addEvent("MRh_SpellAimedConcentrationStart");
        data->addEvent("MRh_SpellTelekinesisStart");
        data->addEvent("BashFail");
        data->addEvent("attackStart");
        data->addEvent("attackStop");
        data->addEvent("MRh_Equipped_Event");
        data->addEvent("attackPowerStart_MC_1HMRight");
        data->addEvent("attackPowerStart_2HMSprint");
        data->addEvent("attackPowerStart_2HWSprint");
        data->addEvent("attackPowerStart_Sprint");
        data->addEvent("attackPowerStartForwardH2HRightHand");
        data->addEvent("attackPowerStartInPlace");
        data->addEvent("attackPowerStartForward");
        data->addEvent("attackPowerStartRight");
        data->addEvent("attackPowerStartLeft");
        data->addEvent("attackPowerStartBackward");
        data->addEvent("bashPowerStart");
        data->addEvent("MRh_SpellReady_event");
        data->addEvent("MRH_SpellRelease_event");
        data->addEvent("attackRelease");
        data->addEvent("UnequipNoAnim");
        data->addEvent("Unequip");
        data->addEvent("SneakStart");
        data->addEvent("SneakStop");
        data->addEvent("BlockBashSprint");
        data->addEvent("SneakSprintStartRoll");
        data->addEvent("SprintStart");
        data->addEvent("sprintStop");
        data->addEvent("00NextClip");
        data->addEvent("StaggerPlayer");
        data->addEvent("staggerStart");
        data->addEvent("MountedStaggerStart");
        data->addEvent("SwimStart");
        data->addEvent("swimStop");
        data->addEvent("MountedSwimStart");
        data->addEvent("MountedSwimStop");
        data->addEvent("TurnLeft");
        data->addEvent("TurnRight");
        data->addEvent("turnStop");
        data->addEvent("NPCshoutStart");
        data->addEvent("shoutStart");
        data->addEvent("shoutStop");
        data->addEvent("shoutSprintMediumStart");
        data->addEvent("shoutSprintLongStart");
        data->addEvent("shoutSprintLongestStart");
        data->addEvent("shoutReleaseSlowTime");
        data->addEvent("CombatReady_BreathExhaleShort");
        data->addEvent("MT_BreathExhaleShort");
        data->addEvent("wardAbsorb");
        data->addEvent("WardBreak");
        //Events raised in the character behaviors that the game engine probably responds to in some way...
        data->addEvent("AddRagdollToWorld");
        data->addEvent("RemoveCharacterControllerFromWorld");
        data->addEvent("AddCharacterControllerToWorld");
        data->addEvent("InterruptCast");
        data->addEvent("Reanimated");
        data->addEvent("HeadTrackingOff");
        data->addEvent("HeadTrackingOn");
        data->addEvent("tailUnequip");
        data->addEvent("arrowDetach");
        data->addEvent("EnableBumper");
        data->addEvent("DisableBumper");
        data->addEvent("weaponSheathe");
        data->addEvent("JumpUp");
        data->addEvent("MTState");
        data->addEvent("AnimObjectUnequip");
        data->addEvent("tailMTState");
        data->addEvent("bowReset");
        data->addEvent("BeginWeaponSheathe");
        data->addEvent("tailCombatLocomotion");
        data->addEvent("tailSneakLocomotion");
        data->addEvent("tailCombatIdle");
        data->addEvent("tailSneakIdle");
        data->addEvent("weaponDraw");
        data->addEvent("PairedKillTarget");
        data->addEvent("PairEnd");
        data->addEvent("MountEnd");
        data->addEvent("StartAnimatedCamera");
        data->addEvent("AnimObjLoad");
        data->addEvent("AnimObjDraw");
        data->addEvent("EndAnimatedCamera");
        data->addEvent("StartAnimatedCameraDelta");
        data->addEvent("2_PairEnd");
        data->addEvent("ExitCartBegin");
        data->addEvent("ExitCartEnd");
        data->addEvent("SoundPlay.NPCWerewolfTransformation");
        data->addEvent("ZeroOutCameraPitch");
        data->addEvent("PowerAttack_Start_end");
        data->addEvent("CastOKStop");
        data->addEvent("slowdownStart");
        data->addEvent("FootRight");
        data->addEvent("FootLeft");
        data->addEvent("preHitFrame");
        data->addEvent("weaponSwing");
        data->addEvent("HitFrame");
        data->addEvent("AttackWinStart");
        data->addEvent("AttackWinEnd");
        /*data->addEvent("SoundPlay.WPNSwingUnarmed");
        data->addEvent("SoundPlay.NPCHumanCombatIdleA");
        data->addEvent("SoundPlay.NPCHumanCombatIdleB");
        data->addEvent("SoundPlay.NPCHumanCombatIdleC");
        data->addEvent("2_SoundPlay.NPCKill1HMDualWieldA");
        data->addEvent("NPCSoundPlay.NPCKillStruggle");
        data->addEvent("NPCSoundPlay.NPCKillShove");
        data->addEvent("NPCSoundPlay.NPCKillChop");
        data->addEvent("NPCSoundPlay.NPCKillSmash");
        data->addEvent("NPCSoundPlay.NPCKillMeleeB");
        data->addEvent("NPCSoundPlay.NPCKillGore");
        data->addEvent("NPCSoundPlay.NPCKillBodyfall");
        data->addEvent("NPCSoundPlay.NPCKillStabIn");
        data->addEvent("NPCSoundPlay.WPNBlockBlade2HandVsOtherSD");
        data->addEvent("SoundPlay.WPNBowZoomIn");
        data->addEvent("2_KillMoveStart");
        data->addEvent("2_DeathEmote");
        data->addEvent("2_KillMoveEnd");
        data->addEvent("2_KillActor");
        data->addEvent("MRh_SpellFire_Event");
    }else{
        FATAL_RUNTIME_ERROR("BehaviorFile::generateDefaultCharacterData(): The behavior graph data failed to construct!");
    }
}*/

QStringList BehaviorFile::getAllBehaviorFileNames() const{
    QStringList list;
    QDirIterator it(QFileInfo(*this).absolutePath()+"/");
    while (it.hasNext()){
        if (QFileInfo(it.next()).fileName().contains(".hkx")){
            list.append(it.fileInfo().filePath().section("/", -2, -1));
        }
    }
    return list;
}

void BehaviorFile::setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time){
    project->setLocalTimeForClipGenAnimData(clipname, triggerindex, time);
}

void BehaviorFile::setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, int eventid){
    project->setEventNameForClipGenAnimData(clipname, triggerindex, getEventNameAt(eventid));
}

bool BehaviorFile::isClipGenNameTaken(const QString & name) const{
    for (int i = 0; i < generators.size() - 1; i++){
        if (generators.at(i).data()->getSignature() == HKB_CLIP_GENERATOR && name == static_cast<hkbClipGenerator *>(generators.at(i).data())->getName()){
            return true;
        }
    }
    return false;
}

bool BehaviorFile::isClipGenNameAvailable(const QString &name) const{
    if (project){
        return !project->isClipGenNameTaken(name);
    }
    return false;
}

bool BehaviorFile::addClipGenToAnimationData(const QString &name){
    if (!isClipGenNameAvailable(name)){
        return false;
    }
    return project->appendClipGeneratorAnimData(name);
}

bool BehaviorFile::removeClipGenFromAnimData(const QString & animationname, const QString & clipname, const QString & variablename){
    return project->removeClipGenFromAnimData(animationname, clipname, variablename);
}

void BehaviorFile::setClipNameAnimData(const QString &oldclipname, const QString &newclipname){
    project->setClipNameAnimData(oldclipname, newclipname);
}

void BehaviorFile::setAnimationIndexAnimData(int index, const QString &clipGenName){
    project->setAnimationIndexForClipGen(index, clipGenName);
}

void BehaviorFile::setPlaybackSpeedAnimData(const QString &clipGenName, qreal speed){
    project->setPlaybackSpeedAnimData(clipGenName, speed);
}

void BehaviorFile::setCropStartAmountLocalTimeAnimData(const QString &clipGenName, qreal time){
    project->setCropStartAmountLocalTimeAnimData(clipGenName, time);
}

void BehaviorFile::setCropEndAmountLocalTimeAnimData(const QString &clipGenName, qreal time){
    project->setCropEndAmountLocalTimeAnimData(clipGenName, time);
}

void BehaviorFile::appendClipTriggerToAnimData(const QString &clipGenName){
    project->appendClipTriggerToAnimData(clipGenName, getEventNameAt(0));
}

void BehaviorFile::removeClipTriggerToAnimDataAt(const QString &clipGenName, int index){
    project->removeClipTriggerToAnimDataAt(clipGenName, index);
}

QString BehaviorFile::isEventReferenced(int eventindex) const{
    hkbBehaviorGraphStringData *stringdata = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    QString objnames;
    int size;
    if (stringdata && stringdata->eventNames.size() > eventindex && eventindex > -1){
        objnames = "The event \""+stringdata->getEventNameAt(eventindex)+" \" is referenced by the following objects: \n";
        size = objnames.size();
        for (auto i = 0; i < generators.size(); i++){
            if (generators.at(i).constData()->isEventReferenced(eventindex)){
                objnames.append(static_cast<const hkbGenerator *>(generators.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
        for (auto i = 0; i < generatorChildren.size(); i++){
            if (generatorChildren.at(i).constData()->isEventReferenced(eventindex)){
                objnames.append(static_cast<const hkbGenerator *>(generatorChildren.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
        for (auto i = 0; i < modifiers.size(); i++){
            if (modifiers.at(i).constData()->isEventReferenced(eventindex)){
                objnames.append(static_cast<const hkbGenerator *>(modifiers.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR(QString("BehaviorFile::isEventReferenced(): stringdata null or invalid variable index!!!"));
    }
    if (objnames.size() > size){
        return objnames;
    }else{
        return "";
    }
}

void BehaviorFile::updateEventIndices(int index){
    hkbBehaviorGraphStringData *stringdata = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (stringdata && stringdata->eventNames.size() >= index && index > -1){
        for (auto i = 0; i < generators.size(); i++){
            generators.at(i).data()->updateEventIndices(index);
        }
        for (auto i = 0; i < generatorChildren.size(); i++){
            generatorChildren.at(i).data()->updateEventIndices(index);
        }
        for (auto i = 0; i < modifiers.size(); i++){
            modifiers.at(i).data()->updateEventIndices(index);
        }
    }else{
        FATAL_RUNTIME_ERROR(QString("BehaviorFile::updateEventIndices(): stringdata null or invalid event index!!!"));
    }
}

QString BehaviorFile::isVariableReferenced(int variableindex) const{
    hkbBehaviorGraphStringData *stringdata = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    QString objnames;
    int size;
    if (stringdata && stringdata->variableNames.size() > variableindex && variableindex > -1){
        objnames = "The variable \""+stringdata->getVariableNameAt(variableindex)+" \" is referenced by the following objects: \n";
        size = objnames.size();
        for (auto i = 0; i < generators.size(); i++){
            if (generators.at(i).constData()->isVariableReferenced(variableindex)){
                objnames.append(static_cast<const hkbGenerator *>(generators.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
        for (auto i = 0; i < generatorChildren.size(); i++){
            if (generatorChildren.at(i).constData()->isVariableReferenced(variableindex)){
                objnames.append(static_cast<const hkbGenerator *>(generatorChildren.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
        for (auto i = 0; i < modifiers.size(); i++){
            if (modifiers.at(i).constData()->isVariableReferenced(variableindex)){
                objnames.append(static_cast<const hkbGenerator *>(modifiers.at(i).constData())->getName()+"\n");
            }
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
        for (auto i = 0; i < otherTypes.size(); i++){
            if (otherTypes.at(i).constData()->getSignature() == HKB_VARIABLE_BINDING_SET){
                const hkbVariableBindingSet *tempptr = static_cast<const hkbVariableBindingSet *>(otherTypes.at(i).constData());
                tempptr->getSignature();
            }
            if (otherTypes.at(i).constData()->getSignature() == HKB_BLENDING_TRANSITION_EFFECT && otherTypes.at(i).constData()->isVariableReferenced(variableindex)){
                objnames.append(static_cast<const hkbBlendingTransitionEffect *>(otherTypes.at(i).constData())->getName()+"\n");
            }/*else if (otherTypes.at(i).constData()->getSignature() != HKB_VARIABLE_BINDING_SET){
                objnames.append(QString::number(otherTypes.at(i).constData()->getSignature(), 16)+"\n");
            }*/
            if (objnames.size() > MAX_ERROR_STRING_SIZE){
                objnames.append("...");
                break;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR(QString("BehaviorFile::isVariableReferenced(): stringdata null or invalid variable index!!!"));
    }
    if (objnames.size() > size){
        return objnames;
    }else{
        return "";
    }
}

void BehaviorFile::updateVariableIndices(int index){
    hkbBehaviorGraphStringData *stringdata = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
    if (stringdata && stringdata->variableNames.size() >= index && index > -1){
        for (auto i = 0; i < otherTypes.size(); i++){
            if (otherTypes.at(i).data()->getSignature() == HKB_VARIABLE_BINDING_SET){
                static_cast<hkbVariableBindingSet *>(otherTypes.at(i).data())->updateVariableIndices(index);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR(QString("BehaviorFile::updateVariableIndices(): stringdata null or invalid variable index!!!"));
    }
}

void BehaviorFile::removeUnreferencedFiles(const hkbBehaviorReferenceGenerator *gentoignore){
    project->removeUnreferencedFiles(gentoignore);
}

QStringList BehaviorFile::getReferencedBehaviors(const hkbBehaviorReferenceGenerator *gentoignore) const{
    QStringList list;
    QString name;
    for (auto i = 0; i < generators.size(); i++){
        if (generators.at(i).data()->getSignature() == HKB_BEHAVIOR_REFERENCE_GENERATOR && gentoignore != generators.at(i).constData()){
            name = static_cast<hkbBehaviorReferenceGenerator *>(generators.at(i).data())->getBehaviorName();
            if (!list.contains(name)){
                list.append(name);
            }
        }
    }
    return list;
}

void BehaviorFile::removeAllData(){
    removeGeneratorData();
    removeModifierData();
    removeOtherData();
}

HkxObject * BehaviorFile::getRootStateMachine() const{
    return static_cast<hkbBehaviorGraph *>(behaviorGraph.data())->rootGenerator.data();
}

hkbBehaviorGraph * BehaviorFile::getBehaviorGraph() const{
    return static_cast<hkbBehaviorGraph *>(behaviorGraph.data());
}

QStringList BehaviorFile::getRigBoneNames() const{
    return character->getRigBoneNames();
}

int BehaviorFile::getNumberOfBones(bool ragdoll) const{
    return character->getNumberOfBones(ragdoll);
}

QStringList BehaviorFile::getRagdollBoneNames() const{
    return character->getRagdollBoneNames();
}

/*TreeGraphicsItem * BehaviorFile::getRootIcon() const{
    hkRootLevelContainer *root = static_cast<hkRootLevelContainer *>(rootObject.data());
    hkbBehaviorGraph *graph = nullptr;
    if (!root->namedVariants.isEmpty() && root->namedVariants.at(0).variant.data() && root->namedVariants.at(0).variant.data()->getSignature() == HKB_BEHAVIOR_GRAPH){
        graph = static_cast<hkbBehaviorGraph *>(root->namedVariants.at(0).variant.data());
        if (!graph->icons.isEmpty()){
            return graph->icons.first();
        }else{
            graph->appendIcon(new TreeGraphicsItem(graph, graph->getName()));
            return graph->icons.first();
        }
    }
    return nullptr;
}*/

bool BehaviorFile::addObjectToFile(HkxObject *obj, long ref){
    if (ref > largestRef){
        largestRef = ref;
    }else{
        largestRef++;
    }
    obj->setReference(largestRef);
    if (obj->getType() == HkxObject::TYPE_GENERATOR){
        if (obj->getSignature() == HKB_STATE_MACHINE_STATE_INFO ||
            obj->getSignature() == HKB_BLENDER_GENERATOR_CHILD ||
            obj->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA
           )
        {
            generatorChildren.append(HkxSharedPtr(obj, ref));
        }else{
            generators.append(HkxSharedPtr(obj, ref));
        }
    }else if (obj->getType() == HkxObject::TYPE_MODIFIER){
        modifiers.append(HkxSharedPtr(obj, ref));
    }else if (obj->getType() == HkxObject::TYPE_OTHER){
        otherTypes.append(HkxSharedPtr(obj, ref));
    }else{
        writeToLog("BehaviorFile: addObjectToFile() failed!\nInvalid type enum for this object!\nObject signature is: "+QString::number(obj->getSignature(), 16), true);
        return false;
    }
    return true;
}

bool BehaviorFile::parse(){
    if (!getReader().parse()){
        return false;
    }
    int index = 2;
    bool ok = true;
    HkxSignature signature;
    QByteArray value;
    long ref = 0;
    //setProgressData("Creating HKX objects...", 60);
    while (index < getReader().getNumElements()){
        value = getReader().getNthAttributeNameAt(index, 1);
        if (value == "class"){
            value = getReader().getNthAttributeValueAt(index, 2);
            if (value != ""){
                ref = getReader().getNthAttributeValueAt(index, 0).remove(0, 1).toLong(&ok);
                if (!ok){
                    writeToLog("BehaviorFile: parse() failed!\nThe object reference string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                signature = (HkxSignature)value.toULongLong(&ok, 16);
                if (!ok){
                    writeToLog("BehaviorFile: parse() failed!\nThe object signature string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                if (signature == HKB_STATE_MACHINE_STATE_INFO){
                    if (!appendAndReadData(index, new hkbStateMachineStateInfo(this, nullptr, ref))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE){
                    if (!appendAndReadData(index, new hkbStateMachine(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_VARIABLE_BINDING_SET){
                    if (!appendAndReadData(index, new hkbVariableBindingSet(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MODIFIER_GENERATOR){
                    if (!appendAndReadData(index, new hkbModifierGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == BS_I_STATE_TAGGING_GENERATOR){
                    if (!appendAndReadData(index, new BSiStateTaggingGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
                    if (!appendAndReadData(index, new BSCyclicBlendTransitionGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == BS_BONE_SWITCH_GENERATOR){
                    if (!appendAndReadData(index, new BSBoneSwitchGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
                    if (!appendAndReadData(index, new BSBoneSwitchGeneratorBoneData(this, ref))){
                        return false;
                    }
                }else if (signature == BS_SYNCHRONIZED_CLIP_GENERATOR){
                    if (!appendAndReadData(index, new BSSynchronizedClipGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MANUAL_SELECTOR_GENERATOR){
                    if (!appendAndReadData(index, new hkbManualSelectorGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BLENDER_GENERATOR_CHILD){
                    if (!appendAndReadData(index, new hkbBlenderGeneratorChild(this, nullptr, ref))){
                        return false;
                    }
                }else if (signature == HKB_BONE_WEIGHT_ARRAY){
                    if (!appendAndReadData(index, new hkbBoneWeightArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MODIFIER_LIST){
                    if (!appendAndReadData(index, new hkbModifierList(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BLENDER_GENERATOR){
                    if (!appendAndReadData(index, new hkbBlenderGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BLENDING_TRANSITION_EFFECT){
                    if (!appendAndReadData(index, new hkbBlendingTransitionEffect(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EXPRESSION_CONDITION){
                    if (!appendAndReadData(index, new hkbExpressionCondition(this, "", ref))){
                        return false;
                    }
                }else if (signature == HKB_STRING_CONDITION){
                    if (!appendAndReadData(index, new hkbStringCondition(this, "", ref))){
                        return false;
                    }
                }else if (signature == BS_OFFSET_ANIMATION_GENERATOR){
                    if (!appendAndReadData(index, new BSOffsetAnimationGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_TWIST_MODIFIER){
                    if (!appendAndReadData(index, new hkbTwistModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EVENT_DRIVEN_MODIFIER){
                    if (!appendAndReadData(index, new hkbEventDrivenModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_IS_ACTIVE_MODIFIER){
                    if (!appendAndReadData(index, new BSIsActiveModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_LIMB_IK_MODIFIER){
                    if (!appendAndReadData(index, new BSLimbIKModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_INTERP_VALUE_MODIFIER){
                    if (!appendAndReadData(index, new BSInterpValueModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_GET_TIME_STEP_MODIFIER){
                    if (!appendAndReadData(index, new BSGetTimeStepModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_FOOT_IK_CONTROLS_MODIFIER){
                    if (!appendAndReadData(index, new hkbFootIkControlsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_GET_HANDLE_ON_BONE_MODIFIER){
                    if (!appendAndReadData(index, new hkbGetHandleOnBoneModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_TRANSFORM_VECTOR_MODIFIER){
                    if (!appendAndReadData(index, new hkbTransformVectorModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_PROXY_MODIFIER){
                    if (!appendAndReadData(index, new hkbProxyModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_LOOK_AT_MODIFIER){
                    if (!appendAndReadData(index, new hkbLookAtModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EVENT_RANGE_DATA_ARRAY){
                    if (!appendAndReadData(index, new hkbEventRangeDataArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MIRROR_MODIFIER){
                    if (!appendAndReadData(index, new hkbMirrorModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_GET_WORLD_FROM_MODEL_MODIFIER){
                    if (!appendAndReadData(index, new hkbGetWorldFromModelModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_SENSE_HANDLE_MODIFIER){
                    if (!appendAndReadData(index, new hkbSenseHandleModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EVALUATE_EXPRESSION_MODIFIER){
                    if (!appendAndReadData(index, new hkbEvaluateExpressionModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EVALUATE_HANDLE_MODIFIER){
                    if (!appendAndReadData(index, new hkbEvaluateHandleModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_ATTACHMENT_MODIFIER){
                    if (!appendAndReadData(index, new hkbAttachmentModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_ATTRIBUTE_MODIFIER){
                    if (!appendAndReadData(index, new hkbAttributeModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_COMBINE_TRANSFORMS_MODIFIER){
                    if (!appendAndReadData(index, new hkbCombineTransformsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER){
                    if (!appendAndReadData(index, new hkbComputeRotationFromAxisAngleModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_COMPUTE_ROTATION_TO_TARGET_MODIFIER){
                    if (!appendAndReadData(index, new hkbComputeRotationToTargetModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EVENTS_FROM_RANGE_MODIFIER){
                    if (!appendAndReadData(index, new hkbEventsFromRangeModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_MOVE_CHARACTER_MODIFIER){
                    if (!appendAndReadData(index, new hkbMoveCharacterModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EXTRACT_RAGDOLL_POSE_MODIFIER){
                    if (!appendAndReadData(index, new hkbExtractRagdollPoseModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_MODIFY_ONCE_MODIFIER){
                    if (!appendAndReadData(index, new BSModifyOnceModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_EVENT_ON_DEACTIVATE_MODIFIER){
                    if (!appendAndReadData(index, new BSEventOnDeactivateModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_EVENT_EVERY_N_EVENTS_MODIFIER){
                    if (!appendAndReadData(index, new BSEventEveryNEventsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_RAGDOLL_CONTACT_LISTENER_MODIFIER){
                    if (!appendAndReadData(index, new BSRagdollContactListenerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_POWERED_RAGDOLL_CONTROLS_MODIFIER){
                    if (!appendAndReadData(index, new hkbPoweredRagdollControlsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER){
                    if (!appendAndReadData(index, new BSEventOnFalseToTrueModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_DIRECT_AT_MODIFIER){
                    if (!appendAndReadData(index, new BSDirectAtModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_DIST_TRIGGER_MODIFER){
                    if (!appendAndReadData(index, new BSDistTriggerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_DECOMPOSE_VECTOR_MODIFIER){
                    if (!appendAndReadData(index, new BSDecomposeVectorModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_COMPUTE_ADD_BONE_ANIM_MODIFIER){
                    if (!appendAndReadData(index, new BSComputeAddBoneAnimModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_TWEENER_MODIFIER){
                    if (!appendAndReadData(index, new BSTweenerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_I_STATE_MANAGER_MODIFIER){
                    if (!appendAndReadData(index, new BSIStateManagerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_TIMER_MODIFIER){
                    if (!appendAndReadData(index, new hkbTimerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_ROTATE_CHARACTER_MODIFIER){
                    if (!appendAndReadData(index, new hkbRotateCharacterModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_DAMPING_MODIFIER){
                    if (!appendAndReadData(index, new hkbDampingModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_DELAYED_MODIFIER){
                    if (!appendAndReadData(index, new hkbDelayedModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_GET_UP_MODIFIER){
                    if (!appendAndReadData(index, new hkbGetUpModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_EXPRESSION_DATA_ARRAY){
                    if (!appendAndReadData(index, new hkbExpressionDataArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BONE_INDEX_ARRAY){
                    if (!appendAndReadData(index, new hkbBoneIndexArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_KEY_FRAME_BONES_MODIFIER){
                    if (!appendAndReadData(index, new hkbKeyframeBonesModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_COMPUTE_DIRECTION_MODIFIER){
                    if (!appendAndReadData(index, new hkbComputeDirectionModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_RIGID_BODY_RAGDOLL_CONTROLS_MODIFIER){
                    if (!appendAndReadData(index, new hkbRigidBodyRagdollControlsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_POSE_MATCHING_GENERATOR){
                    if (!appendAndReadData(index, new hkbPoseMatchingGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_CLIP_GENERATOR){
                    if (!appendAndReadData(index, new hkbClipGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_CLIP_TRIGGER_ARRAY){
                    if (!appendAndReadData(index, new hkbClipTriggerArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_GENERATOR_TRANSITION_EFFECT){
                    if (!appendAndReadData(index, new hkbGeneratorTransitionEffect(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BEHAVIOR_REFERENCE_GENERATOR){
                    if (!appendAndReadData(index, new hkbBehaviorReferenceGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY){
                    if (!appendAndReadData(index, new hkbStateMachineTransitionInfoArray(this, nullptr, ref))){
                        return false;
                    }
                }else if (signature == HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY){
                    if (!appendAndReadData(index, new hkbStateMachineEventPropertyArray(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_STRING_EVENT_PAYLOAD){
                    if (!appendAndReadData(index, new hkbStringEventPayload(this, "", ref))){
                        return false;
                    }
                }else if (signature == BS_SPEED_SAMPLER_MODIFIER){
                    if (!appendAndReadData(index, new BSSpeedSamplerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_DETECT_CLOSE_TO_GROUND_MODIFIER){
                    if (!appendAndReadData(index, new hkbDetectCloseToGroundModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_PASS_BY_TARGET_TRIGGER_MODIFIER){
                    if (!appendAndReadData(index, new BSLookAtModifier(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_HAND_IK_CONTROLS_MODIFIER){
                    if (!appendAndReadData(index, new hkbHandIkControlsModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_TIMER_MODIFIER){
                    if (!appendAndReadData(index, new BSTimerModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BS_LOOK_AT_MODIFIER){
                    if (!appendAndReadData(index, new BSLookAtModifier(this, ref))){
                        return false;
                    }
                }else if (signature == BGS_GAMEBYRO_SEQUENCE_GENERATOR){
                    if (!appendAndReadData(index, new BGSGamebryoSequenceGenerator(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_BEHAVIOR_GRAPH){
                    if (!appendAndReadData(index, new hkbBehaviorGraph(this, ref))){
                        return false;
                    }
                    behaviorGraph = generators.last();
                    generators.removeLast();
                }else if (signature == HKB_BEHAVIOR_GRAPH_DATA){
                    if (!appendAndReadData(index, new hkbBehaviorGraphData(this, ref))){
                        return false;
                    }
                    graphData = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HKB_BEHAVIOR_GRAPH_STRING_DATA){
                    if (!appendAndReadData(index, new hkbBehaviorGraphStringData(this, ref))){
                        return false;
                    }
                    stringData = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HKB_VARIABLE_VALUE_SET){
                    if (!appendAndReadData(index, new hkbVariableValueSet(this, ref))){
                        return false;
                    }
                    variableValues = otherTypes.last();
                    otherTypes.removeLast();
                }else if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this, ref))){
                        return false;
                    }
                    setRootObject(otherTypes.last());
                    otherTypes.removeLast();
                }else{
                    writeToLog("BehaviorFile: parse()!\nUnknown signature detected!\nUnknown object class name is: "+getReader().getNthAttributeValueAt(index, 1)+"\nUnknown object signature is: "+QString::number(signature, 16));
                }
            }
        }
        index++;
    }
    closeFile();
    getReader().clear();
    //setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("BehaviorFile: parse() failed because link() failed!", true);
        return false;
    }
    return true;
}

bool BehaviorFile::link(){
    if (!getRootObject().constData()){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file is nullptr!", true);
        return false;
    }else if (getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file is NOT a hkRootLevelContainer!\nThe root object signature is: "+QString::number(getRootObject()->getSignature(), 16), true);
        return false;
    }
    if (!getRootObject().data()->link()){
        writeToLog("BehaviorFile: link() failed!\nThe root object of this behavior file failed to link to it's children!", true);
        return false;
    }
    for (int i = generators.size() - 1; i >= 0; i--){
        if (!generators.at(i).data()->link()){
            writeToLog("BehaviorFile: link() failed!\nA generator failed to link to it's children!\nObject signature: "+QString::number(generators.at(i)->getSignature(), 16)+"\nObject reference: "+QString::number(generators.at(i).getReference()), true);
            return false;
        }
    }
    for (int i = modifiers.size() - 1; i >= 0; i--){
        if (!modifiers.at(i).data()->link()){
            writeToLog("BehaviorFile: link() failed!\nA modifier failed to link to it's children!\nObject signature: "+QString::number(modifiers.at(i)->getSignature(), 16)+"\nObject reference: "+QString::number(generators.at(i).getReference()), true);
            return false;
        }
    }
    for (int i = generatorChildren.size() - 1; i >= 0; i--){
        if (!generatorChildren.at(i).data()->link()){
            writeToLog("BehaviorFile: link() failed!\nA generator child failed to link to it's children!\nObject signature: "+QString::number(generatorChildren.at(i)->getSignature(), 16)+"\nObject reference: "+QString::number(generators.at(i).getReference()), true);
            return false;
        }
    }
    for (int i = otherTypes.size() - 1; i >= 0; i--){
        if (!otherTypes.at(i).data()->link()){
            writeToLog("BehaviorFile: link() failed!\nA generator child failed to link to it's children!\nObject signature: "+QString::number(otherTypes.at(i)->getSignature(), 16)+"\nObject reference: "+QString::number(generators.at(i).getReference()), true);
            return false;
        }
    }
    if (!behaviorGraph.data()->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbBehaviorGraph failed to link to it's children!\n", true);
        return false;
    }
    if (!variableValues.data()->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbVariableValueSet failed to link to it's children!\n", true);
        return false;
    }
    if (!graphData.data()->link()){
        writeToLog("BehaviorFile: link() failed!\nhkbBehaviorGraphData failed to link to it's children!\n", true);
        return false;
    }
    return true;
}

QVector <HkxObject *> BehaviorFile::merge(BehaviorFile *recessivefile){
    bool found;
    QVector <HkxObject *> objectsnotfound;
    if (recessivefile){
        for (auto i = 0; i < generators.size(); i++){
            found = false;
            for (auto j = i; j < recessivefile->generators.size(); j++){
                if (static_cast<hkbGenerator *>(generators.at(i).data())->getName() == static_cast<hkbGenerator *>(recessivefile->generators.at(j).data())->getName()
                        && generators.at(i).data()->getSignature() == recessivefile->generators.at(j).data()->getSignature())
                {
                    generators.at(i).data()->merge(recessivefile->generators.at(j).data());
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < i; j++){
                    if (static_cast<hkbGenerator *>(generators.at(i).data())->getName() == static_cast<hkbGenerator *>(recessivefile->generators.at(j).data())->getName()
                            && generators.at(i).data()->getSignature() == recessivefile->generators.at(j).data()->getSignature())
                    {
                        generators.at(i).data()->merge(recessivefile->generators.at(j).data());
                        found = true;
                        break;
                    }
                }
            }
            if (!found){
                objectsnotfound.append(generators.at(i).data());
                //writeToLog("ProjectFile: merge(): The object type \""+QString::number(generators.at(i).data()->getSignature(), 16)
                //           +"\" named \""+static_cast<hkbGenerator *>(generators.at(i).data())->getName()+"\" was not found in the recessive behavior!!!");
            }
        }
        for (auto i = 0; i < generatorChildren.size(); i++){
            found = false;
            for (auto j = i; j < recessivefile->generatorChildren.size(); j++){
                if (static_cast<hkbGenerator *>(generatorChildren.at(i).data())->getName() == static_cast<hkbGenerator *>(recessivefile->generatorChildren.at(j).data())->getName()
                        && generatorChildren.at(i).data()->getSignature() == recessivefile->generatorChildren.at(j).data()->getSignature())
                {
                    generatorChildren.at(i).data()->merge(recessivefile->generatorChildren.at(j).data());
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < i; j++){
                    if (static_cast<hkbGenerator *>(generatorChildren.at(i).data())->getName() == static_cast<hkbGenerator *>(recessivefile->generatorChildren.at(j).data())->getName()
                            && generatorChildren.at(i).data()->getSignature() == recessivefile->generatorChildren.at(j).data()->getSignature())
                    {
                        generatorChildren.at(i).data()->merge(recessivefile->generatorChildren.at(j).data());
                        found = true;
                        break;
                    }
                }
            }
            if (!found){
                objectsnotfound.append(generatorChildren.at(i).data());
            }
        }
        for (auto i = 0; i < modifiers.size(); i++){
            found = false;
            for (auto j = i; j < recessivefile->modifiers.size(); j++){
                if (static_cast<hkbModifier *>(modifiers.at(i).data())->getName() == static_cast<hkbModifier *>(recessivefile->modifiers.at(j).data())->getName()
                        && modifiers.at(i).data()->getSignature() == recessivefile->modifiers.at(j).data()->getSignature())
                {
                    modifiers.at(i).data()->merge(recessivefile->modifiers.at(j).data());
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < i; j++){
                    if (static_cast<hkbModifier *>(modifiers.at(i).data())->getName() == static_cast<hkbModifier *>(recessivefile->modifiers.at(j).data())->getName()
                            && modifiers.at(i).data()->getSignature() == recessivefile->modifiers.at(j).data()->getSignature())
                    {
                        modifiers.at(i).data()->merge(recessivefile->modifiers.at(j).data());
                        found = true;
                        break;
                    }
                }
            }
            if (!found){
                objectsnotfound.append(modifiers.at(i).data());
            }
        }
    }else{
        writeToLog("ProjectFile: merge() failed!\nrecessiveproject is nullptr!\n");
    }
    return objectsnotfound;
}

QVector <HkxObject *> BehaviorFile::merge(QVector <HkxObject *> dominantobjects){
    bool found;
    for (auto i = dominantobjects.size(); i < -1; i--){
        found = false;
        if (dominantobjects.at(i)->getType() == HkxObject::TYPE_GENERATOR){
            if (dominantobjects.at(i)->getSignature() == HKB_STATE_MACHINE_STATE_INFO || dominantobjects.at(i)->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
                for (auto j = i; j < generatorChildren.size(); j++){
                    if (static_cast<hkbGenerator *>(dominantobjects.at(i))->getName() == static_cast<hkbGenerator *>(generatorChildren.at(j).data())->getName()
                            && dominantobjects.at(i)->getSignature() == generatorChildren.at(j).data()->getSignature())
                    {
                        dominantobjects.at(i)->merge(generatorChildren.at(j).data());
                        dominantobjects.removeAt(i);
                        found = true;
                        break;
                    }
                }
                if (!found){
                    for (auto j = 0; j < i; j++){
                        if (static_cast<hkbGenerator *>(dominantobjects.at(i))->getName() == static_cast<hkbGenerator *>(generatorChildren.at(j).data())->getName()
                                && dominantobjects.at(i)->getSignature() == generatorChildren.at(j).data()->getSignature())
                        {
                            dominantobjects.at(i)->merge(generatorChildren.at(j).data());
                            dominantobjects.removeAt(i);
                            break;
                        }
                    }
                }
            }else{
                for (auto j = i; j < generators.size(); j++){
                    if (static_cast<hkbGenerator *>(dominantobjects.at(i))->getName() == static_cast<hkbGenerator *>(generators.at(j).data())->getName()
                            && dominantobjects.at(i)->getSignature() == generators.at(j).data()->getSignature())
                    {
                        dominantobjects.at(i)->merge(generators.at(j).data());
                        dominantobjects.removeAt(i);
                        found = true;
                        break;
                    }
                }
                if (!found){
                    for (auto j = 0; j < i; j++){
                        if (static_cast<hkbGenerator *>(dominantobjects.at(i))->getName() == static_cast<hkbGenerator *>(generators.at(j).data())->getName()
                                && dominantobjects.at(i)->getSignature() == generators.at(j).data()->getSignature())
                        {
                            dominantobjects.at(i)->merge(generators.at(j).data());
                            dominantobjects.removeAt(i);
                            break;
                        }
                    }
                }
            }
        }else if (dominantobjects.at(i)->getType() == HkxObject::TYPE_MODIFIER){
            for (auto j = i; j < modifiers.size(); j++){
                if (static_cast<hkbModifier *>(dominantobjects.at(i))->getName() == static_cast<hkbModifier *>(modifiers.at(j).data())->getName()
                        && dominantobjects.at(i)->getSignature() == modifiers.at(j).data()->getSignature())
                {
                    dominantobjects.at(i)->merge(modifiers.at(j).data());
                    dominantobjects.removeAt(i);
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < i; j++){
                    if (static_cast<hkbModifier *>(dominantobjects.at(i))->getName() == static_cast<hkbModifier *>(modifiers.at(j).data())->getName()
                            && dominantobjects.at(i)->getSignature() == modifiers.at(j).data()->getSignature())
                    {
                        dominantobjects.at(i)->merge(modifiers.at(j).data());
                        dominantobjects.removeAt(i);
                        break;
                    }
                }
            }
        }
    }
    return dominantobjects;
}

void BehaviorFile::write(){
    if (getRootObject().data()){
        ulong ref = getRootObject().data()->getReference();
        getRootObject().data()->setIsWritten(false);
        stringData.data()->setIsWritten(false);
        variableValues.data()->setIsWritten(false);
        graphData.data()->setIsWritten(false);
        behaviorGraph.data()->setIsWritten(false);
        ref++;
        stringData.data()->setReference(ref);
        ref++;
        variableValues.data()->setReference(ref);
        ref++;
        graphData.data()->setReference(ref);
        ref++;
        behaviorGraph.data()->setReference(ref);
        ref++;
        for (int i = 0; i < generators.size(); i++, ref++){
            generators.at(i).data()->setIsWritten(false);
            generators.at(i).data()->setReference(ref);
        }
        for (int i = 0; i < generatorChildren.size(); i++, ref++){
            generatorChildren.at(i).data()->setIsWritten(false);
            generatorChildren.at(i).data()->setReference(ref);
        }
        for (int i = 0; i < modifiers.size(); i++, ref++){
            modifiers.at(i).data()->setIsWritten(false);
            modifiers.at(i).data()->setReference(ref);
        }
        for (int i = 0; i < otherTypes.size(); i++, ref++){
            otherTypes.at(i).data()->setIsWritten(false);
            otherTypes.at(i).data()->setReference(ref);
        }
        getWriter().setFile(this);
        if (!getWriter().writeToXMLFile()){
            FATAL_RUNTIME_ERROR("BehaviorFile::write(): writeToXMLFile() failed!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BehaviorFile::write(): The root object is nullptr!!");
    }
}

HkxSharedPtr * BehaviorFile::findGenerator(long ref){
    if (ref < 0){
        return nullptr;
    }
    for (int i = 0; i < generators.size(); i++){
        if (ref == generators.at(i).getReference()){
            return &generators[i];
        }
    }
    return nullptr;
}

HkxSharedPtr * BehaviorFile::findGeneratorChild(long ref){
    if (ref < 0){
        return nullptr;
    }
    for (int i = 0; i < generatorChildren.size(); i++){
        if (ref == generatorChildren.at(i).getReference()){
            return &generatorChildren[i];
        }
    }
    return nullptr;
}

HkxSharedPtr * BehaviorFile::findModifier(long ref){
    if (ref < 0){
        return nullptr;
    }
    for (int i = 0; i < modifiers.size(); i++){
        if (ref == modifiers.at(i).getReference()){
            return &modifiers[i];
        }
    }
    return nullptr;
}

HkxSharedPtr * BehaviorFile::findHkxObject(long ref){
    if (ref < 0){
        return nullptr;
    }
    for (int i = 0; i < otherTypes.size(); i++){
        if (ref == otherTypes.at(i).getReference()){
            return &otherTypes[i];
        }
    }
    return nullptr;
}

int BehaviorFile::addCharacterProperty(int index){
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->addVariable(character->getCharacterPropertyTypeAt(index), character->getCharacterPropertyNameAt(index), true);
}

QString BehaviorFile::getVariableNameAt(int index) const{
    if (stringData.data()){
        return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->getVariableNameAt(index);
    }
    return "";
}

QString BehaviorFile::getEventNameAt(int index) const{
    if (stringData.data()){
        return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->getEventNameAt(index);
    }
    return "";
}

QString BehaviorFile::getCharacterPropertyNameAt(int index, bool fromBehaviorFile) const{
    if (fromBehaviorFile){
        if (graphData.data()){
            return static_cast<hkbBehaviorGraphData *>(graphData.data())->getCharacterPropertyNameAt(index);
        }
    }else{
        return character->getCharacterPropertyNameAt(index);
    }
    return "";
}

QStringList BehaviorFile::getAnimationNames() const{
    return character->getAnimationNames();
}

QString BehaviorFile::getAnimationNameAt(int index) const{
    return character->getAnimationNameAt(index);
}

QStringList BehaviorFile::getLocalFrameNames() const{
    if (character && character->skeleton){
        return character->skeleton->getLocalFrameNames();
    }
    return QStringList();
}

void BehaviorFile::removeBindings(int varIndex){
    for (int i = 0; i < otherTypes.size(); i++){
        if (otherTypes.at(i).data()->getSignature() == HKB_VARIABLE_BINDING_SET){
            static_cast<hkbVariableBindingSet *>(otherTypes.at(i).data())->removeBinding(varIndex);
        }
    }
}

HkxSharedPtr * BehaviorFile::findBehaviorGraph(long ref){
    if (behaviorGraph.getReference() == ref){
        return &behaviorGraph;
    }
    return nullptr;
}

QVector<int> BehaviorFile::removeGeneratorData(){
    QVector<int> removedIndices;
    HkxObject *obj = nullptr;
    for (int i = generators.size() - 1; i >= 0; i--){
        obj = generators.at(i).data();
        if (obj){
            if (obj->ref < 2){
                generators.removeAt(i);
                removedIndices.append(i);
            }
        }
    }
    return removedIndices;
}

QVector<int> BehaviorFile::removeGeneratorChildrenData(){
    QVector<int> removedIndices;
    HkxObject *obj = nullptr;
    for (int i = generatorChildren.size() - 1; i >= 0; i--){
        obj = generatorChildren.at(i).data();
        if (obj){
            if (obj->ref < 2){
                generatorChildren.removeAt(i);
                removedIndices.append(i);
            }
        }
    }
    return removedIndices;
}

QVector<int> BehaviorFile::removeModifierData(){
    QVector<int> removedIndices;
    for (int i = modifiers.size() - 1; i >= 0; i--){
        if (modifiers.at(i).constData() && modifiers.at(i).constData()->ref < 2){
            modifiers.removeAt(i);
            removedIndices.append(i);
        }
    }
    return removedIndices;
}

QVector<int> BehaviorFile::removeOtherData(){
    QVector<int> removedIndices;
    for (int i = otherTypes.size() - 1; i >= 0; i--){
        if (otherTypes.at(i).constData() && otherTypes.at(i).constData()->ref < 2){
            otherTypes.removeAt(i);
            removedIndices.append(i);
        }
    }
    return removedIndices;
}

hkVariableType BehaviorFile::getVariableTypeAt(int index) const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableTypeAt(index);
}

QStringList BehaviorFile::getGeneratorNames() const{
    QStringList list;
    for (int i = 0; i < generators.size(); i++){
        list.append(static_cast<hkbGenerator *>(generators.at(i).data())->getName());
    }
    return list;
}

QStringList BehaviorFile::getGeneratorTypeNames() const{
    QStringList list;
    for (int i = 0; i < generators.size(); i++){
        list.append(static_cast<hkbGenerator *>(generators.at(i).data())->getClassname());
    }
    return list;
}

QStringList BehaviorFile::getModifierNames() const{
    QStringList list;
    for (int i = 0; i < modifiers.size(); i++){
        list.append(static_cast<hkbGenerator *>(modifiers.at(i).data())->getName());
    }
    return list;
}

QStringList BehaviorFile::getModifierTypeNames() const{
    QStringList list;
    for (int i = 0; i < modifiers.size(); i++){
        list.append(static_cast<hkbGenerator *>(modifiers.at(i).data())->getClassname());
    }
    return list;
}

int BehaviorFile::getCharacterPropertyIndexFromBehavior(const QString &name) const{
    hkbBehaviorGraphStringData *strings = nullptr;
    if (stringData.data()){
        strings = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
        for (int i = 0; i < strings->characterPropertyNames.size(); i++){
            if (strings->characterPropertyNames.at(i) == name){
                return i;
            }
        }
    }
    return -1;
}

int BehaviorFile::getCharacterPropertyIndex(const QString &name) const{
    hkbCharacterStringData *strings = nullptr;
    if (character && character->stringData.data()){
        strings = static_cast<hkbCharacterStringData *>(character->stringData.data());
        for (int i = 0; i < strings->characterPropertyNames.size(); i++){
            if (strings->characterPropertyNames.at(i) == name){
                return i;
            }
        }
    }
    return -1;
}

int BehaviorFile::findCharacterPropertyIndexFromCharacter(int indexOfBehaviorProperty) const{
    hkbCharacterStringData *strings = nullptr;
    QString name;
    hkbBehaviorGraphStringData *behaviorstrings = nullptr;
    if (character && stringData.data()){
        strings = static_cast<hkbCharacterStringData *>(character->stringData.data());
        behaviorstrings = static_cast<hkbBehaviorGraphStringData *>(stringData.data());
        if (!behaviorstrings->characterPropertyNames.isEmpty() && indexOfBehaviorProperty < behaviorstrings->characterPropertyNames.size() && indexOfBehaviorProperty >= 0 && !strings->characterPropertyNames.isEmpty()){
            name = behaviorstrings->characterPropertyNames.at(indexOfBehaviorProperty);
            return strings->characterPropertyNames.indexOf(name);
        }
    }
    return -1;
}

QStringList BehaviorFile::getCharacterPropertyNames() const{
    if (character){
        return character->getCharacterPropertyNames();
    }
    return QStringList();
}

QStringList BehaviorFile::getAllReferencedBehaviorFilePaths() const{
    QStringList list;
    hkbBehaviorReferenceGenerator *ptr = nullptr;
    for (int i = 0; i < generators.size(); i++){
        if (generators.at(i).constData() && generators.at(i)->getSignature() == HKB_BEHAVIOR_REFERENCE_GENERATOR){
            ptr = static_cast<hkbBehaviorReferenceGenerator *>(generators.at(i).data());
            if (ptr->behaviorName != ""){
                list.append(ptr->behaviorName);
            }
        }
    }
    return list;
}

HkxObject * BehaviorFile::getBehaviorGraphData() const{
    return graphData.data();
}

int BehaviorFile::getIndexOfGenerator(const HkxSharedPtr & obj) const{
    return generators.indexOf(obj);
}

bool BehaviorFile::setGeneratorData(HkxSharedPtr & ptrToSet, int index){
    if (index >= 0 && index < generators.size()){
        ptrToSet = generators.at(index);
        return true;
    }
    return false;
}

hkbGenerator *BehaviorFile::getGeneratorDataAt(int index){
    if (index >= 0 && index < generators.size()){
        return static_cast<hkbGenerator *>(generators[index].data());
    }
    return nullptr;
}

int BehaviorFile::getIndexOfModifier(const HkxSharedPtr & obj) const{
    return modifiers.indexOf(obj);
}

bool BehaviorFile::setModifierData(HkxSharedPtr & ptrToSet, int index){
    if (index >= 0 && index < modifiers.size()){
        ptrToSet = modifiers.at(index);
        return true;
    }
    return false;
}

hkbModifier* BehaviorFile::getModifierDataAt(int index){
    if (index >= 0 && index < modifiers.size()){
        return static_cast<hkbModifier *>(modifiers[index].data());
    }
    return nullptr;
}

QStringList BehaviorFile::getVariableTypenames() const{
    if (graphData.data()){
        return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableTypeNames();
    }
    return QStringList();
}

QStringList BehaviorFile::getCharacterPropertyTypenames() const{
    if (character){
        return static_cast<CharacterFile *>(character)->getCharacterPropertyTypenames();
    }
    return QStringList();
}

hkVariableType BehaviorFile::getCharacterPropertyTypeAt(int index) const{
    if (character){
        return static_cast<CharacterFile *>(character)->getCharacterPropertyTypeAt(index);
    }
    return VARIABLE_TYPE_INT8;
}

QStringList BehaviorFile::getVariableNames() const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableNames();
}

QStringList BehaviorFile::getEventNames() const{
    return static_cast<hkbBehaviorGraphData *>(graphData.data())->getEventNames();
}

BehaviorFile::~BehaviorFile(){
    //
}
