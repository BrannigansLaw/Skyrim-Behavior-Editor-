#ifndef BSEVENTEVERYNEVENTSMODIFIER_H
#define BSEVENTEVERYNEVENTSMODIFIER_H

#include "hkbmodifier.h"

class BSEventEveryNEventsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSEventEveryNEventsModifierUI;
public:
    BSEventEveryNEventsModifier(HkxFile *parent, long ref = 0);
    virtual ~BSEventEveryNEventsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    QString evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    BSEventEveryNEventsModifier& operator=(const BSEventEveryNEventsModifier&);
    BSEventEveryNEventsModifier(const BSEventEveryNEventsModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload eventToCheckFor;
    hkEventPayload eventToSend;
    int numberOfEventsBeforeSend;
    int minimumNumberOfEventsBeforeSend;
    bool randomizeNumberOfEvents;
};

#endif // BSEVENTEVERYNEVENTSMODIFIER_H
