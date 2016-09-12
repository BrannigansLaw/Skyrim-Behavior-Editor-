#ifndef HKBBEHAVIORGRAPHDATA_H
#define HKBBEHAVIORGRAPHDATA_H

#include <QVector>

#include "hkxobject.h"

class hkbBehaviorGraphData: public HkxObject
{
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
    friend class EventsUI;
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
    virtual ~hkbBehaviorGraphData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QStringList getVariableNames() const;
    QStringList getEventNames() const;
    bool evaulateDataValidity();
    static QString getClassname();
    hkQuadVariable getQuadVariable(int index, bool *ok);
private:
    hkbBehaviorGraphData& operator=(const hkbBehaviorGraphData&);
    hkbBehaviorGraphData(const hkbBehaviorGraphData &);
private:
    void addVariable(hkVariableType type, const QString & name);
    void addVariable(hkVariableType type);
    void removeVariable(int index);
    void setVariableNameAt(int index, const QString & name);
    void setWordVariableValueAt(int index, int value);
    void setQuadVariableValueAt(int index, hkQuadVariable value);
    void addEvent(const QString &name);
    void addEvent();
    void removeEvent(int index);
    void setEventNameAt(int index, const QString & name);
    void setEventFlagAt(int index, bool state);
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
    static QString classname;
    //QVector <??> attributeDefaults;
    QVector <hkVariableInfo> variableInfos;
    QVector <hkVariableInfo> characterPropertyInfos;
    QStringList eventInfos;
    QVector <int> wordMinVariableValues;
    QVector <int> wordMaxVariableValues;
    HkxObjectExpSharedPtr variableInitialValues;
    HkxObjectExpSharedPtr stringData;
};


#endif // HKBBEHAVIORGRAPHDATA_H
