#ifndef BGSGAMEBRYOSEQUENCEGENERATOR_H
#define BGSGAMEBRYOSEQUENCEGENERATOR_H

#include "hkbgenerator.h"

class BGSGamebryoSequenceGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BGSGamebryoSequenceGeneratorUI;
public:
    BGSGamebryoSequenceGenerator(HkxFile *parent, long ref = 0);
    virtual ~BGSGamebryoSequenceGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void removeData(){}
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BGSGamebryoSequenceGenerator& operator=(const BGSGamebryoSequenceGenerator&);
    BGSGamebryoSequenceGenerator(const BGSGamebryoSequenceGenerator &);
private:
    static QStringList BlendModeFunction;   //(BMF_NONE=0;BMF_PERCENT=1;BMF_ONE_MINUS_PERCENT=2)
    static uint refCount;
    static QString classname;
    ulong userData;
    QString name;
    QString pSequence;
    QString eBlendModeFunction;
    qreal fPercent;
};

#endif // BGSGAMEBRYOSEQUENCEGENERATOR_H
