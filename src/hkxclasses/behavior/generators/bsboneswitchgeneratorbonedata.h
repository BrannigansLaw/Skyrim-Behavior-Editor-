#ifndef BSBONESWITCHGENERATORBONEDATA_H
#define BSBONESWITCHGENERATORBONEDATA_H

#include "hkbgenerator.h"

class BSBoneSwitchGeneratorBoneData: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BSBoneSwitchGenerator;
public:
    BSBoneSwitchGeneratorBoneData(HkxFile *parent, long ref = 0);
    virtual ~BSBoneSwitchGeneratorBoneData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSBoneSwitchGeneratorBoneData& operator=(const BSBoneSwitchGeneratorBoneData&);
    BSBoneSwitchGeneratorBoneData(const BSBoneSwitchGeneratorBoneData &);
private:
    static uint refCount;
    static QString classname;
    HkxSharedPtr pGenerator;
    HkxSharedPtr spBoneWeight;
};

#endif // BSBONESWITCHGENERATORBONEDATA_H
