#ifndef ANIMCACHEPROJECTDATA_H
#define ANIMCACHEPROJECTDATA_H

#include <QtCore>

#include "animcacheanimsetdata.h"

class AnimCacheProjectData{
    friend class SkyrimAnimSetData;
public:
    AnimCacheProjectData(const QStringList & files = QStringList(), const QVector <AnimCacheAnimSetData> & animdata = QVector <AnimCacheAnimSetData>());
    bool read(QFile * file);
    bool write(QFile *file, QTextStream &out) const;
private:
    QStringList fileNames;
    QVector <AnimCacheAnimSetData> animSetData;
};

#endif // ANIMCACHEPROJECTDATA_H
