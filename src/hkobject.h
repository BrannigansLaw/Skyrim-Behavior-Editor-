#ifndef HKOBJECT_H
#define HKOBJECT_H

#include "utility.h"
#
class HkxFile;
class HkxXmlReader;
class BehaviorFile;
class HkObjectExpSharedPtr;
class GeneratorIcon;

class HkObject: public QSharedData
{
public:
    enum HkxType {TYPE_OTHER=0, TYPE_GENERATOR=1, TYPE_MODIFIER=2};
public:
    virtual ~HkObject(){}
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);
    qint64 getSignature() const{
        return signature;
    }
    HkxType getType()const{
        return typeCheck;
    }
    virtual bool link() = 0;
protected:
    HkObject(BehaviorFile *parent = NULL/*, long ref = 0*/)
        : parentFile(parent),
          reference(0),
          isDataValid(true)
        /*, reference(ref)*/
    {
        //
    }
    BehaviorFile * getParentFile() const{
        return parentFile;
    }
    void setType(HkxSignature sig, HkxType type){
        signature = sig;typeCheck = type;
    }
    void setDataValidity(bool isValid){
        isDataValid = isValid;
    }

    bool readIntegers(const QByteArray &line, QVector<qint16> & ints);
    bool toBool(const QByteArray &line, bool *ok);
    bool readDoubles(const QByteArray &line, QVector<qreal> & doubles);
    hkVector3 readVector3(const QByteArray &lineIn, bool *ok);
    hkVector4 readVector4(const QByteArray &lineIn, bool *ok);
private:
    HkObject(const HkObject &obj);
    HkObject& operator=(const HkObject&);
private:
    BehaviorFile *parentFile;
    long reference;
    HkxSignature signature;
    HkxType typeCheck;
    bool isDataValid;
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

/*class hkbStateMachineEventPropertyArray: public HkObject
{
public:
    hkbStateMachineEventPropertyArray(BehaviorFile *parent = NULL): HkObject(parent){refCount++;setType(HKB_STATE_MACHINE_EVENT_PROPERTY_ARRAY, TYPE_OTHER);}
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
    hkbStateMachineTransitionInfoArray(BehaviorFile *parent = NULL): HkObject(parent){refCount++;setType(HKB_STATE_MACHINE_TRANSITION_INFO_ARRAY, TYPE_OTHER);}
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
};*/

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
