#ifndef BSEVENTONFALSETOTRUEMODIFIER_H
#define BSEVENTONFALSETOTRUEMODIFIER_H

#include "hkbmodifier.h"

class BSEventOnFalseToTrueModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class BSEventOnFalseToTrueModifierUI;
public:
    BSEventOnFalseToTrueModifier(HkxFile *parent, long ref = 0);
    virtual ~BSEventOnFalseToTrueModifier();
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
    BSEventOnFalseToTrueModifier& operator=(const BSEventOnFalseToTrueModifier&);
    BSEventOnFalseToTrueModifier(const BSEventOnFalseToTrueModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    bool bEnableEvent1;
    bool bVariableToTest1;
    hkEventPayload eventToSend1;
    bool bEnableEvent2;
    bool bVariableToTest2;
    hkEventPayload eventToSend2;
    bool bEnableEvent3;
    bool bVariableToTest3;
    hkEventPayload eventToSend3;
};

#endif // BSEVENTONFALSETOTRUEMODIFIER_H
