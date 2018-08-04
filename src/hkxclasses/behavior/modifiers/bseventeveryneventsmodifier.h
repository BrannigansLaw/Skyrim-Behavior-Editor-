#ifndef BSEVENTEVERYNEVENTSMODIFIER_H
#define BSEVENTEVERYNEVENTSMODIFIER_H

#include "hkbmodifier.h"

class BSEventEveryNEventsModifier final: public hkbModifier
{
    friend class BSEventEveryNEventsModifierUI;
public:
    BSEventEveryNEventsModifier(HkxFile *parent, long ref = 0);
    BSEventEveryNEventsModifier& operator=(const BSEventEveryNEventsModifier&) = delete;
    BSEventEveryNEventsModifier(const BSEventEveryNEventsModifier &) = delete;
    ~BSEventEveryNEventsModifier();
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
    hkEventPayload eventToCheckFor;
    hkEventPayload eventToSend;
    int numberOfEventsBeforeSend;
    int minimumNumberOfEventsBeforeSend;
    bool randomizeNumberOfEvents;
    mutable std::mutex mutex;
};

#endif // BSEVENTEVERYNEVENTSMODIFIER_H
