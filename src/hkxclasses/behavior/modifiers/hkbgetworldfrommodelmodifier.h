#ifndef HKBGETWORLDFROMMODELMODIFIER_H
#define HKBGETWORLDFROMMODELMODIFIER_H

#include "hkbmodifier.h"

class hkbGetWorldFromModelModifier final: public hkbModifier
{
    friend class GetWorldFromModelModifierUI;
public:
    hkbGetWorldFromModelModifier(HkxFile *parent, long ref = 0);
    hkbGetWorldFromModelModifier& operator=(const hkbGetWorldFromModelModifier&) = delete;
    hkbGetWorldFromModelModifier(const hkbGetWorldFromModelModifier &) = delete;
    ~hkbGetWorldFromModelModifier();
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
    hkQuadVariable translationOut;
    hkQuadVariable rotationOut;
    mutable std::mutex mutex;
};

#endif // HKBGETWORLDFROMMODELMODIFIER_H
