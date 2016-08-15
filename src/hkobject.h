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
    void setDataValidity(bool isValid){
        isDataValid = isValid;
    }
    bool getIsDataValid()const{
        return isDataValid;
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

    QList<HkObjectExpSharedPtr> getChildGenerators() const;
    bool readMultipleVector4(const QByteArray &lineIn,  QVector <hkVector4> & vectors);
    bool readReferences(const QByteArray &line, QList <HkObjectExpSharedPtr> & children);
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

class hkbBehaviorGraphStringData: public HkObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
public:
    hkbBehaviorGraphStringData(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraphStringData(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link(){return true;}
private:
    hkbBehaviorGraphStringData& operator=(const hkbBehaviorGraphStringData&);
    hkbBehaviorGraphStringData(const hkbBehaviorGraphStringData &);
private:
    static uint refCount;
    QStringList eventNames;
    QStringList attributeNames;
    QStringList variableNames;
    QStringList characterPropertyNames;
};

class hkbVariableValueSet: public HkObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
public:
    hkbVariableValueSet(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbVariableValueSet(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbVariableValueSet& operator=(const hkbVariableValueSet&);
    hkbVariableValueSet(const hkbVariableValueSet &);
private:
    static uint refCount;
    QVector <int> wordVariableValues;
    QVector <hkVector4> quadVariableValues;
    QList <HkObjectExpSharedPtr> variantVariableValues;
};

class hkbBehaviorGraphData: public HkObject
{
    friend class BehaviorGraphView;
public:
    enum hkVariableType {
        VARIABLE_TYPE_BOOL,
        VARIABLE_TYPE_INT8,
        VARIABLE_TYPE_INT16,
        VARIABLE_TYPE_INT32,
        VARIABLE_TYPE_REAL,
        VARIABLE_TYPE_POINTER,
        VARIABLE_TYPE_VECTOR4,
        VARIABLE_TYPE_QUATERNION
    };
    hkbBehaviorGraphData(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraphData(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QStringList & getVariableNames() const{
        return static_cast<hkbBehaviorGraphStringData *>(stringData.data())->variableNames;
    }
private:
    hkbBehaviorGraphData& operator=(const hkbBehaviorGraphData&);
    hkbBehaviorGraphData(const hkbBehaviorGraphData &);
private:
    static QStringList Type;    //See hkVariableType...
    struct hkVariableInfo
    {
        hkVariableInfo(){}
        struct hkRole
        {
            QString role;
            QString flags;
        };

        hkRole role;
        QString type;
    };

    static uint refCount;
    //QVector <??> attributeDefaults;
    QVector <hkVariableInfo> variableInfos;
    QVector <hkVariableInfo> characterPropertyInfos;
    QStringList eventInfos;
    QVector <int> wordMinVariableValues;
    QVector <int> wordMaxVariableValues;
    HkObjectExpSharedPtr variableInitialValues;
    HkObjectExpSharedPtr stringData;
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

class hkbBoneWeightArray: public HkDynamicObject
{
    friend class BehaviorGraphView;
public:
    hkbBoneWeightArray(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBoneWeightArray(){refCount--;}
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
private:
    hkbBoneWeightArray& operator=(const hkbBoneWeightArray&);
    hkbBoneWeightArray(const hkbBoneWeightArray &);
private:
    static uint refCount;
    QVector <qreal> boneWeights;
};

#endif // HKOBJECT_H
