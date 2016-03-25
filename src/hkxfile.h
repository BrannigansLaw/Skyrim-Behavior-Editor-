#ifndef HKXFILE_H
#define HKXFILE_H

#include "utility.h"
#include "hkxxmlreader.h"
#include "hkobject.h"
#include "generators.h"
#include "modifiers.h"

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
    virtual void read(){}
    void setRootObject(HkObjectExpSharedPtr obj){rootObject = obj;}
private:
    HkObjectExpSharedPtr rootObject;
};

class BehaviorFile: public HkxFile
{
    template <typename T>
    bool appendAndReadData(int ind, T * obj);
public:
    BehaviorFile(const QString & name): HkxFile(name){reader.setBehaviorFile(this);parse();}
    virtual ~BehaviorFile(){}
protected:
    virtual bool parse();
    //void read();
private:
    HkxXmlReader reader;
    QList <hkbGeneratorExpSharedPtr> generators;
    QList <HkbModifierExpSharedPtr> modifiers;
    QList <HkObjectExpSharedPtr> otherTypes;
};

#endif // HKXFILE_H
