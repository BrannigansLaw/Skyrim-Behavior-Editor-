#ifndef ANIMCACHEANIMSETDATA_H
#define ANIMCACHEANIMSETDATA_H

#include <QtCore>

#include "animcachevariable.h"
#include "animcacheanimationinfo.h"
#include "animcacheclipinfo.h"

class AnimCacheAnimSetData{
    friend class AnimCacheProjectData;
    friend class SkyrimAnimSetData;
public:
    AnimCacheAnimSetData(const QStringList & events = QStringList(), const QVector <AnimCacheVariable> & vars = QVector <AnimCacheVariable>(),
                const QVector <AnimCacheClipInfo> & clips = QVector <AnimCacheClipInfo>(), const QVector <AnimCacheAnimationInfo> & anims = QVector <AnimCacheAnimationInfo>());
    bool read(QFile * file);
    bool write(QFile *file, QTextStream &out) const;
    bool addAnimationToCache(const QString & event, const QVector <AnimCacheAnimationInfo> & anims, const QVector <AnimCacheVariable> & vars = QVector <AnimCacheVariable> (), const QVector <AnimCacheClipInfo> & clips = QVector <AnimCacheClipInfo> ());
    void removeAnimationFromCache(const QString & animationname, const QString & clipname = "", const QString & variablename = "");
private:
    QStringList cacheEvents;
    QVector <AnimCacheVariable> behaviorVariables;
    QVector <AnimCacheClipInfo> clipGenerators;
    QVector <AnimCacheAnimationInfo> animations;
};

#endif // ANIMCACHEANIMSETDATA_H
