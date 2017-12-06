#include "skyrimanimdata.h"
#include "src/utility.h"
#include "projectanimdata.h"
#include <QTextStream>

SkyrimAnimData::SkyrimAnimData()
{
    //
}

bool SkyrimAnimData::parse(QFile *file){
    if (!file || (!file->isOpen() && !file->open(QIODevice::ReadOnly | QIODevice::Text))){
        return false;
    }
    QByteArray line;
    bool ok = false;
    uint size = 0;
    //Get project names...
    if (file->fileName().contains("animationdatasinglefile.txt")){
        if (!file->atEnd()){
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
                            (qFatal("SkyrimAnimData::parse(): Corrupted project filename does not have 'txt' extension!"));
                            return false;
                        }
                    }else{
                        (qFatal("SkyrimAnimData::parse(): Unexpected EOF!"));
                        return false;
                    }
                }
            }else{
                (qFatal("SkyrimAnimData::parse(): Corrupted length of current block!"));
                return false;
            }
        }else{
            (qFatal("SkyrimAnimData::parse(): Unexpected EOF!"));
            return false;
        }
    }
    for (uint i = 0; i < size; i++){
        animData.append(new ProjectAnimData());
        if (!animData.last()->read(file)){
            (qFatal("SkyrimAnimData::parse(): ProjectAnimData read failed!"));
            return false;
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
    return NULL;
}

bool SkyrimAnimData::appendClipGenerator(const QString & projectname, SkyrimClipGeneratoData * animdata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::appendClipGenerator(): The project was not found!"));
    }
    return animData.at(index)->appendClipGenerator(animdata);
}

bool SkyrimAnimData::removeClipGenerator(const QString &projectname, const QString &name){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::removeClipGenerator(): The project was not found!"));
    }
    return animData.at(index)->removeClipGenerator(name);
}

bool SkyrimAnimData::appendAnimation(const QString &projectname, SkyrimAnimationMotionData * motiondata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::appendAnimation(): The project was not found!"));
    }
    return animData.at(index)->appendAnimation(motiondata);
}

bool SkyrimAnimData::removeAnimation(const QString &projectname, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::removeAnimation(): The project was not found!"));
    }
    return animData.at(index)->removeAnimation(animationindex);
}

void SkyrimAnimData::setLocalTimeForClipGenAnimData(const QString &projectname, const QString &clipname, int triggerindex, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setLocalTimeForClipGenAnimData(): The project was not found!"));
    }
    animData.at(index)->setLocalTimeForClipGenAnimData(clipname, triggerindex, time);
}

void SkyrimAnimData::setEventNameForClipGenAnimData(const QString &projectname, const QString &clipname, int triggerindex, const QString &eventname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setEventNameForClipGenAnimData(): The project was not found!"));
    }
    animData.at(index)->setEventNameForClipGenAnimData(clipname, triggerindex, eventname);
}

void SkyrimAnimData::setClipNameAnimData(const QString &projectname, const QString &oldclipname, const QString &newclipname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setClipNameAnimData(): The project was not found!"));
    }
    animData.at(index)->setClipNameAnimData(oldclipname, newclipname);
}


void SkyrimAnimData::setAnimationIndexForClipGen(const QString &projectname, const QString &clipGenName, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setAnimationIndexForClipGen(): The project was not found!"));
    }
    animData.at(index)->setAnimationIndexForClipGen(clipGenName, animationindex);
}

void SkyrimAnimData::setPlaybackSpeedAnimData(const QString &projectname, const QString &clipGenName, qreal speed){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setPlaybackSpeedAnimData(): Project was not found!"));
    }
    animData.at(index)->setPlaybackSpeedForClipGen(clipGenName, speed);
}

void SkyrimAnimData::setCropStartAmountLocalTimeAnimData(const QString &projectname, const QString &clipGenName, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setCropStartAmountLocalTimeAnimData(): Project was not found!"));
    }
    animData.at(index)->setCropStartAmountLocalTimeForClipGen(clipGenName, time);
}

void SkyrimAnimData::setCropEndAmountLocalTimeAnimData(const QString &projectname, const QString &clipGenName, qreal time){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::setCropEndAmountLocalTimeAnimData(): Project was not found!"));
    }
    animData.at(index)->setCropEndAmountLocalTimeForClipGen(clipGenName, time);
}

void SkyrimAnimData::appendClipTriggerToAnimData(const QString &projectname, const QString &clipGenName, const QString & eventname){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        (qFatal("SkyrimAnimData::appendClipTriggerToAnimData(): Project was not found!"));
    }
    animData.at(index)->appendClipTriggerToAnimData(clipGenName, eventname);
}

void SkyrimAnimData::removeClipTriggerToAnimDataAt(const QString &projectname, const QString &clipGenName, int index){
    int indexi = getProjectIndex(projectname);
    if (indexi < 0 || indexi >= animData.size()){
        (qFatal("SkyrimAnimData::removeClipTriggerToAnimDataAt(): Project was not found!"));
    }
    animData.at(indexi)->removeClipTriggerToAnimDataAt(clipGenName, index);
}

