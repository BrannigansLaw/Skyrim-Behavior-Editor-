#ifndef HKOBJECT_H
#define HKOBJECT_H

#include "utility.h"
#
class HkxFile;
class HkxXmlReader;
class BehaviorFile;
class HkObjectExpSharedPtr;

class HkObject: public QSharedData
{
public:
    enum HkxType {TYPE_OTHER=0, TYPE_GENERATOR=1, TYPE_MODIFIER=2};
public:
    virtual ~HkObject(){}
    qint64 getSignature(){return signature;}
    HkxType getType()const{return typeCheck;}
    virtual bool link() = 0;
    //virtual bool readData(const HkxXmlReader & reader, int index) = 0;
protected:
    HkObject(BehaviorFile *parent = NULL/*, long ref = 0*/): parentFile(parent)/*, reference(ref)*/{}
    BehaviorFile * getParentFile(){return parentFile;}
    //void setRef(long ref){reference = ref;}
    void setType(HkxSignature sig, HkxType type){signature = sig;typeCheck = type;}
    bool readIntegers(const QByteArray &line, QVector<qint16> & ints);
    bool toBool(const QByteArray &line, bool *ok);
    //qint16 readSingleInt(const QByteArray &line, bool *ok);
    //qreal readSingleDouble(const QByteArray &line, bool *ok);
    bool readDoubles(const QByteArray &line, QVector<qreal> & doubles);
    //qint16 readNumberElements(const QByteArray &line, bool *ok);
    //qint16 readReference(const QByteArray &lineIn, bool *ok);
    //virtual bool readReferences() = 0;
    //bool readReferences(const QByteArray &line, QList <HkObjectExpSharedPtr> & refs);
    //virtual bool readData(const HkxXmlReader & reader, int index){return true;}
    hkVector3 readVector3(const QByteArray &lineIn, bool *ok);
    hkVector4 readVector4(const QByteArray &lineIn, bool *ok);
private:
    HkObject(const HkObject &obj);
    HkObject& operator=(const HkObject&);
private:
    BehaviorFile *parentFile;
    //long reference;
    HkxSignature signature;
    HkxType typeCheck;
};

class HkObjectExpSharedPtr: public QExplicitlySharedDataPointer <HkObject>
{
public:
    HkObjectExpSharedPtr(HkObject *obj = NULL, long ref = -1)
        :QExplicitlySharedDataPointer(obj), reference(ref)
    {
        //
    }
    void setReference(long ref){reference = ref;}
    long getReference()const{return reference;}
    bool readReference(long index, const HkxXmlReader & reader);
private:
    long reference;
};

struct HkEvent
{
    int id;
    HkObjectExpSharedPtr payload;
};

class hkbStateMachineEventPropertyArray: public HkObject
{
public:
    hkbStateMachineEventPropertyArray(BehaviorFile *parent = NULL/*, long ref = 0*/): HkObject(parent/*, ref*/){refCount++;setType(HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY, TYPE_OTHER);}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
protected:
private:
    static uint refCount;
    QVector <HkEvent> events;
};

class hkbStateMachineTransitionInfoArray: public HkObject
{
public:
    hkbStateMachineTransitionInfoArray(BehaviorFile *parent = NULL/*, long ref = 0*/): HkObject(parent/*, ref*/){refCount++;setType(HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY, TYPE_OTHER);}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
protected:
private:
    struct HkTransition
    {
        HkTransition(): eventId(-1), toStateId(0), fromNestedStateId(0), toNestedStateId(0), priority(0){}

        struct HkInterval
        {
            HkInterval(): enterEventId(-1), exitEventId(-1), enterTime(0), exitTime(0){}
            int enterEventId;
            int exitEventId;
            qreal enterTime;
            qreal exitTime;
        };

        HkInterval triggerInterval;
        HkInterval initiateInterval;
        HkObjectExpSharedPtr transition;
        HkObjectExpSharedPtr condition;
        int eventId;
        int toStateId;
        int fromNestedStateId;
        int toNestedStateId;
        int priority;
        QString flags;
    };

    static uint refCount;
    QVector <HkTransition> transitions;
};

class hkbStringEventPayload: public HkObject
{
public:
    hkbStringEventPayload(BehaviorFile *parent = NULL/*, long ref = 0*/): HkObject(parent/*, ref*/){refCount++;setType(HKB_STRING_EVENT_PAYLOAD, TYPE_OTHER);}
protected:
private:
    static uint refCount;
    QString data;
};

class HkDynamicObject: public HkObject
{
public:
    virtual ~HkDynamicObject(){}
    bool linkVar();
protected:
    HkDynamicObject(BehaviorFile *parent = NULL/*, long ref = 0*/): HkObject(parent/*, ref*/){}
protected:
    HkObjectExpSharedPtr variableBindingSet;
protected:
    //bool link();
private:
    HkDynamicObject& operator=(const HkDynamicObject&);
    HkDynamicObject(const HkDynamicObject &);
private:
    //HkObjectExpSharedPtr variableBindingSet;
};

#endif // HKOBJECT_H
