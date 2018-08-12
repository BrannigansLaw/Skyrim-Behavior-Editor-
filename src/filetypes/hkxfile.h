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
public:
    HkxFile& operator=(const HkxFile&) = delete;
    HkxFile(const HkxFile &) = delete;
    virtual ~HkxFile() = default;
    void closeFile();
    virtual bool addObjectToFile(HkxObject *obj, long ref) = 0;
    QString getRootObjectReferenceString() const;
    bool getIsChanged() const;
    QString getFileName() const;
    void setIsChanged(bool wasEdited);
    void setHKXFileName(const QString &name);
    HkxSharedPtr & getRootObject();
protected:
    HkxFile(MainWindow *window, const QString & name);
    MainWindow *getUI() const;
    virtual bool parse();
    virtual bool link();
    void setRootObject(HkxSharedPtr & obj);
    bool appendAndReadData(long & index, HkxObject * obj);
    HkxXmlReader & getReader();
    HkxXMLWriter & getWriter();
private:
    MainWindow *ui;
    HkxSharedPtr rootObject;
    HkxXmlReader reader;
    HkxXMLWriter writer;
    bool changed;
    QString fileNameWithoutPath;
    mutable std::mutex mutex;
};

#endif // HKXFILE_H
