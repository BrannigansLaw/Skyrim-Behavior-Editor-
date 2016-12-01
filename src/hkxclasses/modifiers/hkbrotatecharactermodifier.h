#ifndef HKBROTATECHARACTERMODIFIER_H
#define HKBROTATECHARACTERMODIFIER_H

#include "hkbmodifier.h"

class hkbRotateCharacterModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbRotateCharacterModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbRotateCharacterModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbRotateCharacterModifier& operator=(const hkbRotateCharacterModifier&);
    hkbRotateCharacterModifier(const hkbRotateCharacterModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal degreesPerSecond;
    qreal speedMultiplier;
    hkQuadVariable axisOfRotation;
};

#endif // HKBROTATECHARACTERMODIFIER_H
