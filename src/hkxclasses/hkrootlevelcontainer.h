#ifndef HKROOTLEVELCONTAINER_H
#define HKROOTLEVELCONTAINER_H

#include "src/hkxclasses/hkxobject.h"

class hkRootLevelContainer: public HkxObject
{
    friend class BehaviorGraphView;
    friend class BehaviorFile;
    friend class ProjectFile;
    friend class CharacterFile;
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
    bool merge(hkRootLevelContainer *other);
    void addVariant(const QString & name);
    void setVariantAt(int index, HkxObject *ptr);
    hkRootLevelContainer& operator=(const hkRootLevelContainer&);
    hkRootLevelContainer(const hkRootLevelContainer &);
    struct hkRootLevelContainerNamedVariant
    {
        bool operator==(const hkRootLevelContainerNamedVariant & other){
            if (name == other.name && className == other.className){
                return true;
            }
            return false;
        }
        hkRootLevelContainerNamedVariant(QString varname = "hkbBehaviorGraph", QString classname = "hkbBehaviorGraph")
            : name(varname),
              className(classname)
        {
            //
        }
        QString name;
        QString className;
        HkxSharedPtr variant;  //This can be one of many (any?) types.
    };
private:
    static QString classname;
    static uint refCount;
    QList <hkRootLevelContainerNamedVariant> namedVariants;
};

#endif // HKROOTLEVELCONTAINER_H
