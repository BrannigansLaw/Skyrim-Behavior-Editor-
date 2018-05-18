#ifndef BSISTATEMANAGERMODIFIER_H
#define BSISTATEMANAGERMODIFIER_H

#include "hkbmodifier.h"

class BSIStateManagerModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    BSIStateManagerModifier(HkxFile *parent, long ref = 0);
    virtual ~BSIStateManagerModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    BSIStateManagerModifier& operator=(const BSIStateManagerModifier&);
    BSIStateManagerModifier(const BSIStateManagerModifier &);
private:
    struct BSiStateData{
        BSiStateData()
            : stateID(-1),
              iStateToSetAs(-1)
        {
            //
        }

        HkxSharedPtr pStateMachine;    //This may preserve state machines unexpectedly...
        int stateID;
        int iStateToSetAs;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    int iStateVar;
    QList <BSiStateData> stateData;
};

#endif // BSISTATEMANAGERMODIFIER_H
