#ifndef HKBBEHAVIORGRAPHDATA_H
#define HKBBEHAVIORGRAPHDATA_H

#include <QVector>

#include "src/hkxclasses/hkxobject.h"

class hkbBehaviorGraphData: public HkxObject
{
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
    friend class EventsUI;
    friend class BehaviorFile;
    friend class hkbVariableBindingSet;
public:
    hkbBehaviorGraphData(HkxFile *parent, long ref = 0);
    virtual ~hkbBehaviorGraphData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QStringList getVariableNames() const;
    QStringList getEventNames() const;
    int getNumberOfEvents() const;
    int getNumberOfVariables() const;
    QString evaluateDataValidity();
    static QString getClassname();
    hkQuadVariable getQuadVariable(int index, bool *ok) const;
    HkxObject * getVariantVariable(int index) const;
    hkVariableType getCharacterPropertyTypeAt(int index) const;
    QString getCharacterPropertyNameAt(int index) const;
    hkVariableType getVariableTypeAt(int index) const;
    QString getVariableNameAt(int index) const;
    QString getEventNameAt(int index) const;
    bool write(HkxXMLWriter *writer);
    QStringList getVariableTypeNames() const;
    int getIndexOfVariable(const QString & name) const;
    int getIndexOfEvent(const QString & name) const;
    void addEvent(const QString &name);
    int addVariable(hkVariableType type, const QString & name, bool isProperty = false);
    int addVariable(const QString & type, const QString & name, bool isProperty = false);
    void addVariable(hkVariableType type);
private:
    hkbBehaviorGraphData& operator=(const hkbBehaviorGraphData&);
    hkbBehaviorGraphData(const hkbBehaviorGraphData &);
private:
    bool merge(HkxObject *recessiveobj);
    void removeVariable(int index);
    void setVariableNameAt(int index, const QString & name);
    void setWordVariableValueAt(int index, int value);
    void setQuadVariableValueAt(int index, hkQuadVariable value);
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
            hkRole(): role("ROLE_DEFAULT"), flags("0"){}
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
    HkxSharedPtr variableInitialValues;
    HkxSharedPtr stringData;
};

#endif // HKBBEHAVIORGRAPHDATA_H
