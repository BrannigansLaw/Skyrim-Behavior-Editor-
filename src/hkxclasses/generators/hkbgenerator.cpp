#include "hkbgenerator.h"

hkbGenerator::hkbGenerator(BehaviorFile *parent/*, long ref = 0*/)
    : HkDynamicObject(parent/*, ref*/)
{
    //
}

bool hkbGenerator::link(){
    return true;
}

QString hkbGenerator::getName() const{
    return "";
}

void hkbGenerator::appendIcon(GeneratorIcon *icon){
    icons.append(icon);
}

hkbGenerator::~hkbGenerator(){
    //
}
