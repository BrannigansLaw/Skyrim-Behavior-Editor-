#ifndef HKBMODIFIER_H
#define HKBMODIFIER_H

#include "src/hkxclasses/hkxobject.h"

class CustomTreeGraphicsViewIcon;

class hkbModifier: public DataIconManager
{
    friend class BehaviorGraphView;
    friend class CustomTreeGraphicsViewIcon;
public:
    virtual ~hkbModifier();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
protected:
    hkbModifier(HkxFile *parent, long ref = -1);
};

#endif // HKBMODIFIER_H
