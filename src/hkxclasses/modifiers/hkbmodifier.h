#ifndef HKBMODIFIER_H
#define HKBMODIFIER_H

#include "src/hkxclasses/hkxobject.h"

class GeneratorIcon;

class hkbModifier: public DataIconManager
{
    friend class BehaviorGraphView;
    friend class GeneratorIcon;
public:
    virtual ~hkbModifier();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
protected:
    hkbModifier(BehaviorFile *parent, long ref = -1);
};

#endif // HKBMODIFIER_H
