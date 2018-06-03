#ifndef BSTIMERMODIFIER_H
#define BSTIMERMODIFIER_H

#include "hkbmodifier.h"

class BSTimerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSTimerModifierUI;
public:
    BSTimerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSTimerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
private:
    BSTimerModifier& operator=(const BSTimerModifier&);
    BSTimerModifier(const BSTimerModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal alarmTimeSeconds;
    hkEventPayload alarmEvent;
    bool resetAlarm;
};

#endif // BSTIMERMODIFIER_H
