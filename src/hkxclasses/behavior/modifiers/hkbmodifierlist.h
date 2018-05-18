#ifndef HKBMODIFIERLIST_H
#define HKBMODIFIERLIST_H

#include "hkbmodifier.h"

class hkbModifierList: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class ModifierListUI;
public:
    hkbModifierList(HkxFile *parent, long ref = 0);
    virtual ~hkbModifierList();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon() const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool merge(HkxObject *recessiveObject);
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbModifierList& operator=(const hkbModifierList&);
    hkbModifierList(const hkbModifierList &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    QList <HkxSharedPtr> modifiers;
};

#endif // HKBMODIFIERLIST_H
