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
protected:
    bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
    bool setChildAt(HkxObject *newChild, ushort index);
    bool appendObject(DataIconManager *objToAppend);
    bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
    int addChildrenToList(QList<DataIconManager *> & list, bool reverseOrder = true);
private:
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
