#include "hkxfile.h"
#include "src/ui/mainwindow.h"

/**
 * Class: HkxFile
 *
 */

HkxFile::HkxFile(MainWindow *window, const QString & name)
    : QFile(name),
      ui(window)
{
    parse();
}

void HkxFile::closeFile(){
    if (isOpen()){
        close();
    }
}

HkxObjectExpSharedPtr & HkxFile::getRootObject(){
    return rootObject;
}

bool HkxFile::parse(){
    return true;
}

bool HkxFile::link(){
    return true;
}

void HkxFile::setRootObject(HkxObjectExpSharedPtr & obj){
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

MainWindow * HkxFile::getUI(){
    return ui;
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

void HkxFile::setProgressData(const QString & message, int value){
    ui->setProgressData(message, value);
}

HkxFile::~HkxFile(){
    //
}
