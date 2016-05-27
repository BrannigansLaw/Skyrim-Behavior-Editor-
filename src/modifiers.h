#ifndef MODIFIERS_H
#define MODIFIERS_H

#include "hkobject.h"

class hkbModifier: public HkDynamicObject
{
public:
    virtual ~hkbModifier(){}
    bool link(){return true;}
protected:
    hkbModifier(BehaviorFile *parent = NULL/*, long ref = 0*/): HkDynamicObject(parent/*, ref*/){}
};


#endif // MODIFIERS_H
