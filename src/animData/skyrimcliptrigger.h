#ifndef SKYRIMCLIPTRIGGER_H
#define SKYRIMCLIPTRIGGER_H

#include <QtCore>

class SkyrimClipTrigger
{
    friend class SkyrimClipGeneratoData;
public:
    SkyrimClipTrigger(qreal t = 0, const QString & trigname = "");
private:
    QString name;
    qreal time;
};

#endif // SKYRIMCLIPTRIGGER_H
