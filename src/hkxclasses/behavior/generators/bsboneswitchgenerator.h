#ifndef BSBONESWITCHGENERATOR_H
#define BSBONESWITCHGENERATOR_H

#include "hkbgenerator.h"

class BSBoneSwitchGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BSBoneSwitchGeneratorUI;
public:
    BSBoneSwitchGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSBoneSwitchGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool hasGenerator() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSBoneSwitchGenerator& operator=(const BSBoneSwitchGenerator&);
    BSBoneSwitchGenerator(const BSBoneSwitchGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    HkxSharedPtr pDefaultGenerator;
    QList <HkxSharedPtr> ChildrenA;
};

#endif // BSBONESWITCHGENERATOR_H
