#include "skyrimanimdata.h"
#include "src/utility.h"
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
                            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimData::parse(): Corrupted project filename does not have 'txt' extension!"));
                            return false;
                        }
                    }else{
                        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimData::parse(): Unexpected EOF!"));
                        return false;
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimData::parse(): Corrupted length of current block!"));
                return false;
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimData::parse(): Unexpected EOF!"));
            return false;
        }
    }
    for (uint i = 0; i < size; i++){
        animData.append(ProjectAnimData());
        if (!animData.last().read(file)){
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimData::parse(): ProjectAnimData read failed!"));
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
        out << animData.at(i).write(file, out) << "\n";
    }
    return true;
}

int SkyrimAnimData::getProjectIndex(const QString &projectname) const{
    for (int i = 0; i < projectNames.size(); i++){
        if (projectNames.at(i) == projectname){
            return i;
        }
    }
    return -1;
}

bool SkyrimAnimData::appendClipGenerator(const QString & projectname, const SkyrimAnimData::AnimData &animdata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData.at(index).appendClipGenerator(clipname);
}

bool SkyrimAnimData::removeClipGenerator(const QString &projectname, const QString &name){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData.at(index).removeClipGenerator(clipname);
}

bool SkyrimAnimData::appendAnimation(const QString &projectname, const SkyrimAnimData::AnimMotionData &motiondata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData.at(index).appendAnimation(motiondata);
}

bool SkyrimAnimData::removeAnimation(const QString &projectname, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData.at(index).removeAnimation(animationindex);
}

bool SkyrimAnimData::ProjectAnimData::write(QFile & file, QTextStream & out) const{
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
    }
    for (int i = 0; i < animationData.size(); i++){
        if (!animationData.at(i).write(&file, out)){
            return false;
        }
    }
    out << QString::number(animationMotionDataLines) << "\n";
    for (int i = 0; i < animationMotionData.size(); i++){
        if (!animationMotionData.at(i).write(&file, out)){
            return false;
        }
    }
    return true;
}

bool SkyrimAnimData::ProjectAnimData::appendClipGenerator(const SkyrimAnimData::AnimData &animdata){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i).clipGeneratorName == animdata.clipGeneratorName){
            return false;
        }
    }
    animationData.append(animdata);
    animationDataLines = animationDataLines + animdata.lineCount();
    return true;
}

bool SkyrimAnimData::ProjectAnimData::removeClipGenerator(const QString &clipname){
    for (int i = 0; i < animationData.size(); i++){
        if (animationData.at(i).clipGeneratorName == clipname){
            animationDataLines = animationDataLines - animationData.at(i).lineCount();
            animationData.removeAt(i);
            return true;
        }
    }
    return false;
}

bool SkyrimAnimData::ProjectAnimData::appendAnimation(const SkyrimAnimData::AnimMotionData &motiondata){
    for (int i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i).animationIndex == motiondata.animationIndex){
            return false;
        }
    }
    animationMotionData.append(motiondata);
    animationMotionDataLines = animationMotionDataLines + motiondata.lineCount();
    return true;
}

bool SkyrimAnimData::ProjectAnimData::removeAnimation(int animationindex){
    for (int i = 0; i < animationMotionData.size(); i++){
        if (animationMotionData.at(i).animationIndex == animationindex){
            animationMotionDataLines = animationMotionDataLines - animationMotionData.at(i).lineCount();
            animationMotionData.removeAt(i);
            return true;
        }
    }
    return false;
}

bool SkyrimAnimData::AnimData::read(QFile *file, uint &lineCount){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    QList <QByteArray> list;
    bool ok = false;
    uint index = 0;
    uint size = 0;
    qreal value = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    clipGeneratorName = line;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    index = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    animationIndex = index;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    value = line.toDouble(&ok);
    if (!ok){
        return false;
    }
    playbackSpeed = value;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    value = line.toDouble(&ok);
    if (!ok){
        return false;
    }
    cropStartTime = value;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    value = line.toDouble(&ok);
    if (!ok){
        return false;
    }
    cropEndTime = value;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    size = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < size; i++){
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        list = line.split(':');
        if (list.size() != 2){
            return false;
        }
        triggers.append(Trigger());
        triggers.last().name = list.first();
        triggers.last().time = list[1].toDouble(&ok);
        if (!ok){
            return false;
        }
    }
    if (!chopLine(file, line, lineCount) || line != ""){
        return false;
    }
    return true;
}

