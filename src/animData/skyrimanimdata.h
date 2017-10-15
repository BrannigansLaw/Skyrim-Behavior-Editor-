#ifndef SKYRIMANIMDATA_H
#define SKYRIMANIMDATA_H

#include <QString>
#include <QFile>
#include <QVector>

#include "projectanimdata.h"

class QTextStream;

class SkyrimAnimData
{
    friend class ProjectUI;
public:
    SkyrimAnimData();
    bool parse(QFile * file);
    //bool extract(const QString & projectname);
    bool write(const QString & filename);
    int getProjectIndex(const QString & projectname) const;
    bool appendClipGenerator(const QString &projectname, SkyrimClipGeneratoData *animdata);
    bool removeClipGenerator(const QString & projectname, const QString & clipname);
    bool appendAnimation(const QString & projectname, SkyrimAnimationMotionData *motiondata);
    bool removeAnimation(const QString & projectname, int animationindex);
private:
    QStringList projectNames;
    QVector <ProjectAnimData *> animData;
};

#endif // SKYRIMANIMDATA_H
