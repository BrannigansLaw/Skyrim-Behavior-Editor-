#ifndef HKBBEHAVIORGRAPH_H
#define HKBBEHAVIORGRAPH_H

#include "hkbgenerator.h"

class hkbBehaviorGraph: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorGraph(BehaviorFile *parent/*, qint16 ref = 0*/);
    virtual ~hkbBehaviorGraph();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    hkbBehaviorGraph& operator=(const hkbBehaviorGraph&);
    hkbBehaviorGraph(const hkbBehaviorGraph &);
private:
    static uint refCount;
    static QString classname;
    static QStringList VariableMode;    //{VARIABLE_MODE_DISCARD_WHEN_INACTIVE=0, VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE=1};
    qint64 userData;
    QString name;
    QString variableMode;
    HkxObjectExpSharedPtr rootGenerator;
    HkxObjectExpSharedPtr data;
};

#endif // HKBBEHAVIORGRAPH_H
