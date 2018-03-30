#ifndef HKBGENERATOR_H
#define HKBGENERATOR_H

#include "src/hkxclasses/hkxobject.h"
#include "src/ui/dataiconmanager.h"

class TreeGraphicsItem;

class hkbGenerator: public DataIconManager//, public DataIconManager
{
    friend class BehaviorGraphView;
    friend class TreeGraphicsItem;
public:
    virtual ~hkbGenerator();
    bool link();
    virtual QString getName() const;
   QString getClassname() const;
protected:
    hkbGenerator(HkxFile *parent, long ref = -1);
};

#endif // HKBGENERATOR_H
