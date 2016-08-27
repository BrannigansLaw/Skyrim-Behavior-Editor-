#ifndef BSBONESWITCHGENERATORBONEDATA_H
#define BSBONESWITCHGENERATORBONEDATA_H

#include "hkbgenerator.h"

class BSBoneSwitchGeneratorBoneData: public hkbGenerator
{
    friend class BehaviorGraphView;
public:
    BSBoneSwitchGeneratorBoneData(BehaviorFile *parent = NULL/*, qint16 ref = 0*/);
    virtual ~BSBoneSwitchGeneratorBoneData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
private:
    BSBoneSwitchGeneratorBoneData& operator=(const BSBoneSwitchGeneratorBoneData&);
    BSBoneSwitchGeneratorBoneData(const BSBoneSwitchGeneratorBoneData &);
private:
    static uint refCount;
    HkxObjectExpSharedPtr pGenerator;
    HkxObjectExpSharedPtr spBoneWeight;
};

#endif // BSBONESWITCHGENERATORBONEDATA_H
