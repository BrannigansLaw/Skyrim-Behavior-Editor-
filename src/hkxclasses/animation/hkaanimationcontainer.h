#ifndef HKAANIMATIONCONTAINER_H
#define HKAANIMATIONCONTAINER_H

#include "src/hkxclasses/hkxobject.h"

class hkaAnimationContainer: public HkxObject
{
public:
    hkaAnimationContainer(HkxFile *parent, long ref = 0);
    virtual ~hkaAnimationContainer();
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkaAnimationContainer& operator=(const hkaAnimationContainer&);
    hkaAnimationContainer(const hkaAnimationContainer &);
private:
    static uint refCount;
    static QString classname;
    QVector <HkxSharedPtr> skeletons;
    //animations, bindings, attachments, skins, etc...
};

#endif // HKAANIMATIONCONTAINER_H
