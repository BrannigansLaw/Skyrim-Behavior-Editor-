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
public:
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);
    HkxFile(MainWindow *window, const QString & name);
    virtual ~HkxFile();
    void closeFile();
    virtual bool addObjectToFile(HkxObject *obj, long ref) = 0;
    QString getRootObjectReferenceString();
    bool getIsChanged() const;
    void toggleChanged(bool wasEdited);
protected:
    HkxObjectExpSharedPtr & getRootObject();
    virtual bool parse();
    virtual bool link();
    void setRootObject(HkxObjectExpSharedPtr & obj);
    bool appendAndReadData(int index, HkxObject * obj);
    HkxXmlReader & getReader();
    HkxXMLWriter & getWriter();
    HkxObjectExpSharedPtr rootObject;
private:
    MainWindow *ui;
    HkxXmlReader reader;
    HkxXMLWriter writer;
    bool changed;
};

#endif // HKXFILE_H
