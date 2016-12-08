#ifndef HKXFILE_H
#define HKXFILE_H

#include "utility.h"

#include <QList>
#include <QFile>

class hkbGenerator;
class hkbModifier;
class hkbOther;
class hkRootLevelContainer;

class HkxFile: public QFile
{
public:
    HkxFile(const QString & name): QFile(name){parse();}
    virtual ~HkxFile(){if (isOpen()) close();}
protected:
    virtual bool parse(){return true;}
private:
    HkObjectSmartPointer rootObject;
};

class BehaviorFile: public HkxFile
{
public:
    BehaviorFile(const QString & name): HkxFile(name){}
    virtual ~BehaviorFile(){}
protected:
    virtual bool parse();
private:
    QList <HkObjectSmartPointer> generators;
    QList <HkObjectSmartPointer> modifiers;
    QList <HkObjectSmartPointer> otherTypes;
};

#endif // HKXFILE_H
