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
    QString evaluateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon(HkxObject *child) const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool merge(HkxObject *recessiveObject);
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
    QList <HkxSharedPtr> generators;
    qint8 selectedGeneratorIndex;
    qint8 currentGeneratorIndex;
};

#endif // HKBMANUALSELECTORGENERATOR_H
