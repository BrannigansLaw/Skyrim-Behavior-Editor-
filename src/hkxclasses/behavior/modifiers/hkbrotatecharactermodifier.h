#ifndef HKBROTATECHARACTERMODIFIER_H
#define HKBROTATECHARACTERMODIFIER_H

#include "hkbmodifier.h"

class hkbRotateCharacterModifier final: public hkbModifier
{
    friend class RotateCharacterModifierUI;
public:
    hkbRotateCharacterModifier(HkxFile *parent, long ref = 0);
    hkbRotateCharacterModifier& operator=(const hkbRotateCharacterModifier&) = delete;
    hkbRotateCharacterModifier(const hkbRotateCharacterModifier &) = delete;
    ~hkbRotateCharacterModifier();
public:
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
    qreal degreesPerSecond;
    qreal speedMultiplier;
    hkQuadVariable axisOfRotation;
    mutable std::mutex mutex;
};

#endif // HKBROTATECHARACTERMODIFIER_H
