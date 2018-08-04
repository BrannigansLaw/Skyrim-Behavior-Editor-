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
    QString getFileName() const;
    void setHKXFileName(const QString &name);
protected:
    MainWindow *getUI() const;
    HkxSharedPtr & getRootObject();
    virtual bool parse();
    virtual bool link();
    void setRootObject(HkxSharedPtr & obj);
    bool appendAndReadData(long & index, HkxObject * obj);
    HkxXmlReader & getReader();
    HkxXMLWriter & getWriter();
    HkxSharedPtr rootObject;
private:
    MainWindow *ui;
    HkxXmlReader reader;
    HkxXMLWriter writer;
    bool changed;
    QString fileNameWithoutPath;
    mutable std::mutex mutex;
};

#endif // HKXFILE_H
