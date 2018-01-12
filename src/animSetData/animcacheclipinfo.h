#ifndef ANIMCACHECLIPINFO_H
#define ANIMCACHECLIPINFO_H

#include <QtCore>

class AnimCacheClipInfo{
    friend class CacheWidget;
    friend class CacheClipWidget;
    friend class AnimCacheAnimSetData;
public:
    AnimCacheClipInfo(const QString & eventname = "", const QStringList & clipgens = QStringList(), uint unk = 0);
    AnimCacheClipInfo(const AnimCacheClipInfo & other);
    bool operator ==(const AnimCacheClipInfo & other) const;
    bool read(QFile * file);
    bool write(QFile * file, QTextStream & out) const;
private:
    QString eventName;
    uint unknown;   //Appears to be 1 for sprinting and normal attacks, zero otherwise in defaultmale...
    QStringList clipGenerators;
};

#endif // ANIMCACHECLIPINFO_H
