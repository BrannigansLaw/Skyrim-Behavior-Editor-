#ifndef BSEVENTEVERYNEVENTSMODIFIER_H
#define BSEVENTEVERYNEVENTSMODIFIER_H

#include "hkbmodifier.h"

class BSEventEveryNEventsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSEventEveryNEventsModifier(BehaviorFile *parent, long ref = 0);
    virtual ~BSEventEveryNEventsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSEventEveryNEventsModifier& operator=(const BSEventEveryNEventsModifier&);
    BSEventEveryNEventsModifier(const BSEventEveryNEventsModifier &);
private:
    struct hkEvent{
        hkEvent(): id(-1){}
        int id;
        HkxObjectExpSharedPtr payload;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEvent eventToCheckFor;
    hkEvent eventToSend;
    int numberOfEventsBeforeSend;
    int minimumNumberOfEventsBeforeSend;
    bool randomizeNumberOfEvents;
};

#endif // BSEVENTEVERYNEVENTSMODIFIER_H
