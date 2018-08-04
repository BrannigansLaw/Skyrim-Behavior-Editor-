#ifndef HKBMOVECHARACTERMODIFIER_H
#define HKBMOVECHARACTERMODIFIER_H

#include "hkbmodifier.h"

class hkbMoveCharacterModifier final: public hkbModifier
{
    friend class MoveCharacterModifierUI;
public:
    hkbMoveCharacterModifier(HkxFile *parent, long ref = 0);
    hkbMoveCharacterModifier& operator=(const hkbMoveCharacterModifier&) = delete;
    hkbMoveCharacterModifier(const hkbMoveCharacterModifier &) = delete;
    ~hkbMoveCharacterModifier();
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
    hkQuadVariable offsetPerSecondMS;
    mutable std::mutex mutex;
};

#endif // HKBMOVECHARACTERMODIFIER_H
