#ifndef HKBMODIFIER_H
#define HKBMODIFIER_H

#include "src/hkxclasses/hkxobject.h"
#include "src/ui/dataiconmanager.h"

class TreeGraphicsItem;

class hkbModifier: public DataIconManager//, public DataIconManager
{
    friend class BehaviorGraphView;
    friend class TreeGraphicsItem;
public:
    virtual ~hkbModifier();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
protected:
    hkbModifier(HkxFile *parent, long ref = -1);
};

#endif // HKBMODIFIER_H
