#ifndef BSBONESWITCHGENERATOR_H
#define BSBONESWITCHGENERATOR_H

#include "hkbgenerator.h"

class BSBoneSwitchGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSBoneSwitchGenerator(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSBoneSwitchGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    BSBoneSwitchGenerator& operator=(const BSBoneSwitchGenerator&);
    BSBoneSwitchGenerator(const BSBoneSwitchGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    HkxObjectExpSharedPtr pDefaultGenerator;
    QList <HkxObjectExpSharedPtr> ChildrenA;
};

#endif // BSBONESWITCHGENERATOR_H
