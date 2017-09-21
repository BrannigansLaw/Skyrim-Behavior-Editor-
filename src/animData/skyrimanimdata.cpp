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
        if (!animData.at(i).write(file, out)){
            return false;
        }
    }
    return true;
}

int SkyrimAnimData::getProjectIndex(const QString &projectname) const{
    QString name(projectname);
    name.replace(".hkx", ".txt");
    for (int i = 0; i < projectNames.size(); i++){
        if (name.compare(projectNames.at(i), Qt::CaseInsensitive) == 0){
            return i;
        }
    }
    return -1;
}

bool SkyrimAnimData::appendClipGenerator(const QString & projectname, const SkyrimClipGeneratoData &animdata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData[index].appendClipGenerator(animdata);
}

bool SkyrimAnimData::removeClipGenerator(const QString &projectname, const QString &name){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData[index].removeClipGenerator(name);
}

bool SkyrimAnimData::appendAnimation(const QString &projectname, const SkyrimAnimationMotionData &motiondata){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData[index].appendAnimation(motiondata);
}

bool SkyrimAnimData::removeAnimation(const QString &projectname, int animationindex){
    int index = getProjectIndex(projectname);
    if (index < 0 || index >= animData.size()){
        return false;
    }
    return animData[index].removeAnimation(animationindex);
}
