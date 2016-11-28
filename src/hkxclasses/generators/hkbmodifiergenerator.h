#ifndef HKBMODIFIERGENERATOR_H
#define HKBMODIFIERGENERATOR_H

#include "hkbgenerator.h"

//Do not allow variables to be bound to this class in the editor...
class hkbModifierGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class ModifierGeneratorUI;
    friend class hkbStateMachine;
public:
    hkbModifierGenerator(BehaviorFile *parent, long ref = 0);
    virtual ~hkbModifierGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon(HkxObject *child) const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index = 0);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList <HkxObjectExpSharedPtr> & list, bool reverseOrder = true);
    hkbModifierGenerator& operator=(const hkbModifierGenerator&);
    hkbModifierGenerator(const hkbModifierGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    HkxObjectExpSharedPtr modifier;
    HkxObjectExpSharedPtr generator;
};


#endif // HKBMODIFIERGENERATOR_H
