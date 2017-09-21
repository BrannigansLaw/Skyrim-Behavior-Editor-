#include "skyrimanimsetdata.h"
#include "src/utility.h"
#include <QTextStream>

SkyrimAnimSetData::SkyrimAnimSetData()
{
    //
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
                        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimSetData::parse(): Corrupted project filename does not have 'txt' extension!"));
                        return false;
                    }
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimSetData::parse(): Unexpected EOF!"));
                    return false;
                }
            }
            for (int i = 0; i < projectNames.size(); i++){
                projects.append(ProjectAnimSetData());
                if (!projects.last().read(file)){
                    CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimSetData::parse(): ProjectAnimSetData read failed!"));
                    return false;
                }
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimSetData::parse(): Corrupted length of current block!"));
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
        for (int k = 0; k < projects.at(i).fileNames.size(); k++){
            out << projects.at(i).fileNames.at(k) << "\n";
        }
        for (int j = 0; j < projects.at(i).animSetData.size(); j++){
            out << projects.at(i).animSetData.at(j).write(&file, out) << "\n";
        }
    }
    return true;
}

bool SkyrimAnimSetData::addAnimationToCache(const QString &projectname, const QString & event, const QVector<SkyrimAnimSetData::AnimationInfo> &animations, const QVector<SkyrimAnimSetData::BehaviorVariable> &vars, const QVector<SkyrimAnimSetData::ClipInfo> &clips){
    int count = 0;
    int index = projectNames.indexOf(projectname);
    if (index < 0 || index >= projects.size()){
        return false;
    }
    for (int i = 0; i < projects.at(index).animSetData.size(); i++){
        if (projects.at(index).animSetData.at(i).cacheEvents.contains(event)){
            count++;
            if (!projects[index].animSetData[i].addAnimationToCache(event, animations, vars, clips)){
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
    for (int i = 0; i < projects.at(index).animSetData.size(); i++){
        projects[index].animSetData[i].removeAnimationFromCache(animationname, variablename, clipname);
    }
    return true;
}

bool SkyrimAnimSetData::extractProject(const QString &projectname){
    int index = 0;
    for (; index < projectNames.size(); index++){
        if (projectNames.at(index) == projectname){
            return projects.at(index).write(projectname);
        }
    }
    return false;
}

bool SkyrimAnimSetData::ProjectAnimSetData::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    uint numFiles = 0;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    numFiles = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < numFiles; i++){
        if (file->atEnd()){
            return false;
        }
        line = file->readLine();
        line.chop(1);
        if (!line.contains(".txt")){
            return false;
        }
        fileNames.append(line);
    }
    //Read individual project's animsetdata...
    for (uint i = 0; i < numFiles; i++){
        animSetData.append(AnimSetData());
        if (!animSetData.last().read(file)){
            return false;
        }
    }
    return true;
}

bool SkyrimAnimSetData::ProjectAnimSetData::write(const QString &projectfilename) const{
    QFile file(projectfilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }
    QTextStream out(&file);
    out << QString::number(fileNames.size()) << "\n";
    for (int i = 0; i < fileNames.size(); i++){
        out << fileNames.at(i) << "\n";
    }
    for (int i = 0; i < animSetData.size(); i++){
        if (!animSetData.at(i).write(&file, out)){
            return false;
        }
    }
    return true;
}

bool SkyrimAnimSetData::AnimSetData::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    uint numvars = 0;
    uint numclips = 0;
    uint numevents = 0;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    if (line != "V3" || file->atEnd()){
        return false;
    }
    //Read cache events...
    line = file->readLine();
    line.chop(1);
    numevents = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < numevents; i++){
        if (file->atEnd()){
            return false;
        }
        line = file->readLine();
        line.chop(1);
        cacheEvents.append(line);
    }
    line = file->readLine();
    line.chop(1);
    numvars = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    //Get attack animation data, if any...
    for (uint i = 0; i < numvars; i++){
        behaviorVariables.append(BehaviorVariable());
        if (!behaviorVariables.last().read(file)){
            return false;
        }
    }
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    numclips = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < numclips; i++){
        clipGenerators.append(ClipInfo());
        if (!clipGenerators.last().read(file)){
            return false;
        }
    }
    //Read encrypted animation names, paths...
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    numclips = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < numclips; i++){
        animations.append(AnimationInfo());
        if (!animations.last().read(file)){
            return false;
        }
    }
    return true;
}

