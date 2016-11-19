#include "hkbmodifier.h"

hkbModifier::hkbModifier(BehaviorFile *parent, long ref)
    : HkDynamicObject(parent, ref)
{
    //
}

bool hkbModifier::link(){
    return true;
}

hkbModifier::~hkbModifier(){
    //
}
