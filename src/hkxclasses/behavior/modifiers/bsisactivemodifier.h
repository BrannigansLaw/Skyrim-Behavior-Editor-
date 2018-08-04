#ifndef BSISACTIVEMODIFIER_H
#define BSISACTIVEMODIFIER_H

#include "hkbmodifier.h"

class BSIsActiveModifier final: public hkbModifier
{
    friend class BSIsActiveModifierUI;
public:
    BSIsActiveModifier(HkxFile *parent, long ref = 0);
    BSIsActiveModifier& operator=(const BSIsActiveModifier&) = delete;
    BSIsActiveModifier(const BSIsActiveModifier &) = delete;
    ~BSIsActiveModifier();
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
    bool bIsActive0;
    bool bInvertActive0;
    bool bIsActive1;
    bool bInvertActive1;
    bool bIsActive2;
    bool bInvertActive2;
    bool bIsActive3;
    bool bInvertActive3;
    bool bIsActive4;
    bool bInvertActive4;
    mutable std::mutex mutex;
};

#endif // BSISACTIVEMODIFIER_H
