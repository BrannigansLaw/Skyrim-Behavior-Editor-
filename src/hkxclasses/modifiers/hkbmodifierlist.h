#ifndef HKBMODIFIERLIST_H
#define HKBMODIFIERLIST_H

#include "hkbmodifier.h"

class hkbModifierList: public DataIconManager
{
    friend class BehaviorGraphView;
public:
    hkbModifierList(BehaviorFile *parent, long ref = 0);
    virtual ~hkbModifierList();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(hkbGenerator *objToAppend);
    bool removeObject(hkbGenerator *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder = true);
    hkbModifierList& operator=(const hkbModifierList&);
    hkbModifierList(const hkbModifierList &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    QList <HkxObjectExpSharedPtr> modifiers;
};

#endif // HKBMODIFIERLIST_H
