#ifndef HKBMIRRORMODIFIER_H
#define HKBMIRRORMODIFIER_H

#include "hkbmodifier.h"

class hkbMirrorModifier final: public hkbModifier
{
    friend class MirrorModifierUI;
public:
    hkbMirrorModifier(HkxFile *parent, long ref = 0);
    hkbMirrorModifier& operator=(const hkbMirrorModifier&) = delete;
    hkbMirrorModifier(const hkbMirrorModifier &) = delete;
    ~hkbMirrorModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    bool isAdditive;
    mutable std::mutex mutex;
};

#endif // HKBMIRRORMODIFIER_H
