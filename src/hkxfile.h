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
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
    friend class hkbBehaviorGraph;
    template <typename T>
    bool appendAndReadData(int ind, T * obj);
public:
    BehaviorFile(MainWindow *window, const QString & name);
    virtual ~BehaviorFile(){}
    HkObjectExpSharedPtr * findHkObject(long ref);
    HkObjectExpSharedPtr * findGenerator(long ref);
    HkObjectExpSharedPtr * findModifier(long ref);
    int getIndexOfGenerator(const HkObjectExpSharedPtr & obj){
        return generators.indexOf(obj);
    }
    void setGeneratorData(HkObjectExpSharedPtr & ptrToSet, int index){
        if (index >= 0 && index < generators.size()){
            ptrToSet = generators.at(index);
        }
    }

    HkObjectExpSharedPtr* getGeneratorDataAt(int index){
        if (index >= 0 && index < generators.size()){
            return &generators[index];
        }
        return NULL;
    }

    QStringList & getVariableNames() const{
        return static_cast<hkbBehaviorGraphData *>(graphData.data())->getVariableNames();
    }
    QStringList getGeneratorNames();
protected:
    bool parse();
    bool link();
    //void read();
private:
    void removeUnneededGenerators(){
        for (int i = generators.size() - 1; i >= 0; i--){
            if (generators.at(i).constData()->getSignature() == HKB_STATE_MACHINE_STATE_INFO ||
                generators.at(i).constData()->getSignature() == HKB_BLENDER_GENERATOR_CHILD ||
                generators.at(i).constData()->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA ||
                generators.at(i).constData()->getSignature() == HKB_BEHAVIOR_GRAPH
               )
            {
                generators.removeAt(i);
            }
        }
    }

private:
    HkxXmlReader reader;
    HkObjectExpSharedPtr stringData;
    HkObjectExpSharedPtr variableValues;
    HkObjectExpSharedPtr graphData;
    QList <HkObjectExpSharedPtr> generators;
    QList <HkObjectExpSharedPtr> modifiers;
    QList <HkObjectExpSharedPtr> otherTypes;
};

#endif // HKXFILE_H
