#include "projectfile.h"

#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/behavior/hkbprojectdata.h"
#include "src/hkxclasses/behavior/hkbprojectstringdata.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

ProjectFile::ProjectFile(MainWindow *window, const QString & name)
    : HkxFile(window, name),
      largestRef(0)
{
    getReader().setFile(this);
}

void ProjectFile::setCharacterFile(CharacterFile *file){
    character = file;
}

QString ProjectFile::getCharacterFilePathAt(int index) const{
    if (!stringData.data()){
        return "";
    }
    return static_cast<hkbProjectStringData *>(stringData.data())->getCharacterFilePathAt(index);
}

HkxSharedPtr * ProjectFile::findProjectData(long ref){
    if (projectData.getReference() == ref){
        return &projectData;
    }
    return NULL;
}

HkxSharedPtr * ProjectFile::findProjectStringData(long ref){
    if (stringData.getReference() == ref){
        return &stringData;
    }
    return NULL;
}

bool ProjectFile::addObjectToFile(HkxObject *obj, long ref){
    if (ref > largestRef){
        largestRef = ref;
    }else{
        largestRef++;
    }
    obj->setReference(largestRef);
    if (obj->getSignature() == HKB_PROJECT_DATA){
        projectData = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HKB_PROJECT_STRING_DATA){
        stringData = HkxSharedPtr(obj, ref);
    }else if (obj->getSignature() == HK_ROOT_LEVEL_CONTAINER){
        setRootObject(HkxSharedPtr(obj, ref));
    }else{
        writeToLog("ProjectFile: addObjectToFile() failed!\nInvalid type enum for this object!\nObject signature is: "+QString::number(obj->getSignature(), 16), true);
        return false;
    }
    return true;
}

bool ProjectFile::parse(){
    if (!getReader().parse()){
        return false;
    }
    int index = 2;
    bool ok = true;
    HkxSignature signature;
    QByteArray value;
    long ref = 0;
    //setProgressData("Creating HKX objects...", 60);
    while (index < getReader().getNumElements()){
        value = getReader().getNthAttributeNameAt(index, 1);
        if (value == "class"){
            value = getReader().getNthAttributeValueAt(index, 2);
            if (value != ""){
                ref = getReader().getNthAttributeValueAt(index, 0).remove(0, 1).toLong(&ok);
                if (!ok){
                    writeToLog("ProjectFile: parse() failed!\nThe object reference string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                signature = (HkxSignature)value.toULongLong(&ok, 16);
                if (!ok){
                    writeToLog("ProjectFile: parse() failed!\nThe object signature string contained invalid characters and failed to convert to an integer!", true);
                    return false;
                }
                if (signature == HKB_PROJECT_DATA){
                    if (!appendAndReadData(index, new hkbProjectData(this, ref))){
                        return false;
                    }
                }else if (signature == HKB_PROJECT_STRING_DATA){
                    if (!appendAndReadData(index, new hkbProjectStringData(this, ref))){
                        return false;
                    }
                }else if (signature == HK_ROOT_LEVEL_CONTAINER){
                    if (!appendAndReadData(index, new hkRootLevelContainer(this, ref))){
                        return false;
                    }
                }else{
                    writeToLog("ProjectFile: parse()!\nUnknown signature detected!\nUnknown object class name is: "+getReader().getNthAttributeValueAt(index, 1)+"\nUnknown object signature is: "+QString::number(signature, 16));
                    return false;
                }
            }
        }
        index++;
    }
    closeFile();
    getReader().clear();
    //setProgressData("Linking HKX objects...", 80);
    if (!link()){
        writeToLog("ProjectFile: parse() failed because link() failed!", true);
        return false;
    }
    return true;
}

bool ProjectFile::link(){
    if (!getRootObject().constData()){
        writeToLog("ProjectFile: link() failed!\nThe root object of this project file is NULL!", true);
        return false;
    }else if (getRootObject()->getSignature() != HK_ROOT_LEVEL_CONTAINER){
        writeToLog("ProjectFile: link() failed!\nThe root object of this project file is NOT a hkRootLevelContainer!\nThe root object signature is: "+QString::number(getRootObject()->getSignature(), 16), true);
        return false;
    }
    if (!getRootObject().data()->link()){
        writeToLog("ProjectFile: link() failed!\nThe root object of this project file failed to link to it's children!", true);
        return false;
    }
    if (!projectData.data()->link()){
        writeToLog("ProjectFile: link() failed!\nprojectData failed to link to it's children!\n", true);
        return false;
    }
    return true;
}


void ProjectFile::write(){
    ulong ref = 1;
    getRootObject().data()->setReference(ref);
    ref++;
    stringData.data()->setReference(ref);
    ref++;
    projectData.data()->setReference(ref);
    ref++;
    getWriter().setFile(this);
    getWriter().writeToXMLFile();
}

ProjectFile::~ProjectFile(){
    //
}
