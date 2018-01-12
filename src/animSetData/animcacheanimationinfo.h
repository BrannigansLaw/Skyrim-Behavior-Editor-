#ifndef ANIMCACHEANIMATIONINFO_H
#define ANIMCACHEANIMATIONINFO_H

#include <QtCore>

class AnimCacheAnimationInfo{
    friend class CacheWidget;
    friend class AnimCacheAnimSetData;
public:
    AnimCacheAnimationInfo(const QString & path = "", const QString & name = "", bool compute = false);
    AnimCacheAnimationInfo(const AnimCacheAnimationInfo & other);
    bool operator ==(const AnimCacheAnimationInfo &other) const;
    bool read(QFile * file);
    bool write(QFile * file, QTextStream & out) const;
    void setAnimationData(const QString & path, const QString & name, bool compute);
private:
    QString crcPath;
    QString crcAnimationName;
    //const QString xkh = "7891816";
};

#endif // ANIMCACHEANIMATIONINFO_H
