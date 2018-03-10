#include "projectanimdata.h"
#include "src/utility.h"

#define MIN_NUM_LINES 4

bool ProjectAnimData::chopLine(QFile * file, QByteArray & line, uint & linecount){
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
    uint lineCount = 0;
    uint numFiles = 0;
    uint count = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    uint blocksize = line.toUInt(&ok);
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
        count = line.toUInt(&ok);
        if (!ok || !chopLine(file, line, lineCount)){
            return false;
        }
        numFiles = line.toUInt(&ok);
        if (!ok || numFiles <= 2){
            return false;
        }
        for (uint j = 0; j < numFiles; j++){
            if (!chopLine(file, line, lineCount) || (!line.contains(".hkx") && !line.contains(".HKX"))){
                return false;
            }
            projectFiles.append(line);
        }
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        count = line.toUInt(&ok);
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
            blocksize = line.toUInt(&ok);
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
    uint lineCount = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    uint blocksize = line.toUInt(&ok);
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

bool ProjectAnimData::write(QFile & file, QTextStream & out) const{
    if (out.status() != QTextStream::Ok || !file.isOpen()){
        return false;
    }
    out << QString::number(animationDataLines) << "\n";
    out << "1" << "\n";
    out << QString::number(projectFiles.size()) << "\n";
    for (int i = 0; i < projectFiles.size(); i++){
        out << projectFiles.at(i) << "\n";
    }
    if (animationData.size() > 0){
        out << "1" << "\n";
    }else{
        out << "0" << "\n";
    }
    for (int i = 0; i < animationData.size(); i++){
        if (!animationData.at(i)->write(&file, out)){
            return false;
        }
    }
    out << QString::number(animationMotionDataLines) << "\n";
    for (int i = 0; i < animationMotionData.size(); i++){
        if (!animationMotionData.at(i)->write(&file, out)){
            return false;
        }
    }
    return true;
}

bool ProjectAnimData::appendClipGenerator(SkyrimClipGeneratoData * animData){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == animData->clipGeneratorName){
            return false;
        }
    }
    animationData.append(animData);
    animationDataLines = animationDataLines + animData->lineCount();
    return true;
}

bool ProjectAnimData::removeClipGenerator(const QString &clipname){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname){
            animationDataLines = animationDataLines - animationData.at(i)->lineCount();
            animationData.removeAt(i);
            return true;
        }
    }
    return false;
}

bool ProjectAnimData::appendAnimation(SkyrimAnimationMotionData * motiondata){
    for (int i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == motiondata->animationIndex){
            return false;
        }
    }
    animationMotionData.append(motiondata);
    animationMotionDataLines = animationMotionDataLines + motiondata->lineCount();
    return true;
}

bool ProjectAnimData::removeAnimation(int animationindex){
    for (int i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            animationMotionDataLines = animationMotionDataLines - animationMotionData.at(i)->lineCount();
            animationMotionData.removeAt(i);
            return true;
        }
    }
    return false;
}

SkyrimAnimationMotionData *ProjectAnimData::findMotionData(int animationindex){
    for (int i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i)->animationIndex == animationindex){
            return animationMotionData.at(i);
        }
    }
    return nullptr;
}

void ProjectAnimData::setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname && triggerindex >= 0 && triggerindex < animationData.at(i)->triggers.size()){
            animationData.at(i)->triggers[triggerindex].time = time;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setLocalTimeForClipGenAnimData(): Failed to set data!");
}

void ProjectAnimData::setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, const QString &eventname){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipname && triggerindex >= 0 && triggerindex < animationData.at(i)->triggers.size()){
            animationData.at(i)->triggers[triggerindex].name = eventname;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setEventNameForClipGenAnimData(): Failed to set data!");
}

void ProjectAnimData::setClipNameAnimData(const QString &oldclipname, const QString &newclipname){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == oldclipname){
            animationData.at(i)->clipGeneratorName = newclipname;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setClipNameAnimData(): Failed to set data!");
}

void ProjectAnimData::setAnimationIndexForClipGen(const QString &clipGenName, int index){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->animationIndex = index;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setAnimationIndexForClipGen(): Failed to set data!");
}

void ProjectAnimData::setPlaybackSpeedForClipGen(const QString &clipGenName, qreal speed){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->playbackSpeed = speed;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setPlaybackSpeedForClipGen(): Failed to set data!");
}

void ProjectAnimData::setCropStartAmountLocalTimeForClipGen(const QString &clipGenName, qreal time){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->cropStartTime = time;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setCropStartAmountLocalTimeForClipGen(): Failed to set data!");
}

void ProjectAnimData::setCropEndAmountLocalTimeForClipGen(const QString &clipGenName, qreal time){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->cropEndTime = time;
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::setCropEndAmountLocalTimeForClipGen(): Failed to set data!");
}

void ProjectAnimData::appendClipTriggerToAnimData(const QString &clipGenName, const QString & eventname){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->addTrigger(SkyrimClipTrigger(0, eventname));
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::appendClipTriggerToAnimData(): Failed to set data!");
}

void ProjectAnimData::removeClipTriggerToAnimDataAt(const QString &clipGenName, int index){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i)->clipGeneratorName == clipGenName){
            animationData.at(i)->removeTrigger(index);
            return;
        }
    }
    FATAL_RUNTIME_ERROR("ProjectAnimData::removeClipTriggerToAnimDataAt(): Failed to set data!");
}

bool ProjectAnimData::removeBehaviorFromProject(const QString &behaviorname){
    for (int i = 0; i < projectFiles.size(); i++){
        if (!QString::compare(projectFiles.at(i), behaviorname, Qt::CaseInsensitive)){
            projectFiles.removeAt(i);animationDataLines--;
            return true;
        }
    }
    return false;
}
