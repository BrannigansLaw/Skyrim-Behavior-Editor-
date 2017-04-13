#ifndef BSMODIFYONCEMODIFIER_H
#define BSMODIFYONCEMODIFIER_H

#include "hkbmodifier.h"

class BSModifyOnceModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSModifyOnceModifier(HkxFile *parent, long ref = 0);
    virtual ~BSModifyOnceModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getIndexToInsertIcon() const;
    bool hasChildren() const;
protected:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
private:
    BSModifyOnceModifier& operator=(const BSModifyOnceModifier&);
    BSModifyOnceModifier(const BSModifyOnceModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxObjectExpSharedPtr pOnActivateModifier;
    HkxObjectExpSharedPtr pOnDeactivateModifier;
};

#endif // BSMODIFYONCEMODIFIER_H
