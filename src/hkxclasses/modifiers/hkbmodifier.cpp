#include "hkbmodifier.h"

hkbModifier::hkbModifier(BehaviorFile *parent/*, long ref = 0*/)
    : HkDynamicObject(parent/*, ref*/)
{
    //
}

bool hkbModifier::link(){
    return true;
}

hkbModifier::~hkbModifier(){
    //
}
