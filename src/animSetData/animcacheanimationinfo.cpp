#include "animcacheanimationinfo.h"
#include "hkcrc.h"

AnimCacheAnimationInfo::AnimCacheAnimationInfo(const QString & path, const QString & name, bool compute){
    if (compute){
        HkCRC crcGen;
        crcPath = crcGen.compute(QString(path).replace("/", "\\").toLower().toLocal8Bit());
        crcPath = QString::number(crcPath.toULong(nullptr, 16));
        crcAnimationName = crcGen.compute(name.toLower().toLocal8Bit());
        crcAnimationName = QString::number(crcAnimationName.toULong(nullptr, 16));
    }else{
        crcPath = path;
        crcAnimationName = name;
    }
}

AnimCacheAnimationInfo::AnimCacheAnimationInfo(const AnimCacheAnimationInfo & other){
    crcPath = other.crcPath;
    crcAnimationName = other.crcAnimationName;
}

bool AnimCacheAnimationInfo::operator ==(const AnimCacheAnimationInfo & other) const{
    if (crcPath != other.crcPath || crcAnimationName != other.crcAnimationName){
        return false;
    }
    return true;
}

bool AnimCacheAnimationInfo::read(QFile *file){
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

bool AnimCacheAnimationInfo::write(QFile *file, QTextStream &out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << crcPath << "\n";
    out << crcAnimationName << "\n";
    out << "7891816" << "\n";
    return true;
}

void AnimCacheAnimationInfo::setAnimationData(const QString &path, const QString &name, bool compute){
    if (compute){
        HkCRC crcGen;
        crcPath = crcGen.compute(QString(path).replace("/", "\\").toLower().toLocal8Bit());
        crcPath = QString::number(crcPath.toULong(nullptr, 16));
        crcAnimationName = crcGen.compute(name.toLower().toLocal8Bit());
        crcAnimationName = QString::number(crcAnimationName.toULong(nullptr, 16));
    }else{
        crcPath = path;
        crcAnimationName = name;
    }
}
