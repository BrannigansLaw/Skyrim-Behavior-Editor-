#ifndef HKBFOOTIKCONTROLSMODIFIER_H
#define HKBFOOTIKCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbFootIkControlsModifier final: public hkbModifier
{
    friend class LegUI;
    friend class FootIkControlsModifierUI;
public:
    hkbFootIkControlsModifier(HkxFile *parent, long ref = 0);
    hkbFootIkControlsModifier& operator=(const hkbFootIkControlsModifier&) = delete;
    hkbFootIkControlsModifier(const hkbFootIkControlsModifier &) = delete;
    ~hkbFootIkControlsModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfLegs()const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
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
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    hkControlData gains;
    QVector <hkLeg> legs;
    hkQuadVariable errorOutTranslation;
    hkQuadVariable alignWithGroundRotation;
    mutable std::mutex mutex;
};

#endif // HKBFOOTIKCONTROLSMODIFIER_H
