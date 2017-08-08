#include "hkxfile.h"
#include "src/ui/mainwindow.h"

/**
 * Class: HkxFile
 *
 */

HkxFile::HkxFile(MainWindow *window, const QString & name)
    : QFile(name),
      ui(window),
      changed(false)
{
    parse();
}

void HkxFile::closeFile(){
    if (isOpen()){
        close();
    }
}

bool HkxFile::getIsChanged() const{
    return changed;
}

void HkxFile::toggleChanged(bool wasEdited){
    changed = wasEdited;
}

HkxSharedPtr & HkxFile::getRootObject(){
    return rootObject;
}

bool HkxFile::parse(){
    return true;
}

bool HkxFile::link(){
    return true;
}

void HkxFile::setRootObject(HkxSharedPtr & obj){
    rootObject = obj;
}

QString HkxFile::getRootObjectReferenceString(){
    if (rootObject.data()){
        return rootObject.data()->getReferenceString();
    }
    return "";
}

bool HkxFile::appendAndReadData(int index, HkxObject *obj){
    index++;    //Skip the current line.
    if (!obj->readData(getReader(), index)){
        return false;
    }
    return true;
}

HkxXmlReader & HkxFile::getReader(){
    return reader;
}

HkxXMLWriter & HkxFile::getWriter(){
    return writer;
}

void HkxFile::writeToLog(const QString & message, bool isError){
    if (ui){
        ui->writeToLog(message, isError);
    }
}

HkxFile::~HkxFile(){
    //
}
