#ifndef HKBFOOTIKCONTROLSMODIFIER_H
#define HKBFOOTIKCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbFootIkControlsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class LegUI;
    friend class FootIkControlsModifierUI;
public:
    hkbFootIkControlsModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbFootIkControlsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    int getNumberOfLegs()const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
private:
    hkbFootIkControlsModifier& operator=(const hkbFootIkControlsModifier&);
    hkbFootIkControlsModifier(const hkbFootIkControlsModifier &);
private:
    struct hkControlData{
        hkControlData()
            : onOffGain(0),
              groundAscendingGain(0),
              groundDescendingGain(0),
              footPlantedGain(0),
              footRaisedGain(0),
              footUnlockGain(0),
              worldFromModelFeedbackGain(0),
              errorUpDownBias(0),
              alignWorldFromModelGain(0),
              hipOrientationGain(0),
              maxKneeAngleDifference(0),
              ankleOrientationGain(0)
        {
            //
        }

        qreal onOffGain;
        qreal groundAscendingGain;
        qreal groundDescendingGain;
        qreal footPlantedGain;
        qreal footRaisedGain;
        qreal footUnlockGain;
        qreal worldFromModelFeedbackGain;
        qreal errorUpDownBias;
        qreal alignWorldFromModelGain;
        qreal hipOrientationGain;
        qreal maxKneeAngleDifference;
        qreal ankleOrientationGain;
    };

    struct hkLeg{
        hkLeg()
            : id(-1),
              verticalError(0),
              hitSomething(false),
              isPlantedMS(false)
        {
            //
        }

        hkQuadVariable groundPosition;
        int id;
        HkxSharedPtr payload;
        qreal verticalError;
        bool hitSomething;
        bool isPlantedMS;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkControlData gains;
    QList <hkLeg> legs;
    hkQuadVariable errorOutTranslation;
    hkQuadVariable alignWithGroundRotation;
};

#endif // HKBFOOTIKCONTROLSMODIFIER_H