bool SkyrimAnimData::AnimData::write(QFile *file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << clipGeneratorName << "\n";
    out << QString::number(animationIndex) << "\n";
    out << QString::number(playbackSpeed) << "\n";
    out << QString::number(cropStartTime) << "\n";
    out << QString::number(cropEndTime) << "\n";
    out << QString::number(triggers.size()) << "\n";
    for (int i = 0; i < triggers.size(); i++){
        out << triggers.at(i).name+":"+QString::number(triggers.at(i).time) << "\n";
    }
    out << "\n";
    return true;
}

uint SkyrimAnimData::AnimData::lineCount() const{
    return 6 + triggers.size() + 1;
}

bool SkyrimAnimData::AnimMotionData::read(QFile *file, uint &lineCount){
    if (!file || !file->isOpen()){
        return false;
    }
    QByteArray line;
    QList <QByteArray> list;
    bool ok = false;
    uint index = 0;
    uint count = 0;
    qreal value = 0;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    index = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    animationIndex = index;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    value = line.toDouble(&ok);
    if (!ok){
        return false;
    }
    duration = value;
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    count = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < count; i++){
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        list = line.split(' ');
        if (list.size() != 4){
            return false;
        }
        translations.append(Translation());
        translations.last().x = list[0].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().y = list[1].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().z = list[2].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().w = list[3].toDouble(&ok);
        if (!ok){
            return false;
        }
    }
    if (!chopLine(file, line, lineCount)){
        return false;
    }
    count = line.toUInt(&ok);
    if (!ok){
        return false;
    }
    for (uint i = 0; i < count; i++){
        if (!chopLine(file, line, lineCount)){
            return false;
        }
        list = line.split(' ');
        if (list.size() != 5){
            return false;
        }
        rotations.append(Rotation());
        rotations.last().coord1 = list[0].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().coord2 = list[1].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().coord3 = list[2].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().coord4 = list[3].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().coord5 = list[4].toDouble(&ok);
        if (!ok){
            return false;
        }
    }
    if (!chopLine(file, line, lineCount) || line != ""){
        return false;
    }
    return true;
}

bool SkyrimAnimData::AnimMotionData::write(QFile * file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << QString::number(animationIndex) << "\n";
    out << QString::number(duration) << "\n";
    out << QString::number(translations.size()) << "\n";
    for (int i = 0; i < translations.size(); i++){
        out << QString::number(translations.at(i).x);
        out << " "+QString::number(translations.at(i).y);
        out << " "+QString::number(translations.at(i).z);
        out << " "+QString::number(translations.at(i).w) << "\n";
    }
    out << QString::number(rotations.size()) << "\n";
    for (int i = 0; i < rotations.size(); i++){
        out << QString::number(rotations.at(i).coord1);
        out << " "+QString::number(rotations.at(i).coord2);
        out << " "+QString::number(rotations.at(i).coord3);
        out << " "+QString::number(rotations.at(i).coord4);
        out << " "+QString::number(rotations.at(i).coord5) << "\n";
    }
    out << "\n";
    return true;
}

uint SkyrimAnimData::AnimMotionData::lineCount() const{
    return 4 + translations.size() + rotations.size() + 1;
}

bool chopLine(QFile * file, QByteArray & line, uint & linecount){
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

bool SkyrimAnimData::ProjectAnimData::read(QFile *file){
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
                animationData.append(AnimData());
                if (!animationData.last().read(file, lineCount)){
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
                animationMotionData.append(AnimMotionData());
                if (!animationMotionData.last().read(file, lineCount)){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
