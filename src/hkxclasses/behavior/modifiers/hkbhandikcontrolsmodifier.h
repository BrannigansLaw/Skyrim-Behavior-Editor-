#ifndef HKBHANDIKCONTROLSMODIFIER_H
#define HKBHANDIKCONTROLSMODIFIER_H

#include "hkbmodifier.h"

class hkbHandIkControlsModifier final: public hkbModifier
{
    friend class HandUI;
    friend class HandIkControlsModifierUI;
public:
    hkbHandIkControlsModifier(HkxFile *parent, long ref = 0);
    hkbHandIkControlsModifier& operator=(const hkbHandIkControlsModifier&) = delete;
    hkbHandIkControlsModifier(const hkbHandIkControlsModifier &) = delete;
    ~hkbHandIkControlsModifier();
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfHands()const;
private:
    struct hkControlData{
        hkControlData()
            : transformOnFraction(0),
              normalOnFraction(0),
              fadeInDuration(0),
              fadeOutDuration(0),
              extrapolationTimeStep(0),
              handleChangeSpeed(0),
              handleChangeMode("HANDLE_CHANGE_MODE_ABRUPT"),
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
        hkHand() : enable(false){}
        hkControlData controlData;
        int handIndex;
        bool enable;
    };
private:
    static uint refCount;
    static const QString classname;
    static const QStringList HandleChangeMode;
    long userData;
    QString name;
    bool enable;
    QVector <hkHand> hands;
    mutable std::mutex mutex;
};

#endif // HKBHANDIKCONTROLSMODIFIER_H
