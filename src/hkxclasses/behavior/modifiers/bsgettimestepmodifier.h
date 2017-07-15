#ifndef BSGETTIMESTEPMODIFIER_H
#define BSGETTIMESTEPMODIFIER_H

#include "hkbmodifier.h"

class BSGetTimeStepModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSGetTimeStepModifierUI;
public:
    BSGetTimeStepModifier(HkxFile *parent, long ref = 0);
    virtual ~BSGetTimeStepModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSGetTimeStepModifier& operator=(const BSGetTimeStepModifier&);
    BSGetTimeStepModifier(const BSGetTimeStepModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal timeStep;
};

#endif // BSGETTIMESTEPMODIFIER_H
