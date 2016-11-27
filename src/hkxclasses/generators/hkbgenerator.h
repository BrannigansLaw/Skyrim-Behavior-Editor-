#ifndef HKBGENERATOR_H
#define HKBGENERATOR_H

#include "src/hkxclasses/hkxobject.h"

class GeneratorIcon;

class hkbGenerator: public DataIconManager
{
    friend class BehaviorGraphView;
    friend class GeneratorIcon;
public:
    virtual ~hkbGenerator();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
protected:
    hkbGenerator(BehaviorFile *parent, long ref = -1);
};

#endif // HKBGENERATOR_H
