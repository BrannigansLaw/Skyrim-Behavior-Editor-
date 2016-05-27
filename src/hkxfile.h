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
    virtual bool link(){return true;}
    //virtual void read(){}
    void setRootObject(const HkObjectExpSharedPtr & obj){rootObject = obj;}
    HkObjectExpSharedPtr & getRootObject(){return rootObject;}
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
    HkObjectExpSharedPtr * findHkObject(long ref);
    HkObjectExpSharedPtr * findGenerator(long ref);
    HkObjectExpSharedPtr * findModifier(long ref);
protected:
    bool parse();
    bool link();
    //void read();
private:
    HkxXmlReader reader;
    QList <HkObjectExpSharedPtr> generators;
    QList <HkObjectExpSharedPtr> modifiers;
    QList <HkObjectExpSharedPtr> otherTypes;
};

#endif // HKXFILE_H
