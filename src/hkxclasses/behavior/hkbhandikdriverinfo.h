#ifndef HKBHANDIKDRIVERINFO_H
#define HKBHANDIKDRIVERINFO_H

#include "src/hkxclasses/hkxobject.h"

class hkbHandIkDriverInfo: public HkxObject
{
    friend class BehaviorGraphView;
    friend class HandIkDriverInfoUI;
    friend class HandIkDriverInfoHandUI;
public:
    hkbHandIkDriverInfo(HkxFile *parent, long ref = 0);
    virtual ~hkbHandIkDriverInfo();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    void addHand();
    void removeHandAt(int index);
    hkbHandIkDriverInfo& operator=(const hkbHandIkDriverInfo&);
    hkbHandIkDriverInfo(const hkbHandIkDriverInfo &);
private:
    struct hkbHandIkDriverInfoHand{
        hkbHandIkDriverInfoHand()
            : maxElbowAngleDegrees(0),
              minElbowAngleDegrees(0),
              shoulderIndex(-1),
              shoulderSiblingIndex(-1),
              elbowIndex(-1),
              elbowSiblingIndex(-1),
              wristIndex(-1),
              enforceEndPosition(false),
              enforceEndRotation(false)
        {
            //
        }

        hkQuadVariable elbowAxisLS;
        hkQuadVariable backHandNormalLS;
        hkQuadVariable handOffsetLS;
        hkQuadVariable handOrienationOffsetLS;
        qreal maxElbowAngleDegrees;
        qreal minElbowAngleDegrees;
        int shoulderIndex;
        int shoulderSiblingIndex;
        int elbowIndex;
        int elbowSiblingIndex;
        int wristIndex;
        bool enforceEndPosition;
        bool enforceEndRotation;
        QString localFrameName;
    };

    hkbHandIkDriverInfoHand * getHandAt(int index);

    static QStringList BlendCurve;  //BLEND_CURVE_SMOOTH=0;BLEND_CURVE_LINEAR=1;BLEND_CURVE_LINEAR_TO_SMOOTH=2;BLEND_CURVE_SMOOTH_TO_LINEAR=3
    static uint refCount;
    static QString classname;
    QList <hkbHandIkDriverInfoHand> hands;
    QString fadeInOutCurve;
};

#endif // HKBHANDIKDRIVERINFO_H
