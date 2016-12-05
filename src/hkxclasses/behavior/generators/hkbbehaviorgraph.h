#ifndef HKBBEHAVIORGRAPH_H
#define HKBBEHAVIORGRAPH_H

#include "hkbgenerator.h"

class hkbBehaviorGraph: public hkbGenerator
{
    friend class BehaviorGraphView;
    //friend class MainWindow;
    friend class BehaviorFile;
public:
    hkbBehaviorGraph(HkxFile *parent, long ref = 0);
    virtual ~hkbBehaviorGraph();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder);
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
