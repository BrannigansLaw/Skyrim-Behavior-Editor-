#ifndef HKBCLIPTRIGGERARRAY_H
#define HKBCLIPTRIGGERARRAY_H

#include "hkxobject.h"

class hkbClipTriggerArray: public HkxObject
{
public:
    hkbClipTriggerArray(BehaviorFile *parent, long ref = -1);
    virtual ~hkbClipTriggerArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    int getLastTriggerIndex() const;
    bool write(HkxXMLWriter *writer);
protected:
private:
    struct HkTrigger
    {
        HkTrigger()
            : localTime(0),
              id(-1),
              relativeToEndOfClip(false),
              acyclic(false),
              isAnnotation(false)
        {
            //
        }
        qreal localTime;
        int id;
        HkxObjectExpSharedPtr payload;
        bool relativeToEndOfClip;
        bool acyclic;
        bool isAnnotation;
    };
    void addTrigger(const HkTrigger & trigger = HkTrigger());
    void setTriggerId(int index, int id);
    void removeTrigger(int index);
private:
    static uint refCount;
    static QString classname;
    QList <HkTrigger> triggers;
};

#endif // HKBCLIPTRIGGERARRAY_H
