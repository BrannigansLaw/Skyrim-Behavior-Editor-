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
    bool appendClipGenerator(SkyrimClipGeneratoData *animData);
    bool removeClipGenerator(const QString & clipname);
    bool appendAnimation(SkyrimAnimationMotionData *motiondata);
    bool removeAnimation(int animationindex);
    SkyrimAnimationMotionData * findMotionData(int animationindex);
    void setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time);
    void setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, const QString &eventname);
    void setClipNameAnimData(const QString &oldclipname, const QString &newclipname);
    void setAnimationIndexForClipGen(const QString &clipGenName, int index);
    void setPlaybackSpeedForClipGen(const QString & clipGenName, qreal speed);
    void setCropStartAmountLocalTimeForClipGen(const QString & clipGenName, qreal time);
    void setCropEndAmountLocalTimeForClipGen(const QString & clipGenName, qreal time);
private:
    uint animationDataLines;
    uint animationMotionDataLines;
    QStringList projectFiles;
    QVector <SkyrimClipGeneratoData *> animationData;
    QVector <SkyrimAnimationMotionData *> animationMotionData;
    bool chopLine(QFile *file, QByteArray &line, uint &linecount);
};

#endif // PROJECTANIMDATA_H
