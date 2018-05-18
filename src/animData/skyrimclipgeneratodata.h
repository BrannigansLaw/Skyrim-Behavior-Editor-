#ifndef SKYRIMCLIPGENERATODATA_H
#define SKYRIMCLIPGENERATODATA_H

#include <QTextStream>

#include "skyrimcliptrigger.h"

class ProjectAnimData;

class SkyrimClipGeneratoData
{
    friend class ProjectAnimData;
public:
    SkyrimClipGeneratoData(ProjectAnimData *par, const QString & name, uint ind = 0, qreal speed = 0, qreal startcrop = 0, qreal endcrop = 0, const QVector <SkyrimClipTrigger> & trigs = QVector <SkyrimClipTrigger>());
    SkyrimClipGeneratoData(const SkyrimClipGeneratoData & other);
    bool read(QFile * file, ulong &lineCount);
    bool write(QFile * file, QTextStream & out);
    uint lineCount() const;
    void addTrigger(const SkyrimClipTrigger &trig = SkyrimClipTrigger());
    bool removeTrigger(int index);
    QString getClipGeneratorName() const;
    QString trimFloat(QString &string) const;
    void rearrangeTriggers();
private:
    ProjectAnimData *parent;
    QString clipGeneratorName;
    uint animationIndex;
    qreal playbackSpeed;
    qreal cropStartTime;
    qreal cropEndTime;
    QVector <SkyrimClipTrigger> triggers;
    bool chopLine(QFile *file, QByteArray &line, ulong &linecount);
};

#endif // SKYRIMCLIPGENERATODATA_H
