#ifndef HKXXMLREADER_H
#define HKXXMLREADER_H

#include <QByteArray>
#include <QList>
#include <QVector>

#include "src/utility.h"

class HkxFile;

class HkxXmlReader
{
public:
    enum HkxXmlParseLine {
        NoError = 0,
        //NullPointerArgs,
        EmptyLineEndFile = -1,
        OrphanedCharacter = -2,
        InvalidElementName = -3,
        InvalidElementValue = -4,
        //ElementValueSplitOnMutipleLines = -5,
        InvalidAttributeName = -6,
        InvalidAttributeValue = -7,
        OrphanedAttribute = -8,
        MalformedAttribute = -9,
        OrphanedElementTag = -10,
        MalformedComment = -11,
        UnexpectedNewlineCharacter = -12,
        EmptyLine = -13,
        MalformedEndElementTag = -14,
        UnknownError = -50
    };
    HkxXmlReader(HkxFile *file = NULL);
    bool parse();
    virtual ~HkxXmlReader();
    void setFile(HkxFile *file);
    bool atEnd() const;
    int getLastElementIndex() const;
    int getNumElements() const;
    int getNumAttributesAt(int index) const;
    QByteArray getElementNameAt(int index) const;
    QByteArray getElementValueAt(int index) const;
    QByteArray getNthAttributeNameAt(int index, int nth) const;
    QByteArray getNthAttributeValueAt(int index, int nth) const;
    QByteArray findFirstValueWithAttributeValue(const QString & attributevalue) const;
    HkxXmlParseLine readNextLine();
    void clear();
private:
    int skipComment(const QByteArray & line, int index);
    int readValue(const QByteArray & line, int startIndex, bool isSplitOnMultipleLines);
    int readAttribute(const QByteArray & line, int startIndex);
    int readElementTag(const QByteArray & line, int startIndex, bool isIsolatedEndElemTag, bool isEndTag);
private:
    struct Attribute{
        Attribute(const QByteArray & elem): name(elem){}
        QByteArray name;
        QByteArray value;
    };
    struct Element{
        Element(const QByteArray & elem): name(elem), isContainedOnOneLine(true)/*, isClosed(false)*/{}
        QByteArray name;
        QByteArray value;
        QList <Attribute> attributeList;
        bool isContainedOnOneLine;
    };
    HkxFile *hkxXmlFile;
    QList <Element> elementList;
    bool isEOF;
    QVector <long> indexOfElemTags;
    ulong lineNumber;
};

#endif // HKXXMLREADER_H
