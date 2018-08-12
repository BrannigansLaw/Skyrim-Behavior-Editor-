#include "projectanimdata.h"
#include "src/utility.h"

#define MIN_NUM_LINES 3

bool ProjectAnimData::chopLine(QFile * file, QByteArray & line, ulong &linecount){
    if (file){
        if (!file->atEnd()){
            line = file->readLine();
            line.chop(1);
            linecount++;
            return true;
        }
    }
    return false;
}

ProjectAnimData::ProjectAnimData()
    : animationDataLines(MIN_NUM_LINES), animationMotionDataLines(0)
{
    //
}

bool ProjectAnimData::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    ulong lineCount = 0;
    ulong numFiles = 0;
    ulong count = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    ulong blocksize = line.toULong(&ok);
    if (!ok){
        return false;
    }
    animationDataLines = blocksize;
    //Read individual projects...
    while (!file->atEnd()){
        lineCount = 0;
        //Read project filepaths...
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        count = line.toULong(&ok);
        if (!ok || !chopLine(file, line, lineCount)){
            return false;
        }
        numFiles = line.toULong(&ok);
        if (!ok || numFiles <= 2){
            return false;
        }
        for (ulong j = 0; j < numFiles; j++){
            if (!chopLine(file, line, lineCount) || (!line.contains(".hkx") && !line.contains(".HKX"))){
                return false;
            }
            projectFiles.append(line);
        }
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        count = line.toULong(&ok);
        if (!ok){
            return false;
        }
        if (count > 0){
            //Read animation data...
            for (;lineCount < blocksize;){
                animationData.append(new SkyrimClipGeneratoData(this, ""));
                if (!animationData.last()->read(file, lineCount)){
                    return false;
                }
            }
            if (!chopLine(file, line, lineCount)){
                return false;
            }
            blocksize = line.toULong(&ok);
            if (!ok){
                return false;
            }
            animationMotionDataLines = blocksize;
            lineCount = 0;
            //Get animation motion data...
            for (;lineCount < blocksize;){
                animationMotionData.append(new SkyrimAnimationMotionData(this));
                if (!animationMotionData.last()->read(file, lineCount)){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

bool ProjectAnimData::readMotionOnly(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    ulong lineCount = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    ulong blocksize = line.toULong(&ok);
    if (!ok){
        return false;
    }
    animationMotionDataLines = blocksize;
    //Read individual projects...
    while (!file->atEnd()){
        lineCount = 0;
        //Get animation motion data...
        for (;lineCount < blocksize;){
            animationMotionData.append(new SkyrimAnimationMotionData(this));
            if (!animationMotionData.last()->read(file, lineCount)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool ProjectAnimData::write(QFile & file, QTextStream & out){
    if (out.status() != QTextStream::Ok || !file.isOpen()){
        return false;
    }
    fixNumberAnimationLines();
    out << QString::number(animationDataLines) << "\n";
    out << "1" << "\n";
    out << QString::number(projectFiles.size()) << "\n";
    for (auto i = 0; i < projectFiles.size(); i++){
        out << projectFiles.at(i) << "\n";
    }
    if (animationData.size() > 0){
        out << "1" << "\n";
        for (auto i = 0; i < animationData.size(); i++){
            if (!animationData.at(i)->write(&file, out)){
                return false;
            }
        }
        out << QString::number(animationMotionDataLines) << "\n";
        for (auto i = 0; i < animationMotionData.size(); i++){
            if (!animationMotionData.at(i)->write(&file, out)){
                return false;
            }
        }
    }else{
        out << "0" << "\n";
    }
    return true;
}

bool ProjectAnimData::appendClipGenerator(SkyrimClipGeneratoData * animData){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == animData->clipGeneratorName){
            return false;
        }
    }
    animationData.append(animData);
    animationDataLines = animationDataLines + animData->lineCount();
    return true;
}

bool ProjectAnimData::removeClipGenerator(const QString &clipname){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname){
            animationDataLines = animationDataLines - animationData.at(i)->lineCount();
            animationData.removeAt(i);
            return true;
        }
    }
    return false;
}

bool ProjectAnimData::appendAnimation(SkyrimAnimationMotionData * motiondata){
    if (motiondata){
        for (auto i = 0; i < animationMotionData.size(); i++){
            if (animationMotionData.at(i)->animationIndex == motiondata->animationIndex){
                return false;
            }
        }
        animationMotionData.append(motiondata);
        animationMotionDataLines = animationMotionDataLines + motiondata->lineCount();
        return true;
    }
    return false;
}

bool ProjectAnimData::removeAnimation(int animationindex){
    for (auto i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            animationMotionDataLines = animationMotionDataLines - animationMotionData.at(i)->lineCount();
            animationMotionData.removeAt(i);
            return true;
        }
    }
    return false;
}

SkyrimAnimationMotionData *ProjectAnimData::findMotionData(int animationindex){
    for (auto i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            return animationMotionData.at(i);
        }
    }
    animationMotionData.append(new SkyrimAnimationMotionData(this, animationindex));
    return animationMotionData.last();
}

void ProjectAnimData::setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname && triggerindex >= 0 && triggerindex < animationData.at(i)->triggers.size()){
            animationData.at(i)->triggers[triggerindex].time = time;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setLocalTimeForClipGenAnimData(): Failed to set data!");
}

void ProjectAnimData::setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, const QString &eventname){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname && triggerindex >= 0 && triggerindex < animationData.at(i)->triggers.size()){
            animationData.at(i)->triggers[triggerindex].name = eventname;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setEventNameForClipGenAnimData(): Failed to set data!");
}

