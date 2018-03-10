#include "skyrimanimdata.h"
#include "src/utility.h"
#include "projectanimdata.h"
#include <QTextStream>

SkyrimAnimData::SkyrimAnimData()
{
    //
}

bool SkyrimAnimData::parse(QFile *file, const QString & projectToIgnore){
    if (!file || (!file->isOpen() && !file->open(QIODevice::ReadOnly | QIODevice::Text))){
        return false;
    }
    QByteArray line;
    bool ok = false;
    int size = 0;
    int indexToIgnore;
    int blocksize;
    QString name = projectToIgnore;
    //Get project names...
    if (file->fileName().contains("animationdatasinglefile.txt")){
        if (!file->atEnd()){
            line = file->readLine();
            line.chop(1);
            size = line.toUInt(&ok);
            if (ok){
                for (int i = 0; i < size; i++){
                    if (!file->atEnd()){
                        line = file->readLine();
                        line.chop(1);
                        if (line.contains(".txt")){
                            projectNames.append(line);
                        }else{
                            FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): Corrupted project filename does not have 'txt' extension!");
                            return false;
                        }
                    }else{
                        FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): Unexpected EOF!");
                        return false;
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): Corrupted length of current block!");
                return false;
            }
        }else{
            FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): Unexpected EOF!");
            return false;
        }
    }
    if (projectToIgnore == ""){
        indexToIgnore = projectNames.size();
    }else{
        if (name.contains(".hkx")){
            name.replace(".hkx", ".txt");
        }
        if (!name.contains(".txt")){
            name.append(".txt");
        }
        for (int i = 0; i < projectNames.size(); i++){
            if (!QString::compare(projectNames.at(i), name, Qt::CaseInsensitive)){
                indexToIgnore = i;
                break;
            }
        }
        if (indexToIgnore == -1){
            indexToIgnore = projectNames.size();
        }
    }
    for (int i = 0; i < size && i < indexToIgnore; i++){
        animData.append(new ProjectAnimData());
        if (!animData.last()->read(file)){
            FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): ProjectAnimData read failed!");
            return false;
        }
    }
    if (indexToIgnore != projectNames.size()){
        indexToIgnore++;
        line = file->readLine();
        line.chop(1);
        blocksize = line.toUInt(&ok);
        if (!ok || file->atEnd()){
            return false;
        }
        animData.append(new ProjectAnimData());
        line = file->readLine();
        line.chop(1);
        int numFiles = line.toUInt(&ok);
        if (!ok || numFiles != 1 || file->atEnd()){
            return false;
        }
        line = file->readLine();
        line.chop(1);
        numFiles = line.toUInt(&ok);
        if (!ok || numFiles <= 2 || file->atEnd()){
            return false;
        }
        for (int i = 0; i < numFiles && !file->atEnd(); i++){
            line = file->readLine();
            if ((!line.contains(".hkx") && !line.contains(".HKX"))){
                return false;
            }
            line.chop(1);
            animData.last()->projectFiles.append(line);
        }
        animData.last()->animationDataLines = numFiles + 2;
        for (int i = numFiles + 2; i < blocksize && !file->atEnd(); i++){
            file->readLine();
        }
        if (!animData.last()->readMotionOnly(file)){
            FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): readMotionOnly read failed!");
            return false;
        }
        for (int i = indexToIgnore; i < size; i++){
            animData.append(new ProjectAnimData());
            if (!animData.last()->read(file)){
                FATAL_RUNTIME_ERROR("SkyrimAnimData::parse(): ProjectAnimData read failed!");
                return false;
            }
        }
    }
    return true;
}

/*bool SkyrimAnimData::extract(const QString & projectname){
    int index = 0;
    for (; index < projectNames.size(); index++){
        if (!QString::compare(projectNames.at(index), projectname.section("/", -1, -1), Qt::CaseInsensitive)){
            return animData.at(index).write(projectname);
        }
    }
    return false;
}*/

bool SkyrimAnimData::write(const QString &filename){
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }
    QTextStream out(&file);
    out << QString::number(projectNames.size()) << "\n";
    for (int i = 0; i < projectNames.size(); i++){
        out << projectNames.at(i) << "\n";
    }
    for (int i = 0; i < animData.size(); i++){
        if (!animData.at(i)->write(file, out)){
            return false;
        }
    }
    return true;
}

int SkyrimAnimData::getProjectIndex(const QString &projectname) const{
    QString name = projectname;
    if (name.contains(".hkx")){
        name.replace(".hkx", ".txt");
    }
    if (!name.contains(".txt")){
        name.append(".txt");
    }
    for (int i = 0; i < projectNames.size(); i++){
        if (name.compare(projectNames.at(i), Qt::CaseInsensitive) == 0){
            return i;
        }
    }
    return -1;
}