bool SkyrimAnimSetData::AnimSetData::write(QFile *file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << "V3\n";
    out << QString::number(cacheEvents.size()) << "\n";
    for (int i = 0; i < cacheEvents.size(); i++){
        out << cacheEvents.at(i) << "\n";
    }
    out << QString::number(behaviorVariables.size()) << "\n";
    for (int i = 0; i < behaviorVariables.size(); i++){
        behaviorVariables.at(i).write(file, out);
    }
    out << QString::number(clipGenerators.size()) << "\n";
    for (int i = 0; i < clipGenerators.size(); i++){
        clipGenerators.at(i).write(file, out);
    }
    out << QString::number(animations.size()) << "\n";
    for (int i = 0; i < animations.size(); i++){
        animations.at(i).write(file, out);
    }
    return true;
}

bool SkyrimAnimSetData::AnimSetData::addAnimationToCache(const QString & event, const QVector<SkyrimAnimSetData::AnimationInfo> &anims, const QVector<SkyrimAnimSetData::BehaviorVariable> &vars, const QVector <ClipInfo> & clips){
    bool exists;
    if (!cacheEvents.contains(event)){
        cacheEvents.append(event);
    }
    for (int i = 0; i < anims.size(); i++){
        exists = false;
        for (int j = 0; j < animations.size(); j++){
            if (animations.at(j) == anims.at(i)){
                exists = true;
            }
        }
        if (!exists){
            animations.append(anims.at(i));
        }
    }
    for (int i = 0; i < vars.size(); i++){
        exists = false;
        for (int j = 0; j < behaviorVariables.size(); j++){
            if (behaviorVariables.at(j) == vars.at(i)){
                exists = true;
            }
        }
        if (!exists){
            behaviorVariables.append(vars.at(i));
        }
    }
    for (int i = 0; i < clips.size(); i++){
        exists = false;
        for (int j = 0; j < clipGenerators.size(); j++){
            if (clipGenerators.at(j) == clips.at(i)){
                exists = true;
            }
        }
        if (!exists){
            clipGenerators.append(clips.at(i));
        }
    }
    return true;
}


void SkyrimAnimSetData::AnimSetData::removeAnimationFromCache(const QString &animationname, const QString &clipname,  const QString &variablename){
    if (animationname != ""){
        QString animationhash = QString(HkCRC().compute(animationname.toLocal8Bit().toLower()));
        for (int i = animations.size() - 1; i >= 0; i--){
            if (animations.at(i).crcAnimationName == animationhash){
                animations.removeAt(i);
            }
        }
    }
    if (variablename != ""){
        for (int i = behaviorVariables.size() - 1; i >= 0; i--){
            if (behaviorVariables.at(i).name == variablename){
                behaviorVariables.removeAt(i);
            }
        }
    }
    if (clipname != ""){
        for (int i = clipGenerators.size() - 1; i >= 0; i--){
            clipGenerators[i].clipGenerators.removeAll(clipname);
        }
    }
}

bool SkyrimAnimSetData::BehaviorVariable::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    bool ok = false;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    name = line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    value1 = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    value2 = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    return true;
}

bool SkyrimAnimSetData::BehaviorVariable::write(QFile *file, QTextStream &out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << name << "\n";
    out << QString::number(value1) << "\n";
    out << QString::number(value2) << "\n";
    return true;
}

bool SkyrimAnimSetData::ClipInfo::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    uint numclips = 0;
    bool ok = false;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    eventName = line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    unknown = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    numclips = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < numclips; i++){
        if (file->atEnd()){
            return false;
        }
        line = file->readLine();
        line.chop(1);
        clipGenerators.append(line);
    }
    return true;
}

bool SkyrimAnimSetData::ClipInfo::write(QFile *file, QTextStream &out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << eventName << "\n";
    out << QString::number(unknown) << "\n";
    out << QString::number(clipGenerators.size()) << "\n";
    for (int i = 0; i < clipGenerators.size(); i++){
        out << clipGenerators.at(i) << "\n";
    }
    return true;
}

bool SkyrimAnimSetData::AnimationInfo::read(QFile *file){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    crcPath = line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    crcAnimationName = line;
    if (file->atEnd()){
        return false;
    }
    line = file->readLine();
    line.chop(1);
    if (line != "7891816"){
        return false;
    }
    return true;
}

bool SkyrimAnimSetData::AnimationInfo::write(QFile *file, QTextStream &out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << crcPath << "\n";
    out << crcAnimationName << "\n";
    out << "7891816" << "\n";
    return true;
}
