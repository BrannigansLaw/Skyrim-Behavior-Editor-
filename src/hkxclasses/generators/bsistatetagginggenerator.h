#ifndef BSISTATETAGGINGGENERATOR_H
#define BSISTATETAGGINGGENERATOR_H

#include "hkbgenerator.h"

class BSiStateTaggingGenerator: public hkbGenerator
{
    friend class BSiStateTaggingGeneratorUI;
    friend class BehaviorGraphView;
public:
    BSiStateTaggingGenerator(BehaviorFile *parent/*, qint16 ref = 0*/);
    virtual ~BSiStateTaggingGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    BSiStateTaggingGenerator& operator=(const BSiStateTaggingGenerator&);
    BSiStateTaggingGenerator(const BSiStateTaggingGenerator &);
private:
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    HkxObjectExpSharedPtr pDefaultGenerator;
    int iStateToSetAs;
    int iPriority;
};

#endif // BSISTATETAGGINGGENERATOR_H
