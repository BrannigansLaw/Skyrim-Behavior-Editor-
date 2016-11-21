#ifndef HKXXMLWRITER_H
#define HKXXMLWRITER_H

#include "src/utility.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>

class BehaviorFile;

class HkxXMLWriter
{
    friend class BehaviorFile;
public:
    HkxXMLWriter(BehaviorFile *file = NULL);
    virtual ~HkxXMLWriter();
    bool writeToXMLFile();
//private:
    void setBehaviorFile(BehaviorFile *file);
    void writeHeader(const QString & version, const QString & encoding);
    bool writeLine(const QString & tag, const QStringList & attribs, const QStringList & attribValues, const QString & value);
    bool writeLine(const QString & tag, bool opening);
    bool writeLine(const QString & value);
public:
    const QString version;
    const QString encoding;
    const QString classversion;
    const QString contentsversion;
    const QString toplevelobject;
    const QString filetype;
    const QString section;
    const QString object;
    const QString parameter;
    const QString string;
    const QString name;
    const QString clas;
    const QString signature;
    const QString numelements;
private:
    BehaviorFile *hkxXmlFile;
    QFile newfile;
    QTextStream stream;
    ulong nestLevel;
};

#endif // HKXXMLWRITER_H
