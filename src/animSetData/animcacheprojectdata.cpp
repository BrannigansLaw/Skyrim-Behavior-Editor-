#include "animcacheprojectdata.h"
#include <QStringList>

AnimCacheProjectData::AnimCacheProjectData(const QStringList & files, const QVector<AnimCacheAnimSetData *> &animdata)
    : fileNames(files), animSetData(animdata)
{
    //
}

AnimCacheProjectData::~AnimCacheProjectData(){
    for (int i = 0; i < animSetData.size(); i++){
        if (animSetData.at(i)){
            delete animSetData.at(i);
        }
    }
}

bool AnimCacheProjectData::read(QFile *file){
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
        animSetData.append(new AnimCacheAnimSetData());
        if (!animSetData.last()->read(file)){
            return false;
        }
    }
    return true;
}

bool AnimCacheProjectData::write(QFile *file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << QString::number(fileNames.size()) << "\n";
    for (int i = 0; i < fileNames.size(); i++){
        out << fileNames.at(i) << "\n";
    }
    for (int i = 0; i < animSetData.size(); i++){
        if (!animSetData.at(i)->write(file, out)){
            return false;
        }
    }
    return true;
}
