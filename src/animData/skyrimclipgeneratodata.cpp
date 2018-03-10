#include "skyrimclipgeneratodata.h"
#include "projectanimdata.h"

bool SkyrimClipGeneratoData::chopLine(QFile * file, QByteArray & line, uint & linecount){
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

SkyrimClipGeneratoData::SkyrimClipGeneratoData(ProjectAnimData *par, const QString & name, uint ind, qreal speed, qreal startcrop, qreal endcrop, const QVector <SkyrimClipTrigger> & trigs)
    : parent(par), clipGeneratorName(name), animationIndex(ind), playbackSpeed(speed), cropStartTime(startcrop), cropEndTime(endcrop), triggers(trigs)
{
    //
}

SkyrimClipGeneratoData::SkyrimClipGeneratoData(const SkyrimClipGeneratoData & other){
    parent = other.parent;
    clipGeneratorName = other.clipGeneratorName;
    animationIndex = other.animationIndex;
    playbackSpeed = other.playbackSpeed;
    cropStartTime = other.cropStartTime;
    cropEndTime = other.cropEndTime;
    triggers = other.triggers;
}

void SkyrimClipGeneratoData::addTrigger(const SkyrimClipTrigger & trig){
    triggers.append(trig);
    parent->animationDataLines++;
}

bool SkyrimClipGeneratoData::removeTrigger(int index){
    if (index >= 0 && index < triggers.size()){
        triggers.removeAt(index);
        parent->animationDataLines--;
        return true;
    }
    return false;
}

QString SkyrimClipGeneratoData::getClipGeneratorName() const{
    return clipGeneratorName;
}

bool SkyrimClipGeneratoData::read(QFile *file, uint &lineCount){
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
        triggers.append(SkyrimClipTrigger());
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

bool SkyrimClipGeneratoData::write(QFile *file, QTextStream & out) const{
    if (!file || !file->isOpen()){
        return false;
    }
    out << clipGeneratorName << "\n";
    out << QString::number(animationIndex) << "\n";
    out << QString::number(playbackSpeed, char('f'), 6) << "\n";
    out << QString::number(cropStartTime, char('f'), 6) << "\n";
    out << QString::number(cropEndTime, char('f'), 6) << "\n";
    out << QString::number(triggers.size()) << "\n";
    for (int i = 0; i < triggers.size(); i++){
        out << triggers.at(i).name+":"+QString::number(triggers.at(i).time, char('f'), 6) << "\n";
    }
    out << "\n";
    return true;
}

uint SkyrimClipGeneratoData::lineCount() const{
    return 6 + triggers.size() + 1;
}
