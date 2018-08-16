#ifndef HKBMODIFIERLIST_H
#define HKBMODIFIERLIST_H

#include "hkbmodifier.h"

class hkbModifierList final: public hkbModifier
{
    friend class ModifierListUI;
public:
    hkbModifierList(HkxFile *parent, long ref = 0);
    hkbModifierList& operator=(const hkbModifierList&) = delete;
    hkbModifierList(const hkbModifierList &) = delete;
    ~hkbModifierList();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    bool merge(HkxObject *recessiveObject);
    QVector <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    QVector <HkxSharedPtr> modifiers;
    mutable std::mutex mutex;
};

#endif // HKBMODIFIERLIST_H
