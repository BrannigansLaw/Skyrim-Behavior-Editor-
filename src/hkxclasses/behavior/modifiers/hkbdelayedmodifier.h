#ifndef HKBDELAYEDMODIFIER_H
#define HKBDELAYEDMODIFIER_H

#include "hkbmodifier.h"

class hkbDelayedModifier final: public hkbModifier
{
    friend class DelayedModifierUI;
public:
    hkbDelayedModifier(HkxFile *parent, long ref = 0);
    hkbDelayedModifier& operator=(const hkbDelayedModifier&) = delete;
    hkbDelayedModifier(const hkbDelayedModifier &) = delete;
    ~hkbDelayedModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
    QVector <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr modifier;
    qreal delaySeconds;
    qreal durationSeconds;
    qreal secondsElapsed;
    mutable std::mutex mutex;
};

#endif // HKBDELAYEDMODIFIER_H
