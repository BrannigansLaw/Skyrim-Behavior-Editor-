#ifndef BSBONESWITCHGENERATORBONEDATA_H
#define BSBONESWITCHGENERATORBONEDATA_H

#include "hkbgenerator.h"

class BSBoneSwitchGeneratorBoneData: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class BSBoneSwitchGenerator;
    friend class BSBoneSwitchGeneratorUI;
    friend class BSBoneSwitchGeneratorBoneDataUI;
public:
    BSBoneSwitchGeneratorBoneData(HkxFile *parent, hkbGenerator *parentBSG, long ref = 0);
    virtual ~BSBoneSwitchGeneratorBoneData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    QString getName() const;
    int getThisIndex() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    BSBoneSwitchGeneratorBoneData& operator=(const BSBoneSwitchGeneratorBoneData&);
    BSBoneSwitchGeneratorBoneData(const BSBoneSwitchGeneratorBoneData &);
private:
    static uint refCount;
    static QString classname;
    HkxSharedPtr parentBSG;
    HkxSharedPtr pGenerator;
    HkxSharedPtr spBoneWeight;
};

#endif // BSBONESWITCHGENERATORBONEDATA_H
