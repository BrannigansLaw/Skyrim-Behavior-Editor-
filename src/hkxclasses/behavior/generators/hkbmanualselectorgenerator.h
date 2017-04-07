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
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
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
