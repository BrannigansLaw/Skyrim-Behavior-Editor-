#ifndef BSTWEENERMODIFIER_H
#define BSTWEENERMODIFIER_H

#include "hkbmodifier.h"

class BSTweenerModifier final: public hkbModifier
{
    friend class BSTweenerModifierUI;
public:
    BSTweenerModifier(HkxFile *parent, long ref = 0);
    BSTweenerModifier& operator=(const BSTweenerModifier&) = delete;
    BSTweenerModifier(const BSTweenerModifier &) = delete;
    ~BSTweenerModifier();
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
    bool tweenPosition;
    bool tweenRotation;
    bool useTweenDuration;
    qreal tweenDuration;
    hkQuadVariable targetPosition;
    hkQuadVariable targetRotation;
    mutable std::mutex mutex;
};

#endif // BSTWEENERMODIFIER_H
