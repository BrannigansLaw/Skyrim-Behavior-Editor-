#ifndef HKBMODIFIERGENERATOR_H
#define HKBMODIFIERGENERATOR_H

#include "hkbgenerator.h"

//Do not allow variables to be bound to this class in the editor...
class hkbModifierGenerator: public hkbGenerator
{
    friend class BehaviorGraphView;
    friend class ModifierGeneratorUI;
    friend class hkbStateMachine;
public:
    hkbModifierGenerator(BehaviorFile *parent/*, qint16 ref = 0*/);
    virtual ~hkbModifierGenerator();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
private:
    hkbModifierGenerator& operator=(const hkbModifierGenerator&);
    hkbModifierGenerator(const hkbModifierGenerator &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    HkxObjectExpSharedPtr modifier;
    HkxObjectExpSharedPtr generator;
};


#endif // HKBMODIFIERGENERATOR_H
