#ifndef HKBEVENTSFROMRANGEMODIFIER_H
#define HKBEVENTSFROMRANGEMODIFIER_H

#include "hkbmodifier.h"

class hkbEventsFromRangeModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class EventsFromRangeModifierUI;
public:
    hkbEventsFromRangeModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbEventsFromRangeModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfRanges() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
private:
    hkbEventsFromRangeModifier& operator=(const hkbEventsFromRangeModifier&);
    hkbEventsFromRangeModifier(const hkbEventsFromRangeModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    qreal inputValue;
    qreal lowerBound;
    HkxSharedPtr eventRanges;
};

#endif // HKBEVENTSFROMRANGEMODIFIER_H
