#ifndef ANIMCACHEVARIABLE_H
#define ANIMCACHEVARIABLE_H

#include <QtCore>

class AnimCacheVariable{
    friend class CacheWidget;
    friend class AnimCacheAnimSetData;
    friend class CacheVariableWidget;
public:
    AnimCacheVariable(const QString & name = "", uint val1 = 0, uint val2 = 0);
    AnimCacheVariable(const AnimCacheVariable & other);
    bool operator ==(const AnimCacheVariable & other) const;
    bool read(QFile * file);
    bool write(QFile * file, QTextStream & out) const;
private:
    QString name;
    uint minValue;
    uint maxValue;
};

#endif // ANIMCACHEVARIABLE_H
