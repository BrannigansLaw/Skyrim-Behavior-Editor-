#ifndef BSISACTIVEMODIFIER_H
#define BSISACTIVEMODIFIER_H

#include "hkbmodifier.h"

class BSIsActiveModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSIsActiveModifierUI;
public:
    BSIsActiveModifier(HkxFile *parent, long ref = 0);
    virtual ~BSIsActiveModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSIsActiveModifier& operator=(const BSIsActiveModifier&);
    BSIsActiveModifier(const BSIsActiveModifier &);
private:
    static uint refCount;
    static QString classname;
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
};

#endif // BSISACTIVEMODIFIER_H
