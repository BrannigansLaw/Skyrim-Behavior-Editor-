#ifndef BSINTERPVALUEMODIFIER_H
#define BSINTERPVALUEMODIFIER_H

#include "hkbmodifier.h"

class BSInterpValueModifier final: public hkbModifier
{
    friend class BSInterpValueModifierUI;
public:
    BSInterpValueModifier(HkxFile *parent, long ref = 0);
    BSInterpValueModifier& operator=(const BSInterpValueModifier&) = delete;
    BSInterpValueModifier(const BSInterpValueModifier &) = delete;
    ~BSInterpValueModifier();
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
    qreal source;
    qreal target;
    qreal result;
    qreal gain;
    mutable std::mutex mutex;
};

#endif // BSINTERPVALUEMODIFIER_H
