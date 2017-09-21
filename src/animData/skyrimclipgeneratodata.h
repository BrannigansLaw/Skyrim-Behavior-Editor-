#ifndef SKYRIMCLIPGENERATODATA_H
#define SKYRIMCLIPGENERATODATA_H

#include <QTextStream>

#include "skyrimcliptrigger.h"

class ProjectAnimData;

class SkyrimClipGeneratoData
{
    friend class ProjectAnimData;
public:
    SkyrimClipGeneratoData();
    SkyrimClipGeneratoData(ProjectAnimData *par, uint ind = 0, qreal speed = 0, qreal startcrop = 0, qreal endcrop = 0, const QVector <SkyrimClipTrigger> & trigs = QVector <SkyrimClipTrigger>());
    SkyrimClipGeneratoData(const SkyrimClipGeneratoData & other);
    bool read(QFile * file, uint & lineCount);
    bool write(QFile * file, QTextStream & out) const;
    uint lineCount() const;
    void addTrigger(const SkyrimClipTrigger &trig = SkyrimClipTrigger());
    bool removeTrigger(int index);
private:
    ProjectAnimData *parent;
    QString clipGeneratorName;
    uint animationIndex;
    qreal playbackSpeed;
    qreal cropStartTime;
    qreal cropEndTime;
    QVector <SkyrimClipTrigger> triggers;
    bool chopLine(QFile *file, QByteArray &line, uint &linecount);
};

#endif // SKYRIMCLIPGENERATODATA_H
