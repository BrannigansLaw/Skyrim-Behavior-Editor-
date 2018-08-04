#include "hkxxmlwriter.h"
#include "src/filetypes/behaviorfile.h"

#include <QFile>
#include <QTextStream>

HkxXMLWriter::HkxXMLWriter(HkxFile *file)
    : hkxXmlFile(file),
      version("1.0"),
      encoding("ascii"),
      classversion("8"),
      contentsversion("hk_2010.2.0-r1"),
      toplevelobject("#0001"),
      filetype("hkpackfile"),
      section("hksection"),
      object("hkobject"),
      parameter("hkparam"),
      string("hkcstring"),
      name("name"),
      clas("class"),
      signature("signature"),
      numelements("numelements"),
      nestLevel(0),
      newfile(nullptr),
      stream(nullptr)
{
    //
}

HkxXMLWriter::~HkxXMLWriter(){
    //
}

void HkxXMLWriter::setFile(HkxFile *file){
    hkxXmlFile = file;
}

bool HkxXMLWriter::writeToXMLFile(){
    bool result = true;
    if (hkxXmlFile){
        nestLevel = 0;
        newfile = new QFile(hkxXmlFile->fileName());
        stream = new QTextStream(newfile);
        if (!newfile->open(QIODevice::WriteOnly | QIODevice::Text)){
            LogFile::writeToLog("HkxXMLWriter: writeToXMLFile()!\nUnable to open a new file!!!");
            result = false;
        }else{
            writeHeader(version, encoding);
            QStringList list1 = {"classversion", "contentsversion", "toplevelobject"};
            QStringList list2 = {classversion, contentsversion, hkxXmlFile->getRootObjectReferenceString()};
            writeLine(filetype, list1, list2, "");
            writeLine("\n");
            writeLine(section, QStringList(name), QStringList("__data__"), "");
            writeLine("\n");
            if (!hkxXmlFile->getRootObject()->write(this)){
                result = false;
            }
            writeLine(section, false);
            writeLine("\n");
            writeLine(filetype, false);
        }
    }else{
        //LogFile::writeToLog("HkxXMLWriter: writeToXMLFile()!\nUnable to open a new file!!!");
        result = false;
    }
    newfile->close();
    delete newfile;
    newfile = nullptr;
    delete stream;
    stream = nullptr;
    return result;
}

void HkxXMLWriter::writeHeader(const QString & version, const QString & encoding){
    *stream << "<?xml version=\""+version+"\" encoding=\""+encoding+"\"?>\n";
}

bool HkxXMLWriter::writeLine(const QString & tag, const QStringList & attribs, const QStringList & attribValues, const QString & value, bool nullValueAllowed){
    if (tag == "" || attribs.size() != attribValues.size()){
        LogFile::writeToLog("HkxXMLWriter: writeLine()!\nXML tag is null or the number of attributes does not match the number of attribute values!!!");
        return false;
    }
    QString text;
    for (uint i = 0; i < nestLevel; i++){
        text.append("\t");
    }
    text = text+"<"+tag;
    *stream << text;
    for (auto j = 0; j < attribs.size(); j++){
        *stream << " "+attribs.at(j)+"=\""+attribValues.at(j)+"\"";
    }
    if (value == ""){
        bool ok;
        if (attribValues.last().toInt(&ok) == 0 && attribValues.size() == 2 && ok){
            *stream << "></"+tag+">\n";
        }else{
            if (nullValueAllowed){
                *stream << "></"+tag+">\n";
            }else{
                nestLevel++;
                *stream << ">\n";
            }
        }
    }else{
        *stream << ">"+value+"</"+tag+">\n";
    }
    return true;
}

bool HkxXMLWriter::writeLine(const QString & tag, bool opening){
    if (tag == ""){
        LogFile::writeToLog("HkxXMLWriter: writeLine()!\nXML tag is null!!!");
        return false;
    }
    QString text;
    if (opening){
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        nestLevel++;
        *stream << text+"<"+tag+">\n";
    }else{
        if (nestLevel > 0){
            nestLevel--;
        }
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        *stream << text+"</"+tag+">\n";
    }
    return true;
}

bool HkxXMLWriter::writeLine(const QString & value){
    if (value == "\n"){
        *stream << "\n";
    }else{
        QString text;
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        QStringList list = value.split('\n', QString::SkipEmptyParts);
        if (list.isEmpty()){
            *stream << text+value+"\n";
        }else{
            for (auto i = 0; i < list.size(); i++){
                *stream << text+list.at(i)+"\n";
            }
        }
    }
    return true;
}
