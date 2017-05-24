#ifndef HKBBEHAVIORGRAPHSTRINGDATA_H
#define HKBBEHAVIORGRAPHSTRINGDATA_H

#include <QStringList>

#include "src/hkxclasses/hkxobject.h"

class BehaviorFile;

class hkbBehaviorGraphStringData: public HkxObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
    friend class EventsUI;
    friend class BehaviorFile;
public:
    hkbBehaviorGraphStringData(HkxFile *parent, long ref = 0);
    virtual ~hkbBehaviorGraphStringData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    QString getVariableNameAt(int index) const;
    QString getEventNameAt(int index) const;
    int getCharacterPropertyIndex(const QString &name) const;
private:
    hkbBehaviorGraphStringData& operator=(const hkbBehaviorGraphStringData&);
    hkbBehaviorGraphStringData(const hkbBehaviorGraphStringData &);
private:
    static uint refCount;
    static QString classname;
    QStringList eventNames;
    QStringList attributeNames;
    QStringList variableNames;
    QStringList characterPropertyNames;
};

#endif // HKBBEHAVIORGRAPHSTRINGDATA_H
