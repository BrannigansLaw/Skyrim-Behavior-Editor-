#ifndef BSBONESWITCHGENERATORBONEDATA_H
#define BSBONESWITCHGENERATORBONEDATA_H

#include "hkbgenerator.h"

class BSBoneSwitchGeneratorBoneData: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSBoneSwitchGeneratorBoneData(BehaviorFile *parent, long ref = 0);
    virtual ~BSBoneSwitchGeneratorBoneData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
private:
    BSBoneSwitchGeneratorBoneData& operator=(const BSBoneSwitchGeneratorBoneData&);
    BSBoneSwitchGeneratorBoneData(const BSBoneSwitchGeneratorBoneData &);
private:
    static uint refCount;
    static QString classname;
    HkxObjectExpSharedPtr pGenerator;
    HkxObjectExpSharedPtr spBoneWeight;
};

#endif // BSBONESWITCHGENERATORBONEDATA_H
