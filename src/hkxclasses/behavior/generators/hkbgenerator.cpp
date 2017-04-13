#include "hkbgenerator.h"
#include "src/ui/treegraphicsitem.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/dataiconmanager.h"

hkbGenerator::hkbGenerator(HkxFile *parent, long ref)
    : DataIconManager(parent, ref)
{
    //
}

bool hkbGenerator::link(){
    return true;
}

QString hkbGenerator::getName() const{
    return "";
}

QString hkbGenerator::getClassname() const{
    switch (getSignature()){
    case HKB_STATE_MACHINE:
        return "hkbStateMachine";
    case HKB_MANUAL_SELECTOR_GENERATOR:
        return "hkbManualSelectorGenerator";
    case HKB_BLENDER_GENERATOR:
        return "hkbBlenderGenerator";
    case BS_I_STATE_TAGGING_GENERATOR:
        return "BSiStateTaggingGenerator";
    case BS_BONE_SWITCH_GENERATOR:
        return "BSBoneSwitchGenerator";
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        return "BSCyclicBlendTransitionGenerator";
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
        return "BSSynchronizedClipGenerator";
    case HKB_MODIFIER_GENERATOR:
        return "hkbModifierGenerator";
    case BS_OFFSET_ANIMATION_GENERATOR:
        return "BSOffsetAnimationGenerator";
    case HKB_POSE_MATCHING_GENERATOR:
        return "hkbPoseMatchingGenerator";
    case HKB_CLIP_GENERATOR:
        return "hkbClipGenerator";
    case HKB_BEHAVIOR_REFERENCE_GENERATOR:
        return "hkbBehaviorReferenceGenerator";
    default:
        return "";
    }
}

hkbGenerator::~hkbGenerator(){
    //
}
