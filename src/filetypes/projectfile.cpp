#include "projectfile.h"
#include "behaviorfile.h"
#include "characterfile.h"

#include "src/animData/skyrimanimationmotiondata.h"
#include "src/animData/skyrimanimdata.h"
#include "src/animSetData/skyrimanimsetdata.h"
#include "src/xml/hkxxmlreader.h"
#include "src/xml/hkxxmlwriter.h"
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/behavior/hkbprojectdata.h"
#include "src/hkxclasses/behavior/hkbprojectstringdata.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include <mutex>

ProjectFile::ProjectFile(MainWindow *window, const QString & name)
    : HkxFile(window, name),
      largestRef(0),
      projectIndex(-1),
      skyrimAnimData(new SkyrimAnimData)
{
    projectName = QString(fileName().section("/", -1, -1)).remove(".hkx");
    projectPath = "meshes/actors/"+projectName+"/animations";
    getReader().setFile(this);
}

void ProjectFile::setCharacterFile(CharacterFile *file){
    character = file;
}

bool ProjectFile::isClipGenNameTaken(const QString &name) const{
    for (int i = 0; i < behaviorFiles.size(); i++){
        if (behaviorFiles.at(i)->isClipGenNameTaken(name)){
            return true;
        }
    }
    return false;
}

bool ProjectFile::readAnimationData(const QString & filename){
    QFile *animfile = new QFile(filename);
    QString projectname = fileName().section("/", -1, -1);
    if (!animfile->exists()){
        delete animfile;
        animfile = new QFile(QDir::currentPath()+"/animationdatasinglefile.txt");
        if (!animfile->exists()){
            (qFatal("animationdatasinglefile.txt is missing from the application directory!"));
        }
    }
    if (!skyrimAnimData->parse(animfile, projectname)){
        delete animfile;
        (qFatal("ProjectFile::readAnimationData(): The project animation data file could not be parsed!!!"));
        return false;
    }
    delete animfile;
    projectIndex = skyrimAnimData->getProjectIndex(projectname);
    return true;
}

bool ProjectFile::readAnimationSetData(const QString & filename){
    QFile *animsetfile = new QFile(QString(filename).replace("animationdatasinglefile.txt", "animationsetdatasinglefile.txt"));
    if (!animsetfile->exists()){
        delete animsetfile;
        animsetfile = new QFile(QDir::currentPath()+"/animationsetdatasinglefile.txt");
        if (!animsetfile->exists()){
            (qFatal("animationsetdatasinglefile.txt is missing from the application directory!"));
        }
    }
    if (!skyrimAnimSetData.parse(animsetfile)){
        delete animsetfile;
        (qFatal("ProjectFile::readAnimationSetData(): The project animation set data file could not be parsed!!!"));
        return false;
    }
    delete animsetfile;
    return true;
}

bool ProjectFile::removeClipGenFromAnimData(const QString &name){
    return skyrimAnimData->removeClipGenerator(projectName+".txt", name);
}

