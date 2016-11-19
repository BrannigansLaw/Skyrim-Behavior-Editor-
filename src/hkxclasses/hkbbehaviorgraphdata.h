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
    hkbBehaviorGraphData(BehaviorFile *parent, long ref = 0);
    virtual ~hkbBehaviorGraphData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QStringList getVariableNames() const;
    QStringList getEventNames() const;
    bool evaulateDataValidity();
    static QString getClassname();
    hkQuadVariable getQuadVariable(int index, bool *ok) const;
    hkVariableType getVariableTypeAt(int index) const;
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
