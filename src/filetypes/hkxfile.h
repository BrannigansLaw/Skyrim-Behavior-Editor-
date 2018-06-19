#ifndef HKXFILE_H
#define HKXFILE_H

#include "src/utility.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/hkxclasses/hkxobject.h"

#include <QFile>

class MainWindow;

class HkxFile: public QFile
{
    friend class MainWindow;
    friend class BehaviorGraphView;
    friend class HkxXMLWriter;
    friend class ProjectFile;
public:
    HkxFile(MainWindow *window, const QString & name);
    virtual ~HkxFile();
    void closeFile();
    virtual bool addObjectToFile(HkxObject *obj, long ref) = 0;
    QString getRootObjectReferenceString();
    bool getIsChanged() const;
    void setIsChanged(bool wasEdited);
protected:
    std::unique_lock <std::mutex> lockNGuard() const;
    MainWindow *getUI() const;
    HkxSharedPtr & getRootObject();
    virtual bool parse();
    virtual bool link();
    void setRootObject(HkxSharedPtr & obj);
    bool appendAndReadData(int index, HkxObject * obj);
    HkxXmlReader & getReader();
    HkxXMLWriter & getWriter();
    HkxSharedPtr rootObject;
private:
    MainWindow *ui;
    HkxXmlReader reader;
    HkxXMLWriter writer;
    bool changed;
    mutable std::mutex mutex;
};

#endif // HKXFILE_H
