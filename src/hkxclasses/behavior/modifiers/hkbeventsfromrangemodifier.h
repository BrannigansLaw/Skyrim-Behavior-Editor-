#ifndef HKBEVENTSFROMRANGEMODIFIER_H
#define HKBEVENTSFROMRANGEMODIFIER_H

#include "hkbmodifier.h"

class hkbEventsFromRangeModifier final: public hkbModifier
{
    friend class EventsFromRangeModifierUI;
public:
    hkbEventsFromRangeModifier(HkxFile *parent, long ref = 0);
    hkbEventsFromRangeModifier& operator=(const hkbEventsFromRangeModifier&) = delete;
    hkbEventsFromRangeModifier(const hkbEventsFromRangeModifier &) = delete;
    ~hkbEventsFromRangeModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfRanges() const;
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
    qreal inputValue;
    qreal lowerBound;
    HkxSharedPtr eventRanges;
    mutable std::mutex mutex;
};

#endif // HKBEVENTSFROMRANGEMODIFIER_H
