#ifndef MODIFIERS_H
#define MODIFIERS_H

#include "hkobject.h"

class hkbModifier: public HkDynamicObject
{
public:
    virtual ~hkbModifier(){}
protected:
    hkbModifier(HkxFile *parent = NULL/*, long ref = 0*/): HkDynamicObject(parent/*, ref*/){}
};

class HkbModifierExpSharedPtr: public QSharedDataPointer <hkbModifier>
{
public:
    HkbModifierExpSharedPtr(hkbModifier *obj = NULL, long ref = -1):QSharedDataPointer(obj), reference(ref){}
    void setReference(long ref){reference = ref;}
    long getReference()const{return reference;}
private:
    long reference;
};


#endif // MODIFIERS_H
