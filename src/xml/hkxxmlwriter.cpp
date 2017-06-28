#include "hkxxmlwriter.h"
#include "src/filetypes/behaviorfile.h"

#include <QFile>
#include <QTextStream>

HkxXMLWriter::HkxXMLWriter(BehaviorFile *file)
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
      nestLevel(0)
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
    bool isGood = false;
    if (hkxXmlFile){
        newfile.setFileName(hkxXmlFile->fileName()/*.insert(hkxXmlFile->fileName().indexOf('.'), "_out")*/);
        if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text)){
            hkxXmlFile->writeToLog("HkxXMLWriter: writeToXMLFile()!\nUnable to open a new file!!!", true);
            return false;
        }
        stream.setDevice(&newfile);
        writeHeader(version, encoding);
        //writeLine("\n");
        QStringList list1 = {"classversion", "contentsversion", "toplevelobject"};
        QStringList list2 = {classversion, contentsversion, hkxXmlFile->getRootObjectReferenceString()};
        writeLine(filetype, list1, list2, "");
        writeLine("\n");
        writeLine(section, QStringList(name), QStringList("__data__"), "");
        writeLine("\n");
        if (hkxXmlFile->getRootObject().data()->write(this)){
            isGood = true;
        }
        //writeLine("\n");
        writeLine(section, false);
        writeLine("\n");
        writeLine(filetype, false);
        newfile.close();
        return isGood;
    }
    return isGood;
}

void HkxXMLWriter::writeHeader(const QString & version, const QString & encoding){
    stream << "<?xml version=\""+version+"\" encoding=\""+encoding+"\"?>\n";
}

bool HkxXMLWriter::writeLine(const QString & tag, const QStringList & attribs, const QStringList & attribValues, const QString & value, bool nullValueAllowed){
    if (tag == "" || attribs.size() != attribValues.size()){
        hkxXmlFile->writeToLog("HkxXMLWriter: writeLine()!\nXML tag is null or the number of attributes does not match the number of attribute values!!!");
        return false;
    }
    QString text;
    for (uint i = 0; i < nestLevel; i++){
        text.append("\t");
    }
    text = text+"<"+tag;
    stream << text;
    for (int j = 0; j < attribs.size(); j++){
        stream << " "+attribs.at(j)+"=\""+attribValues.at(j)+"\"";
    }
    if (value == ""){
        bool ok;
        if (attribValues.last().toInt(&ok) == 0 && attribValues.size() == 2 && ok){
            stream << "></"+tag+">\n";
        }else{
            if (nullValueAllowed){
                stream << "></"+tag+">\n";
            }else{
                nestLevel++;
                stream << ">\n";
            }
        }
    }else{
        stream << ">"+value+"</"+tag+">\n";
    }
    return true;
}

bool HkxXMLWriter::writeLine(const QString & tag, bool opening){
    if (tag == ""){
        hkxXmlFile->writeToLog("HkxXMLWriter: writeLine()!\nXML tag is null!!!");
        return false;
    }
    QString text;
    if (opening){
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        nestLevel++;
        stream << text+"<"+tag+">\n";
    }else{
        if (nestLevel > 0){
            nestLevel--;
        }
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        stream << text+"</"+tag+">\n";
    }
    return true;
}

bool HkxXMLWriter::writeLine(const QString & value){
    if (value == "\n"){
        stream << "\n";
    }else{
        QString text;
        for (uint i = 0; i < nestLevel; i++){
            text.append("\t");
        }
        QStringList list = value.split('\n', QString::SkipEmptyParts);
        if (list.isEmpty()){
            stream << text+value+"\n";
        }else{
            for (int i = 0; i < list.size(); i++){
                stream << text+list.at(i)+"\n";
            }
        }
    }
    return true;
}