ProjectAnimData *SkyrimAnimData::getProjectAnimData(const QString &projectname) const{
    int index = getProjectIndex(projectname);
    if (index > -1 && index < animData.size()){
        return animData.at(index);
    }
    return nullptr;
}

int SkyrimAnimData::addNewProject(const QString &projectname, const QStringList &projectfilerelativepaths){
    for (auto i = 0; i < projectNames.size(); i++){
        if (!QString::compare(projectNames.at(i), projectname, Qt::CaseInsensitive)){
            return -1;
        }
    }
    projectNames.append(projectname);
    animData.append(new ProjectAnimData());
    for (auto i = 0; i < projectfilerelativepaths.size(); i++){
        animData.last()->projectFiles.append(projectfilerelativepaths.at(i));
        animData.last()->animationDataLines++;
    }
    return animData.size() - 1;
}

bool SkyrimAnimData::appendClipGenerator(const QString & projectname, SkyrimClipGeneratoData * animdata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::appendClipGenerator(): The project was not found!");
    }
    return animData.at(index)->appendClipGenerator(animdata);
}

bool SkyrimAnimData::removeClipGenerator(const QString &projectname, const QString &name){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::removeClipGenerator(): The project was not found!");
    }
    return animData.at(index)->removeClipGenerator(name);
}

bool SkyrimAnimData::appendAnimation(const QString &projectname, SkyrimAnimationMotionData * motiondata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::appendAnimation(): The project was not found!");
    }
    return animData.at(index)->appendAnimation(motiondata);
}

bool SkyrimAnimData::removeAnimation(const QString &projectname, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::removeAnimation(): The project was not found!");
    }
    return animData.at(index)->removeAnimation(animationindex);
}

void SkyrimAnimData::setLocalTimeForClipGenAnimData(const QString &projectname, const QString &clipname, int triggerindex, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setLocalTimeForClipGenAnimData(): The project was not found!");
    }
    animData.at(index)->setLocalTimeForClipGenAnimData(clipname, triggerindex, time);
}

void SkyrimAnimData::setEventNameForClipGenAnimData(const QString &projectname, const QString &clipname, int triggerindex, const QString &eventname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setEventNameForClipGenAnimData(): The project was not found!");
    }
    animData.at(index)->setEventNameForClipGenAnimData(clipname, triggerindex, eventname);
}

void SkyrimAnimData::setClipNameAnimData(const QString &projectname, const QString &oldclipname, const QString &newclipname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setClipNameAnimData(): The project was not found!");
    }
    animData.at(index)->setClipNameAnimData(oldclipname, newclipname);
}


void SkyrimAnimData::setAnimationIndexForClipGen(const QString &projectname, const QString &clipGenName, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setAnimationIndexForClipGen(): The project was not found!");
    }
    animData.at(index)->setAnimationIndexForClipGen(clipGenName, animationindex);
}

void SkyrimAnimData::setPlaybackSpeedAnimData(const QString &projectname, const QString &clipGenName, qreal speed){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setPlaybackSpeedAnimData(): Project was not found!");
    }
    animData.at(index)->setPlaybackSpeedForClipGen(clipGenName, speed);
}

void SkyrimAnimData::setCropStartAmountLocalTimeAnimData(const QString &projectname, const QString &clipGenName, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setCropStartAmountLocalTimeAnimData(): Project was not found!");
    }
    animData.at(index)->setCropStartAmountLocalTimeForClipGen(clipGenName, time);
}

void SkyrimAnimData::setCropEndAmountLocalTimeAnimData(const QString &projectname, const QString &clipGenName, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::setCropEndAmountLocalTimeAnimData(): Project was not found!");
    }
    animData.at(index)->setCropEndAmountLocalTimeForClipGen(clipGenName, time);
}

void SkyrimAnimData::appendClipTriggerToAnimData(const QString &projectname, const QString &clipGenName, const QString & eventname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::appendClipTriggerToAnimData(): Project was not found!");
    }
    animData.at(index)->appendClipTriggerToAnimData(clipGenName, eventname);
}

void SkyrimAnimData::removeClipTriggerToAnimDataAt(const QString &projectname, const QString &clipGenName, int index){
    int indexi = getProjectIndex(projectname);
    if (indexi < 0 || indexi >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::removeClipTriggerToAnimDataAt(): Project was not found!");
    }
    animData.at(indexi)->removeClipTriggerToAnimDataAt(clipGenName, index);
}

bool SkyrimAnimData::isProjectNameTaken(const QString & name) const{
    for (auto i = 0; i < projectNames.size(); i++){
        if (projectNames.at(i) == name){
            return true;
        }
    }
    return false;
}

bool SkyrimAnimData::removeBehaviorFromProject(const QString &projectname, const QString &behaviorfilename){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        FATAL_RUNTIME_ERROR("SkyrimAnimData::removeBehaviorFromProject(): Project was not found!");
    }
    animData.at(index)->removeBehaviorFromProject(behaviorfilename);
}

