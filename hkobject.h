#ifndef HKOBJECT_H
#define HKOBJECT_H

#include "utility.h"

class HkxFile;

class HkObject
{
public:
    enum TypeFlag {TYPE_OTHER=0x0, TYPE_GENERATOR=0x1, MODIFIER=0x2};
public:
    HkObject(HkxFile *parent = NULL, qint32 ref = 0, qint64 sig = 0, TypeFlag type = TYPE_GENERATOR): parentFile(parent), reference(ref), signature(sig), typeFlag(type){}
    virtual ~HkObject(){}
    qint64 getSignature(){return signature;}
private:
    HkObject(const HkObject &obj);
    HkObject& operator=(const HkObject&);
private:
    HkxFile *parentFile;
    qint32 reference;
    qint64 signature;
    TypeFlag typeFlag;
};

class hkRootLevelContainer: public HkObject
{
public:
    hkRootLevelContainer(HkxFile *parent = NULL, qint32 ref = 0): HkObject(parent, ref){refCount++;}
    virtual ~hkRootLevelContainer(){refCount--;}
private:
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
private:
    static uint refCount;
    struct hkRootLevelContainerNamedVariant
    {
        hkRootLevelContainerNamedVariant(){}
        QString name;
        QString className;
        HkObjectSmartPointer variant;  //This can be one of many (any?) types.
    };
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

class HkDynamicObject: public HkObject
{
public:
    HkDynamicObject(HkxFile *parent = NULL, qint32 ref = 0): HkObject(parent, ref){}
    virtual ~HkDynamicObject(){}
private:
    HkDynamicObject& operator=(const HkDynamicObject&);
    HkDynamicObject(const HkDynamicObject &);
private:
    HkObjectSmartPointer variableBindingSet;
};

#endif // HKOBJECT_H
