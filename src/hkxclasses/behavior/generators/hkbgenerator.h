#ifndef HKBGENERATOR_H
#define HKBGENERATOR_H

#include "src/hkxclasses/hkxobject.h"

class CustomTreeGraphicsViewIcon;

class hkbGenerator: public DataIconManager
{
    friend class BehaviorGraphView;
    friend class CustomTreeGraphicsViewIcon;
public:
    virtual ~hkbGenerator();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
protected:
    hkbGenerator(HkxFile *parent, long ref = -1);
};

#endif // HKBGENERATOR_H
