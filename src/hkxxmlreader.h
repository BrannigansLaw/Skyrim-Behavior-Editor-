#ifndef HKXXMLREADER_H
#define HKXXMLREADER_H

#include "utility.h"

class BehaviorFile;

class HkxXmlReader
{
public:
    enum HkxXmlParseLine {
        NoError = 0,
        //NullPointerArgs,
        EmptyLineEndFile = 1,
        OrphanedCharacter = 2,
        InvalidElementName = 3,
        InvalidElementValue = 4,
        //ElementValueSplitOnMutipleLines = 5,
        InvalidAttributeName = 6,
        InvalidAttributeValue = 7,
        DuplicateAttribute = 8,
        OrphanedAttribute = 9,
        OrphanedElementTag = 10,
        MalformedComment = 11,
        UnexpectedNewlineCharacter = 12,
        EmptyLine = 13,
        MalformedEndElementTag = 14,
        UnknownError = 50
    };
    HkxXmlReader(BehaviorFile *file = NULL);
    virtual ~HkxXmlReader(){/*if (hkxXmlFile){hkxXmlFile->closeFile();}*/}
    void setBehaviorFile(BehaviorFile *file){hkxXmlFile = file;}
    bool parse();
    bool atEnd() const{return isEOF;}
    int getLastElementIndex()const{return elementList.size() - 1;}
    int getNumElements()const{return elementList.size();}
    int getNumAttributesAt(int index) const;
    QByteArray getElementNameAt(int index) const;
    QByteArray getElementValueAt(int index) const;
    QByteArray getNthAttributeNameAt(int index, int nth) const;
    QByteArray getNthAttributeValueAt(int index, int nth) const;
    HkxXmlParseLine readNextLine();
private:
    struct Attribute{
        Attribute(const QByteArray & elem): name(elem){name.resize(15);}
        QByteArray name;
        QByteArray value;
    };
    struct Element{
        Element(const QByteArray & elem): name(elem), isContainedOnOneLine(true){name.append(QByteArray(10, '\0'));value.append(QByteArray(9, '\0'));}
        QByteArray name;
        QByteArray value;
        QList <Attribute> attributeList;
        bool isContainedOnOneLine;
    };
    BehaviorFile *hkxXmlFile;
    QList <Element> elementList;
    bool isEOF;
};

#endif // HKXXMLREADER_H
