#ifndef HKBEVENTRANGEDATAARRAY_H
#define HKBEVENTRANGEDATAARRAY_H

#include "src/hkxclasses/hkxobject.h"

//class hkbEventsFromRangeModifier;

class hkbEventRangeDataArray: public HkxObject
{
    friend class EventRangeDataUI;
    friend class EventsFromRangeModifierUI;
    friend class hkbEventsFromRangeModifier;
public:
    hkbEventRangeDataArray(HkxFile *parent, long ref = -1);
    virtual ~hkbEventRangeDataArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QString evaluateDataValidity();
    static QString getClassname();
    int getLastEventDataIndex() const;
    bool write(HkxXMLWriter *writer);
    int getNumberOfRanges() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
protected:
private:
    struct hkbEventRangeData
    {
        static const QStringList EventRangeMode;    //(EVENT_MODE_SEND_ON_ENTER_RANGE=0;EVENT_MODE_SEND_WHEN_IN_RANGE=1)
        hkbEventRangeData()
            : upperBound(0),
              eventMode(EventRangeMode.first())
        {
            //
        }
        qreal upperBound;
        hkEventPayload event;
        QString eventMode;
    };
    void addEventData(const hkbEventRangeData & data = hkbEventRangeData());
    void setEventDataId(int index, int id);
    void removeEventData(int index);
private:
    static uint refCount;
    static QString classname;
    //hkbEventsFromRangeModifier *parent;
    QList <hkbEventRangeData> eventData;
};

#endif // HKBEVENTRANGEDATAARRAY_H
