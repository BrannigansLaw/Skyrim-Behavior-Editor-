#include "skyrimanimsetdata.h"
#include "src/utility.h"
#include <QTextStream>

SkyrimAnimSetData::SkyrimAnimSetData()
{
    //
}

SkyrimAnimSetData::~SkyrimAnimSetData(){
    for (int i = 0; i < projects.size(); i++){
        if (projects.at(i)){
            delete projects.at(i);
        }
    }
}

bool SkyrimAnimSetData::parse(QFile *file){
    if (!file || (!file->isOpen() && !file->open(QIODevice::ReadOnly | QIODevice::Text))){
        return false;
    }
    QByteArray line;
    bool ok = false;
    uint size = 0;
    //Get project names...
    while (!file->atEnd()){
        line = file->readLine();
        line.chop(1);
        size = line.toUInt(&ok);
        if (ok){
            for (uint i = 0; i < size; i++){
                if (!file->atEnd()){
                    line = file->readLine();
                    line.chop(1);
                    if (line.contains(".txt")){
                        projectNames.append(line);
                    }else{
                        FATAL_RUNTIME_ERROR("SkyrimAnimSetData::parse(): Corrupted project filename does not have 'txt' extension!");
                        return false;
                    }
                }else{
                    FATAL_RUNTIME_ERROR("SkyrimAnimSetData::parse(): Unexpected EOF!");
                    return false;
                }
            }
            for (int i = 0; i < projectNames.size(); i++){
                projects.append(new AnimCacheProjectData());
                if (!projects.last()->read(file)){
                    FATAL_RUNTIME_ERROR("SkyrimAnimSetData::parse(): ProjectAnimSetData read failed!");
                    return false;
                }
            }
        }else{
            FATAL_RUNTIME_ERROR("SkyrimAnimSetData::parse(): Corrupted length of current block!");
            return false;
        }
    }
    return true;
}

bool SkyrimAnimSetData::write(const QString &filename){
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }
    QTextStream out(&file);
    out << QString::number(projectNames.size()) << "\n";
    for (int i = 0; i < projectNames.size(); i++){
        out << projectNames.at(i) << "\n";
    }
    for (int i = 0; i < projects.size(); i++){
        projects.at(i)->write(&file, out);
    }
    return true;
}

bool SkyrimAnimSetData::addNewProject(const QString &projectname){
    for (auto i = 0; i < projectNames.size(); i++){
        if (!QString::compare(projectNames.at(i), projectname, Qt::CaseInsensitive)){
            return false;
        }
    }
    projectNames.append(projectname);
    projects.append(new AnimCacheProjectData());
    return true;
}

bool SkyrimAnimSetData::addAnimationToCache(const QString &projectname, const QString & eventname, const QVector<AnimCacheAnimationInfo> &animations, const QVector<AnimCacheVariable> &vars, const QVector<AnimCacheClipInfo> &clips){
    int count = 0;
    int index = projectNames.indexOf(projectname);
    if (index < 0 || index >= projects.size()){
        return false;
    }
    for (int i = 0; i < projects.at(index)->animSetData.size(); i++){
        if (projects.at(index)->animSetData.at(i)->cacheEvents.contains(eventname)){
            count++;
            if (!projects[index]->animSetData[i]->addAnimationToCache(eventname, animations, vars, clips)){
                return false;
            }
        }
    }
    /*if (count == 0){
        createNewCacheBlock(event, animations, vars, clips);
    }*/
    return true;
}

bool SkyrimAnimSetData::removeAnimationFromCache(const QString & projectname, const QString &animationname, const QString &variablename, const QString &clipname){
    int index = projectNames.indexOf(projectname);
    if (index < 0 || index >= projects.size()){
        return false;
    }
    for (int i = 0; i < projects.at(index)->animSetData.size(); i++){
        projects[index]->animSetData[i]->removeAnimationFromCache(animationname, variablename, clipname);
    }
    return true;
}

AnimCacheProjectData *SkyrimAnimSetData::getProjectCacheData(const QString & name){
    for (int i = 0; i < projectNames.size(); i++){
        if (projectNames.at(i).contains(name, Qt::CaseInsensitive)){
            return projects[i];
        }
    }
    FATAL_RUNTIME_ERROR("SkyrimAnimSetData::getProjectCacheData(): getProjectCacheData() failed!");
    return nullptr;
}

/*bool SkyrimAnimSetData::extractProject(const QString &projectname){
    int index = 0;
    for (; index < projectNames.size(); index++){
        if (projectNames.at(index) == projectname){
            return projects.at(index).write(projectname);
        }
    }
    return false;
}*/
