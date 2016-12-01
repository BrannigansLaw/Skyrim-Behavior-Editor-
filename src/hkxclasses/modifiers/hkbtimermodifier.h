#ifndef HKBTIMERMODIFIER_H
#define HKBTIMERMODIFIER_H

#include "hkbmodifier.h"

class hkbTimerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbTimerModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbTimerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbTimerModifier& operator=(const hkbTimerModifier&);
    hkbTimerModifier(const hkbTimerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal alarmTimeSeconds;
    int id;
    HkxObjectExpSharedPtr payload;
};

#endif // HKBTIMERMODIFIER_H
