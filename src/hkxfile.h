#ifndef HKXFILE_H
#define HKXFILE_H

#include "utility.h"
#include "hkxxmlreader.h"
#include "hkobject.h"
#include "generators.h"

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
    virtual ~HkxFile(){}
    void closeFile(){if (isOpen()) close();}
protected:
    virtual bool parse(){return true;}
    void setRootObject(HkObjectExpSharedPtr obj){rootObject = obj;}
private:
    HkObjectExpSharedPtr rootObject;
};

class BehaviorFile: public HkxFile
{
public:
    BehaviorFile(const QString & name): HkxFile(name){reader.setBehaviorFile(this);parse();}
    virtual ~BehaviorFile(){}
protected:
    virtual bool parse();
private:
    HkxXmlReader reader;
    QList <hkbGeneratorExpSharedPtr> generators;
    QList <HkObjectExpSharedPtr> modifiers;
    QList <HkObjectExpSharedPtr> otherTypes;
};

#endif // HKXFILE_H
