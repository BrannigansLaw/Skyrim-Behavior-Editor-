#ifndef BSTWEENERMODIFIER_H
#define BSTWEENERMODIFIER_H

#include "hkbmodifier.h"

class BSTweenerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSTweenerModifier(BehaviorFile *parent, long ref = 0);
    virtual ~BSTweenerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSTweenerModifier& operator=(const BSTweenerModifier&);
    BSTweenerModifier(const BSTweenerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    bool tweenPosition;
    bool tweenRotation;
    bool useTweenDuration;
    qreal tweenDuration;
    hkQuadVariable targetPosition;
    hkQuadVariable targetRotation;
};

#endif // BSTWEENERMODIFIER_H
