#ifndef HKXFILE_H
#define HKXFILE_H

#include "utility.h"
#include "hkxxmlreader.h"
#include "hkobject.h"
//#include "generators.h"
//#include "modifiers.h"

#include <QList>
#include <QFile>

class hkbGenerator;
class hkbModifier;
class hkbOther;
class hkRootLevelContainer;
class MainWindow;

class HkxFile: public QFile
{
    friend class BehaviorGraphView;
public:
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);

    HkxFile(MainWindow *window, const QString & name): QFile(name), ui(window){
        parse();
    }

    virtual ~HkxFile(){}

    void closeFile(){
        if (isOpen()){
            close();
        }
    }

    HkObjectExpSharedPtr & getRootObject(){
        return rootObject;
    }

protected:
    virtual bool parse(){
        return true;
    }
    virtual bool link(){
        return true;
    }
    //virtual void read(){}
    void setRootObject(HkObjectExpSharedPtr & obj){
        rootObject = obj;
    }

    MainWindow * getUI(){
        return ui;
    }

private:
    MainWindow *ui;
    HkObjectExpSharedPtr rootObject;
};

class BehaviorFile: public HkxFile
{
    friend class BehaviorGraphView;
    template <typename T>
    bool appendAndReadData(int ind, T * obj);
public:
    BehaviorFile(MainWindow *window, const QString & name);
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
