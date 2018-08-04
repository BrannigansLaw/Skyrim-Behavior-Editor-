#ifndef HKXXMLWRITER_H
#define HKXXMLWRITER_H

#include "src/utility.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>

class HkxFile;

class HkxXMLWriter
{
    friend class BehaviorFile;
public:
    HkxXMLWriter(HkxFile *file = nullptr);
    virtual ~HkxXMLWriter();
    bool writeToXMLFile();
//private:
    void setFile(HkxFile *file);
    void writeHeader(const QString & version, const QString & encoding);
    bool writeLine(const QString & tag, const QStringList & attribs, const QStringList & attribValues, const QString & value, bool nullValueAllowed = false);
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
    HkxFile *hkxXmlFile;
    QFile *newfile;
    QTextStream *stream;
    ulong nestLevel;
};

#endif // HKXXMLWRITER_H
