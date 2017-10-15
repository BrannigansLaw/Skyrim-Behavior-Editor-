#include "animcacheanimsetdata.h"
#include "hkcrc.h"

AnimCacheAnimSetData::AnimCacheAnimSetData(const QStringList & events, const QVector <AnimCacheVariable> & vars, const QVector <AnimCacheClipInfo> & clips, const QVector <AnimCacheAnimationInfo> & anims)
    : cacheEvents(events), behaviorVariables(vars), clipGenerators(clips), animations(anims)
{
    //
}

bool AnimCacheAnimSetData::read(QFile *file){
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
        behaviorVariables.append(AnimCacheVariable());
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
        clipGenerators.append(AnimCacheClipInfo());
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
        animations.append(AnimCacheAnimationInfo());
        if (!animations.last().read(file)){
            return false;
        }
    }
    return true;
}

bool AnimCacheAnimSetData::write(QFile *file, QTextStream & out) const{
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

bool AnimCacheAnimSetData::addAnimationToCache(const QString & event, const QVector<AnimCacheAnimationInfo> &anims, const QVector<AnimCacheVariable> &vars, const QVector <AnimCacheClipInfo> & clips){
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


void AnimCacheAnimSetData::removeAnimationFromCache(const QString &animationname, const QString &clipname,  const QString &variablename){
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
