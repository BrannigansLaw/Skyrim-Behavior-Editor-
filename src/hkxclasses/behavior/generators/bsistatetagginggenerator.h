#ifndef BSISTATETAGGINGGENERATOR_H
#define BSISTATETAGGINGGENERATOR_H

#include "hkbgenerator.h"

class BSiStateTaggingGenerator: public hkbGenerator
{
    friend class BSiStateTaggingGeneratorUI;
    friend class BehaviorGraphView;
    friend class hkbStateMachine;
public:
    BSiStateTaggingGenerator(HkxFile *parent, long ref = 0);
    virtual ~BSiStateTaggingGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSiStateTaggingGenerator& operator=(const BSiStateTaggingGenerator&);
    BSiStateTaggingGenerator(const BSiStateTaggingGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxSharedPtr pDefaultGenerator;
    int iStateToSetAs;
    int iPriority;
};

#endif // BSISTATETAGGINGGENERATOR_H
