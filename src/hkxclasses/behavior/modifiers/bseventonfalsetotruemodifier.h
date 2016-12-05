#ifndef BSEVENTONFALSETOTRUEMODIFIER_H
#define BSEVENTONFALSETOTRUEMODIFIER_H

#include "hkbmodifier.h"

class BSEventOnFalseToTrueModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSEventOnFalseToTrueModifier(HkxFile *parent, long ref = 0);
    virtual ~BSEventOnFalseToTrueModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSEventOnFalseToTrueModifier& operator=(const BSEventOnFalseToTrueModifier&);
    BSEventOnFalseToTrueModifier(const BSEventOnFalseToTrueModifier &);
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
    bool bEnableEvent1;
    bool bVariableToTest1;
    hkEvent eventToSend1;
    bool bEnableEvent2;
    bool bVariableToTest2;
    hkEvent eventToSend2;
    bool bEnableEvent3;
    bool bVariableToTest3;
    hkEvent eventToSend3;
};

#endif // BSEVENTONFALSETOTRUEMODIFIER_H
