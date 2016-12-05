#ifndef HKBMIRRORMODIFIER_H
#define HKBMIRRORMODIFIER_H

#include "hkbmodifier.h"

class hkbMirrorModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbMirrorModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbMirrorModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbMirrorModifier& operator=(const hkbMirrorModifier&);
    hkbMirrorModifier(const hkbMirrorModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    bool isAdditive;
};

#endif // HKBMIRRORMODIFIER_H
