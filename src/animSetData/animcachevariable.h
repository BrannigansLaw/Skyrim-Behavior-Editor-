#ifndef ANIMCACHEVARIABLE_H
#define ANIMCACHEVARIABLE_H

#include <QtCore>

class AnimCacheVariable{
    friend class AnimCacheAnimSetData;
public:
    AnimCacheVariable(const QString & name = "", uint val1 = 0, uint val2 = 0);
    AnimCacheVariable(const AnimCacheVariable & other);
    bool operator ==(const AnimCacheVariable & other) const;
    bool read(QFile * file);
    bool write(QFile * file, QTextStream & out) const;
private:
    QString name;
    uint value1;
    uint value2;
};

#endif // ANIMCACHEVARIABLE_H
