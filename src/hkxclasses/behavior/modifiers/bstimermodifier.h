#ifndef BSTIMERMODIFIER_H
#define BSTIMERMODIFIER_H

#include "hkbmodifier.h"

class BSTimerModifier final: public hkbModifier
{
    friend class BSTimerModifierUI;
public:
    BSTimerModifier(HkxFile *parent, long ref = 0);
    BSTimerModifier& operator=(const BSTimerModifier&) = delete;
    BSTimerModifier(const BSTimerModifier &) = delete;
    ~BSTimerModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    qreal alarmTimeSeconds;
    hkEventPayload alarmEvent;
    bool resetAlarm;
    mutable std::mutex mutex;
};

#endif // BSTIMERMODIFIER_H
