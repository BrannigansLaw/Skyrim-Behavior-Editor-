#ifndef HKBBEHAVIORGRAPHSTRINGDATA_H
#define HKBBEHAVIORGRAPHSTRINGDATA_H

#include <QStringList>

#include "hkxobject.h"

class BehaviorFile;

class hkbBehaviorGraphStringData: public HkxObject
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
public:
    hkbBehaviorGraphStringData(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraphStringData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
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

#endif // HKBBEHAVIORGRAPHSTRINGDATA_H
