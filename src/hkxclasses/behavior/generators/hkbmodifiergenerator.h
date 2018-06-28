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
    hkbModifierGenerator(HkxFile *parent, long ref = 0);
    virtual ~hkbModifierGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    int getIndexToInsertIcon(HkxObject *child) const;
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbModifierGenerator& operator=(const hkbModifierGenerator&);
    hkbModifierGenerator(const hkbModifierGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    HkxSharedPtr modifier;
    HkxSharedPtr generator;
};


#endif // HKBMODIFIERGENERATOR_H
