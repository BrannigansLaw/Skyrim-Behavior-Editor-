#ifndef HKBHANDIKCONTROLSMODIFIER_H
#define HKBHANDIKCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbHandIkControlsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class HandUI;
    friend class HandIkControlsModifierUI;
public:
    hkbHandIkControlsModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbHandIkControlsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfHands()const;
private:
    hkbHandIkControlsModifier& operator=(const hkbHandIkControlsModifier&);
    hkbHandIkControlsModifier(const hkbHandIkControlsModifier &);
private:
    struct hkControlData{
        hkControlData()
            : transformOnFraction(0),
              normalOnFraction(0),
              fadeInDuration(0),
              fadeOutDuration(0),
              extrapolationTimeStep(0),
              handleChangeSpeed(0),
              handleChangeMode("HANDLE_CHANGE_MODE_ABRUPT"),//HANDLE_CHANGE_MODE_CONSTANT_VELOCITY
              fixUp(false)
        {
            //
        }

        hkQuadVariable targetPosition;
        hkQuadVariable targetRotation;
        hkQuadVariable targetNormal;
        HkxSharedPtr targetHandle;
        qreal transformOnFraction;
        qreal normalOnFraction;
        qreal fadeInDuration;
        qreal fadeOutDuration;
        qreal extrapolationTimeStep;
        qreal handleChangeSpeed;
        QString handleChangeMode;
        bool fixUp;
    };

    struct hkHand{
        hkHand()
            : enable(false)
        {
            //
        }

        hkControlData controlData;
        int handIndex;
        bool enable;
    };

    static QStringList HandleChangeMode;
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    QList <hkHand> hands;
};

#endif // HKBHANDIKCONTROLSMODIFIER_H
