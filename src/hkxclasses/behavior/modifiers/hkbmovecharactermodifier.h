#ifndef HKBMOVECHARACTERMODIFIER_H
#define HKBMOVECHARACTERMODIFIER_H

#include "hkbmodifier.h"

class hkbMoveCharacterModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class MoveCharacterModifierUI;
public:
    hkbMoveCharacterModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbMoveCharacterModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbMoveCharacterModifier& operator=(const hkbMoveCharacterModifier&);
    hkbMoveCharacterModifier(const hkbMoveCharacterModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable offsetPerSecondMS;
};

#endif // HKBMOVECHARACTERMODIFIER_H
