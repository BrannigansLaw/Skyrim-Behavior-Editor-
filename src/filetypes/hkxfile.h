#ifndef HKXFILE_H
#define HKXFILE_H

#include "src/utility.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/hkxclasses/hkxobject.h"

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
    /*enum HkxFileType {
        HKX_BEHAVIOR=0,
        HKX_PROJECT=1,
        HKX_CHARACTER=2,
        HKX_SKELETON=3
    };*/
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);
    HkxFile(MainWindow *window, const QString & name);
    virtual ~HkxFile();
    void closeFile();
    HkxObjectExpSharedPtr & getRootObject();
    virtual bool addObjectToFile(HkxObject *obj, long ref) = 0;
    QString getRootObjectReferenceString();
    //HkxFileType getFiletype() const;
protected:
    virtual bool parse();
    virtual bool link();
    void setRootObject(HkxObjectExpSharedPtr & obj);
    bool appendAndReadData(int index, HkxObject * obj);
    MainWindow * getUI();
    HkxXmlReader & getReader();
    HkxXMLWriter & getWriter();
    HkxObjectExpSharedPtr rootObject;
    //void setFileType(HkxFileType type);
private:
    MainWindow *ui;
    //HkxObjectExpSharedPtr rootObject;
    HkxXmlReader reader;
    HkxXMLWriter writer;
    //HkxFileType filetype;
};

#endif // HKXFILE_H
