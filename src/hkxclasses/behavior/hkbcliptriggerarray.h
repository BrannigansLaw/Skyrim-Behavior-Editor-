#ifndef HKBCLIPTRIGGERARRAY_H
#define HKBCLIPTRIGGERARRAY_H

#include "src/hkxclasses/hkxobject.h"

class BehaviorFile;

class hkbClipTriggerArray: public HkxObject
{
    friend class hkbClipGenerator;
    friend class ClipGeneratorUI;
    friend class ClipTriggerUI;
public:
    hkbClipTriggerArray(HkxFile *parent, long ref = -1);
    virtual ~hkbClipTriggerArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaluateDataValidity();
    static QString getClassname();
    int getLastTriggerIndex() const;
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    bool merge(HkxObject *recessiveObject);
    void updateReferences(long &ref);
protected:
private:
    struct HkTrigger
    {
        HkTrigger()
            : localTime(0),
              relativeToEndOfClip(false),
              acyclic(false),
              isAnnotation(false)
        {
            //
        }
        qreal localTime;
        hkEventPayload event;
        bool relativeToEndOfClip;
        bool acyclic;
        bool isAnnotation;
    };
    void addTrigger(const HkTrigger & trigger = HkTrigger());
    void setTriggerId(int index, int id);
    void setLocalTime(int index, qreal time);
    void removeTrigger(int index);
private:
    static uint refCount;
    static QString classname;
    QList <HkTrigger> triggers;
};

#endif // HKBCLIPTRIGGERARRAY_H
