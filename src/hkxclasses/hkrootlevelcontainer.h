#ifndef HKROOTLEVELCONTAINER_H
#define HKROOTLEVELCONTAINER_H

#include "src/hkxclasses/hkxobject.h"

class hkRootLevelContainer: public HkxObject
{
    friend class BehaviorGraphView;
    friend class BehaviorFile;
public:
    hkRootLevelContainer(HkxFile *parent, long ref = -1);
    virtual ~hkRootLevelContainer();
    bool link();
    bool readData(const HkxXmlReader & reader, long index);
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
private:
    static QString classname;
    static uint refCount;
    struct hkRootLevelContainerNamedVariant
    {
        hkRootLevelContainerNamedVariant(QString varname = "hkbBehaviorGraph", QString classname = "hkbBehaviorGraph"):name(varname), className(classname){}
        QString name;
        QString className;
        HkxSharedPtr variant;  //This can be one of many (any?) types.
    };
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

#endif // HKROOTLEVELCONTAINER_H
