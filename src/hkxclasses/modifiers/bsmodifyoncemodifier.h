#ifndef BSMODIFYONCEMODIFIER_H
#define BSMODIFYONCEMODIFIER_H

#include "hkbmodifier.h"

class BSModifyOnceModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSModifyOnceModifier(BehaviorFile *parent, long ref = 0);
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
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder = true);
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
