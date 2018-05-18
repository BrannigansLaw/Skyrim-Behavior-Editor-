#ifndef HKBGETWORLDFROMMODELMODIFIER_H
#define HKBGETWORLDFROMMODELMODIFIER_H

#include "hkbmodifier.h"

class hkbGetWorldFromModelModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class GetWorldFromModelModifierUI;
public:
    hkbGetWorldFromModelModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbGetWorldFromModelModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbGetWorldFromModelModifier& operator=(const hkbGetWorldFromModelModifier&);
    hkbGetWorldFromModelModifier(const hkbGetWorldFromModelModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable translationOut;
    hkQuadVariable rotationOut;
};

#endif // HKBGETWORLDFROMMODELMODIFIER_H
