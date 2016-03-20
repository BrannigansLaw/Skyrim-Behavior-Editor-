#ifndef HKOBJECT_H
#define HKOBJECT_H

#include "utility.h"

class HkxFile;
class HkxXmlReader;

class HkObject: public QSharedData
{
public:
    enum HkxType {TYPE_OTHER=0, TYPE_GENERATOR=1, MODIFIER=2};
public:
    virtual ~HkObject(){}
    qint64 getSignature(){return signature;}
    //virtual bool readData(const HkxXmlReader & reader, int startIndex){return false;}
protected:
    HkObject(HkxFile *parent = NULL, qint32 ref = 0): parentFile(parent), reference(ref){}
    void setType(HkxSignature sig, HkxType type){signature = sig;typeCheck = type;}
    bool readIntegers(const QByteArray &line, QVector<qint16> & ints);
    bool readSingleBool(const QByteArray &line, bool *ok);
    qint16 readSingleInt(const QByteArray &line, bool *ok);
    qreal readSingleDouble(const QByteArray &line, bool *ok);
    bool readDoubles(const QByteArray &line, QVector<qreal> & doubles);
    qint16 readNumberElements(const QByteArray &line, bool *ok);
    qint16 readReference(const QByteArray &lineIn, bool *ok);
    bool readReferences(const QByteArray &line, QVector<qint16> & refs);
    hkVector3 readVector3(const QByteArray &lineIn, bool *ok);
    hkVector4 readVector4(const QByteArray &lineIn, bool *ok);
private:
    HkObject(const HkObject &obj);
    HkObject& operator=(const HkObject&);
private:
    HkxFile *parentFile;
    qint32 reference;
    HkxSignature signature;
    HkxType typeCheck;
};

class HkObjectExpSharedPtr: public QSharedDataPointer <HkObject>
{
public:
    HkObjectExpSharedPtr(HkObject *obj = NULL, qint32 ref = -1):QSharedDataPointer(obj), reference(ref){}
    void setReference(qint32 ref){reference = ref;}
    qint32 getReference()const{return reference;}
private:
    qint32 reference;
};

/*class HkbModifierExpSharedPtr: public QSharedDataPointer <hkbModifier>
{
public:
    HkbModifierExpSharedPtr(qint32 ref = -1): reference(ref){}
    void setReference(qint32 ref){reference = ref;}
    qint32 getReference()const{return reference;}
private:
    qint32 reference;
};*/

class hkRootLevelContainer: public HkObject
{
public:
    hkRootLevelContainer(HkxFile *parent = NULL, qint32 ref = 0): HkObject(parent, ref){refCount++;setType(HK_ROOT_LEVEL_CONTAINER, TYPE_OTHER);}
    virtual ~hkRootLevelContainer(){refCount--;}
    bool readData(const HkxXmlReader & reader, int startIndex);
private:
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
private:
    static uint refCount;
    struct hkRootLevelContainerNamedVariant
    {
        hkRootLevelContainerNamedVariant(QString varname = "default", QString classname = "default"):name(varname), className(classname){}
        QString name;
        QString className;
        HkObjectExpSharedPtr variant;  //This can be one of many (any?) types.
    };
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

class HkDynamicObject: public HkObject
{
public:
    virtual ~HkDynamicObject(){}
protected:
    HkDynamicObject(HkxFile *parent = NULL, qint32 ref = 0): HkObject(parent, ref){}
private:
    HkDynamicObject& operator=(const HkDynamicObject&);
    HkDynamicObject(const HkDynamicObject &);
private:
    HkObjectExpSharedPtr variableBindingSet;
};

#endif // HKOBJECT_H
