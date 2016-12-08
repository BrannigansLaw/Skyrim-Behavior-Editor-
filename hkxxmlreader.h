#ifndef HKXXMLREADER_H
#define HKXXMLREADER_H

#include "utility.h"

class BehaviorFile;

class HkxXmlReader
{
public:
    enum HkxXmlReaderError {NoError = 0, EmptyLine, OrphanedCharacter, InvalidElementName, InvalidAttributeName, OrphanedElementTag};
    HkxXmlReader(BehaviorFile *file): hkxXmlFile(file){
        if (!hkxXmlFile->open(QIODevice::ReadOnly)) return false;
        QByteArray line = hkxXmlFile->readLine(MAX_HKXXML_LINE_LENGHT);
        if (line != "<?xml version=\"1.0\" encoding=\"ascii\"?>\n") return false;
    }
    virtual ~HkxXmlReader(){}
    HkxXmlReaderError readNextLine(QByteArray & elementName, QByteArrayList &attributeNames, QByteArrayList &attributeText, QByteArrayList &elementData);
private:
    struct Element{
        Element(const QByteArray & elem): name(elem), nestCount(0){name.resize(10);}
        QByteArray name;
        ushort nestCount;
    };
    BehaviorFile *hkxXmlFile;
    QList <Element> elementList;
};

#endif // HKXXMLREADER_H
