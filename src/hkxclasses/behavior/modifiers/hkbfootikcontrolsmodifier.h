#ifndef HKBFOOTIKCONTROLSMODIFIER_H
#define HKBFOOTIKCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbFootIkControlsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
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
private:
    hkbFootIkControlsModifier& operator=(const hkbFootIkControlsModifier&);
    hkbFootIkControlsModifier(const hkbFootIkControlsModifier &);
private:
    struct BsControlData{
        BsControlData()
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

    struct BSLeg{
        BSLeg()
            : id(-1),
              verticalError(0),
              hitSomething(false),
              isPlantedMS(false)
        {
            //
        }

        hkQuadVariable groundPosition;
        int id;
        HkxObjectExpSharedPtr payload;
        qreal verticalError;
        bool hitSomething;
        bool isPlantedMS;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    BsControlData gains;
    QList <BSLeg> legs;
    hkQuadVariable errorOutTranslation;
    hkQuadVariable alignWithGroundRotation;
};

#endif // HKBFOOTIKCONTROLSMODIFIER_H
