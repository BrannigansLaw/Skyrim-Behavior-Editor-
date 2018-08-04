#include "skyrimanimationmotiondata.h"
#include "projectanimdata.h"

bool SkyrimAnimationMotionData::chopLine(QFile * file, QByteArray & line, ulong & linecount){
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

SkyrimAnimationMotionData::SkyrimAnimationMotionData(ProjectAnimData *par, uint ind, qreal dur, const QVector <SkyrimAnimationTranslation> & trans, const QVector <SkyrimAnimationRotation> & rots)
    : parent(par), animationIndex(ind), duration(dur), translations(trans), rotations(rots)
{
    //
}

/*SkyrimAnimationMotionData::SkyrimAnimationMotionData(const SkyrimAnimationMotionData & other){
    parent = other.parent;
    animationIndex = other.animationIndex;
    duration = other.duration;
    translations = other.translations;
    rotations = other.rotations;
}*/

bool SkyrimAnimationMotionData::read(QFile *file, ulong &lineCount){
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
        translations.append(SkyrimAnimationTranslation());
        translations.last().localTime = list[0].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().x = list[1].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().y = list[2].toDouble(&ok);
        if (!ok){
            return false;
        }
        translations.last().z = list[3].toDouble(&ok);
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
        rotations.append(SkyrimAnimationRotation());
        rotations.last().localTime = list[0].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().x = list[1].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().y = list[2].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().z = list[3].toDouble(&ok);
        if (!ok){
            return false;
        }
        rotations.last().w = list[4].toDouble(&ok);
        if (!ok){
            return false;
        }
    }
    if (!chopLine(file, line, lineCount) || line != ""){
        return false;
    }
    return true;
}

QString SkyrimAnimationMotionData::trimFloat(QString & string) const{
    for (auto i = string.size() - 1; i >= 0; i--){
        if (string.at(i) == '0'){
            string.remove(i, 1);
        }else if (string.at(i) == '.'){
            string.remove(i, 1);
            break;
        }else{
            break;
        }
    }
    return string;
}

bool SkyrimAnimationMotionData::operator==(const SkyrimAnimationMotionData &other) const{
    if (animationIndex != other.animationIndex/* || duration != other.duration || translations.size() != other.translations.size() ||
            rotations.size() != other.rotations.size()*/)
    {
        return false;
    }
    return true;
}

bool SkyrimAnimationMotionData::write(QFile * file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << QString::number(animationIndex) << "\n";
    out << trimFloat(QString::number(duration, char('f'), 6)) << "\n";
    out << QString::number(translations.size()) << "\n";
    for (auto i = 0; i < translations.size(); i++){
        out << trimFloat(QString::number(translations.at(i).localTime, char('f'), 6));
        out << " "+trimFloat(QString::number(translations.at(i).x, char('f'), 6));
        out << " "+trimFloat(QString::number(translations.at(i).y, char('f'), 6));
        out << " "+trimFloat(QString::number(translations.at(i).z, char('f'), 6)) << "\n";
    }
    out << QString::number(rotations.size()) << "\n";
    for (auto i = 0; i < rotations.size(); i++){
        out << trimFloat(QString::number(rotations.at(i).localTime, char('f'), 6));
        out << " "+trimFloat(QString::number(rotations.at(i).x, char('f'), 6));
        out << " "+trimFloat(QString::number(rotations.at(i).y, char('f'), 6));
        out << " "+trimFloat(QString::number(rotations.at(i).z, char('f'), 6));
        out << " "+trimFloat(QString::number(rotations.at(i).w, char('f'), 6)) << "\n";
    }
    out << "\n";
    return true;
}

void SkyrimAnimationMotionData::addTranslation(const SkyrimAnimationTranslation & trans){
    translations.append(trans);
    parent->animationMotionDataLines++;
}

bool SkyrimAnimationMotionData::removeTranslation(int index){
    if (index >= 0 && index < translations.size()){
        translations.removeAt(index);
        parent->animationMotionDataLines--;
        return true;
    }
    return false;
}

void SkyrimAnimationMotionData::addRotation(const SkyrimAnimationRotation & rot){
    rotations.append(rot);
    parent->animationMotionDataLines++;
}
bool SkyrimAnimationMotionData::removeRotation(int index){
    if (index >= 0 && index < rotations.size()){
        rotations.removeAt(index);
        parent->animationMotionDataLines--;
        return true;
    }
    return false;
}

uint SkyrimAnimationMotionData::lineCount() const{
    return 4 + translations.size() + rotations.size() + 1;
}
