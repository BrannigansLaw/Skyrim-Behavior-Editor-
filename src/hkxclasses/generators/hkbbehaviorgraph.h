#ifndef HKBBEHAVIORGRAPH_H
#define HKBBEHAVIORGRAPH_H

#include "hkbgenerator.h"

class hkbBehaviorGraph: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    hkbBehaviorGraph(BehaviorFile *parent, long ref = 0);
    virtual ~hkbBehaviorGraph();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(hkbGenerator *objToAppend);
    bool removeObject(hkbGenerator *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder);
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