bool ProjectFile::removeAnimationFromAnimData(const QString &name){
    return skyrimAnimData->removeAnimation(projectName+".txt", character->getAnimationNames().indexOf(name));    //Unsafe...
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

void ProjectFile::setAnimationIndexDuration(int indexofanimationlist, int animationindex, qreal duration){
    ProjectAnimData *project = skyrimAnimData->getProjectAnimData(projectName);
    if (project){
        if (indexofanimationlist == -1){
            project->animationMotionData.last()->animationIndex = animationindex;
            project->animationMotionData.last()->duration = duration;
        }else if (indexofanimationlist < project->animationMotionData.size()){
            project->animationMotionData.at(indexofanimationlist)->animationIndex = animationindex;
            project->animationMotionData.at(indexofanimationlist)->duration = duration;
        }
    }else{
        (qFatal("ProjectFile::setAnimationIndexDuration(): skyrimAnimData->getProjectAnimData Failed!"));
    }
}

void ProjectFile::generateAnimDataForProject(){
    HkxObject *generator;
    for (int i = 0; i < behaviorFiles.size(); i++){
        for (int j = 0; j < behaviorFiles.at(i)->generators.size(); j++){
            generator = behaviorFiles.at(i)->generators.at(j).data();
            if (generator->getSignature() == HKB_CLIP_GENERATOR){
                if (!skyrimAnimData->appendClipGenerator(projectName, new SkyrimClipGeneratoData(static_cast<hkbClipGenerator *>(generator)->getClipGeneratorAnimData(skyrimAnimData->getProjectAnimData(projectName), getAnimationIndex(static_cast<hkbClipGenerator *>(generator)->animationName))))){
                    (qFatal("ProjectFile::generateAnimDataForProject(): skyrimAnimData->appendClipGenerator Failed!"));
                }
            }
        }
    }
}

int ProjectFile::getAnimationIndex(const QString & name) const{
    if (character){
        return character->getAnimationIndex(name);
    }
    return -1;
}

bool ProjectFile::isAnimationUsed(const QString &animationname){
    HkxObject *generator;
    for (int i = 0; i < behaviorFiles.size(); i++){
        for (int j = 0; j < behaviorFiles.at(i)->generators.size(); j++){
            generator = behaviorFiles.at(i)->generators.at(j).data();
            if (generator->getSignature() == HKB_CLIP_GENERATOR){
                if (!QString::compare(animationname, static_cast<hkbClipGenerator *>(generator)->getAnimationName(), Qt::CaseInsensitive)){
                    writeToLog("ProjectFile: isAnimationUsed()!\nAnimation is used in the Clip Generator \""+static_cast<hkbClipGenerator *>(generator)->getName()+"\" in behavior: "+behaviorFiles.at(i)->fileName().section("/",-1,-1));
                    return true;
                }
            }
        }
    }
    return false;
}

bool ProjectFile::appendClipGeneratorAnimData(const QString &name){
    if (name == ""){
        return false;
    }
    return skyrimAnimData->appendClipGenerator(projectName, new SkyrimClipGeneratoData(skyrimAnimData->getProjectAnimData(projectName), name));
}

void ProjectFile::setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time){
    skyrimAnimData->setLocalTimeForClipGenAnimData(projectName, clipname, triggerindex, time);
}

void ProjectFile::setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, const QString &eventname){
    skyrimAnimData->setEventNameForClipGenAnimData(projectName, clipname, triggerindex, eventname);
}

void ProjectFile::setClipNameAnimData(const QString &oldclipname, const QString &newclipname){
    skyrimAnimData->setClipNameAnimData(projectName, oldclipname, newclipname);
}

void ProjectFile::setAnimationIndexForClipGen(int index, const QString &clipGenName){
    skyrimAnimData->setAnimationIndexForClipGen(projectName, clipGenName, index);
}

void ProjectFile::setPlaybackSpeedAnimData(const QString &clipGenName, qreal speed){
    skyrimAnimData->setPlaybackSpeedAnimData(projectName, clipGenName, speed);
}

void ProjectFile::setCropStartAmountLocalTimeAnimData(const QString &clipGenName, qreal time){
    skyrimAnimData->setCropStartAmountLocalTimeAnimData(projectName, clipGenName, time);
}

void ProjectFile::setCropEndAmountLocalTimeAnimData(const QString &clipGenName, qreal time){
    skyrimAnimData->setCropEndAmountLocalTimeAnimData(projectName, clipGenName, time);
}

void ProjectFile::appendClipTriggerToAnimData(const QString &clipGenName, const QString &eventname){
    skyrimAnimData->appendClipTriggerToAnimData(projectName, clipGenName, eventname);
}

void ProjectFile::removeClipTriggerToAnimDataAt(const QString &clipGenName, int index){
    skyrimAnimData->removeClipTriggerToAnimDataAt(projectName, clipGenName, index);
}

/*SkyrimAnimData *ProjectFile::getAnimData() const{
    return skyrimAnimData;
}*/

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
