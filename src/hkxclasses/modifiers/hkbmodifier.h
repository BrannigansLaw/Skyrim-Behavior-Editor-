#ifndef HKBMODIFIER_H
#define HKBMODIFIER_H

#include "src/hkxclasses/hkxobject.h"

class hkbModifier: public HkDynamicObject
{
public:
    virtual ~hkbModifier();
    bool link();
protected:
    hkbModifier(BehaviorFile *parent = NULL/*, long ref = 0*/);
};

#endif // HKBMODIFIER_H
