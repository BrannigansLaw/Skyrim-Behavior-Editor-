#ifndef HKBDELAYEDMODIFIER_H
#define HKBDELAYEDMODIFIER_H

#include "hkbmodifier.h"

class hkbDelayedModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbDelayedModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbDelayedModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool hasChildren() const;
private:
    QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int, DataIconManager *obj);
    bool removeObjectAt(int index);
    hkbDelayedModifier& operator=(const hkbDelayedModifier&);
    hkbDelayedModifier(const hkbDelayedModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxObjectExpSharedPtr modifier;
    qreal delaySeconds;
    qreal durationSeconds;
    qreal secondsElapsed;
};

#endif // HKBDELAYEDMODIFIER_H