void ProjectAnimData::setClipNameAnimData(const QString &oldclipname, const QString &newclipname){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == oldclipname){
            animationData.at(i)->clipGeneratorName = newclipname;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setClipNameAnimData(): Failed to set data!");
}

void ProjectAnimData::setAnimationIndexForClipGen(const QString &clipGenName, int index){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->animationIndex = index;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setAnimationIndexForClipGen(): Failed to set data!");
}

void ProjectAnimData::setPlaybackSpeedForClipGen(const QString &clipGenName, qreal speed){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->playbackSpeed = speed;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setPlaybackSpeedForClipGen(): Failed to set data!");
}

void ProjectAnimData::setCropStartAmountLocalTimeForClipGen(const QString &clipGenName, qreal time){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->cropStartTime = time;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setCropStartAmountLocalTimeForClipGen(): Failed to set data!");
}

void ProjectAnimData::setCropEndAmountLocalTimeForClipGen(const QString &clipGenName, qreal time){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->cropEndTime = time;
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::setCropEndAmountLocalTimeForClipGen(): Failed to set data!");
}

void ProjectAnimData::appendClipTriggerToAnimData(const QString &clipGenName, const QString & eventname){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->addTrigger(SkyrimClipTrigger(0, eventname));
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::appendClipTriggerToAnimData(): Failed to set data!");
}

void ProjectAnimData::removeClipTriggerToAnimDataAt(const QString &clipGenName, int index){
    for (auto i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->removeTrigger(index);
            return;
        }
    }
    CRITICAL_ERROR_MESSAGE("ProjectAnimData::removeClipTriggerToAnimDataAt(): Failed to set data!");
}

bool ProjectAnimData::removeBehaviorFromProject(const QString &behaviorname){
    for (auto i = 0; i < projectFiles.size(); i++){
        if (!QString::compare(projectFiles.at(i), behaviorname, Qt::CaseInsensitive)){
            projectFiles.removeAt(i);
            animationDataLines--;
            return true;
        }
    }
    return false;
}

qreal ProjectAnimData::getAnimationDuration(int animationindex) const{
    for (auto i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            return animationMotionData.at(i)->duration;
        }
    }
    return 0;
}

SkyrimAnimationMotionData ProjectAnimData::getAnimationMotionData(int animationindex) const{
    for (auto i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            return *animationMotionData.at(i);
        }
    }
    //WARNING_MESSAGE("ProjectAnimData::getAnimationMotionData(): Motion data was not found!");
    return SkyrimAnimationMotionData(nullptr);
}

void ProjectAnimData::merge(ProjectAnimData *recessiveanimdata){
    bool found;
    if (recessiveanimdata){
        for (auto i = 0; i < recessiveanimdata->projectFiles.size(); i++){
            if (!projectFiles.contains(recessiveanimdata->projectFiles.at(i), Qt::CaseInsensitive)){
                projectFiles.append(recessiveanimdata->projectFiles.at(i));
                animationDataLines++;
            }
        }
        for (auto i = 0; i < recessiveanimdata->animationMotionData.size(); i++){
            found = false;
            for (auto j = 0; j < animationMotionData.size(); j++){
                if (animationMotionData.at(j) && recessiveanimdata->animationMotionData.at(i)){
                    if (*animationMotionData.at(j) == *recessiveanimdata->animationMotionData.at(i)){
                        found = true;
                        break;
                    }
                }else{
                    LogFile::writeToLog("ProjectAnimData::merge(): nullptr found in either dominant or recessive animationMotionData!");
                }
            }
            if (!found){
                animationMotionData.append(recessiveanimdata->animationMotionData.at(i));
                animationMotionDataLines += recessiveanimdata->animationMotionData.at(i)->lineCount();
            }
        }
    }else{
        LogFile::writeToLog("ProjectAnimData::merge(): recessiveanimdata is nullptr!");
    }
}

void ProjectAnimData::fixNumberAnimationLines(){
    auto num = 2 + projectFiles.size() + 1;
    for (auto i = 0; i < animationData.size(); i++){
        num = num + animationData.at(i)->lineCount();
    }
    animationDataLines = num;
}
