#ifndef HKROOTLEVELCONTAINER_H
#define HKROOTLEVELCONTAINER_H

#include "hkxobject.h"

class hkRootLevelContainer: public HkxObject
{
    friend class BehaviorGraphView;
public:
    hkRootLevelContainer(BehaviorFile *parent/*, long ref = 0*/);
    virtual ~hkRootLevelContainer();
    bool link();
    bool readData(const HkxXmlReader & reader, int index);
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
private:
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
private:
    static QString classname;
    static uint refCount;
    struct hkRootLevelContainerNamedVariant
    {
        hkRootLevelContainerNamedVariant(QString varname = "default", QString classname = "default"):name(varname), className(classname){}
        QString name;
        QString className;
        HkxObjectExpSharedPtr variant;  //This can be one of many (any?) types.
    };
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

#endif // HKROOTLEVELCONTAINER_H
