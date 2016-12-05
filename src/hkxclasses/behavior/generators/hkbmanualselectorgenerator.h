#ifndef HKBMANUALSELECTORGENERATOR_H
#define HKBMANUALSELECTORGENERATOR_H

#include "hkbgenerator.h"

class hkbManualSelectorGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class ManualSelectorGeneratorUI;
public:
    hkbManualSelectorGenerator(HkxFile *parent, long ref = 0);
    virtual ~hkbManualSelectorGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon(HkxObject *child) const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder = true);
    hkbManualSelectorGenerator& operator=(const hkbManualSelectorGenerator&);
    hkbManualSelectorGenerator(const hkbManualSelectorGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    QList <HkxObjectExpSharedPtr> generators;
    qint8 selectedGeneratorIndex;
    qint8 currentGeneratorIndex;
};

#endif // HKBMANUALSELECTORGENERATOR_H
