#ifndef PROJECTANIMDATA_H
#define PROJECTANIMDATA_H

#include "skyrimclipgeneratodata.h"
#include "skyrimanimationmotiondata.h"

class ProjectAnimData{
    friend class AnimationsUI;
    friend class SkyrimClipGeneratoData;
    friend class SkyrimAnimationMotionData;
public:
    ProjectAnimData();
    bool read(QFile * file);
    bool write(QFile &file, QTextStream &output) const;
    bool appendClipGenerator(const SkyrimClipGeneratoData & animData);
    bool removeClipGenerator(const QString & clipname);
    bool appendAnimation(const SkyrimAnimationMotionData &motiondata);
    bool removeAnimation(int animationindex);
private:
    uint animationDataLines;
    uint animationMotionDataLines;
    QStringList projectFiles;
    QVector <SkyrimClipGeneratoData> animationData;
    QVector <SkyrimAnimationMotionData> animationMotionData;
    bool chopLine(QFile *file, QByteArray &line, uint &linecount);
};

#endif // PROJECTANIMDATA_H
