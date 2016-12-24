#ifndef HKBFOOTIKDRIVERINFO_H
#define HKBFOOTIKDRIVERINFO_H

#include "src/hkxclasses/hkxobject.h"

class hkbFootIkDriverInfo: public HkxObject
{
    friend class FootIkDriverInfoUI;
public:
    hkbFootIkDriverInfo(HkxFile *parent, long ref = 0);
    virtual ~hkbFootIkDriverInfo();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbFootIkDriverInfo& operator=(const hkbFootIkDriverInfo&);
    hkbFootIkDriverInfo(const hkbFootIkDriverInfo &);
private:
    struct BsFootIkDriverLeg{
        BsFootIkDriverLeg()
            : footPlantedAnkleHeightMS(0),
              footRaisedAnkleHeightMS(0),
              maxAnkleHeightMS(0),
              minAnkleHeightMS(0),
              maxKneeAngleDegrees(0),
              minKneeAngleDegrees(0),
              maxAnkleAngleDegrees(0),
              hipIndex(-1),
              kneeIndex(-1),
              ankleIndex(-1)
        {
            //
        }

        hkQuadVariable kneeAxisLS;
        hkQuadVariable footEndLS;
        qreal footPlantedAnkleHeightMS;
        qreal footRaisedAnkleHeightMS;
        qreal maxAnkleHeightMS;
        qreal minAnkleHeightMS;
        qreal maxKneeAngleDegrees;
        qreal minKneeAngleDegrees;
        qreal maxAnkleAngleDegrees;
        int hipIndex;
        int kneeIndex;
        int ankleIndex;
    };

    static uint refCount;
    static QString classname;
    QList <BsFootIkDriverLeg> legs;
    qreal raycastDistanceUp;
    qreal raycastDistanceDown;
    qreal originalGroundHeightMS;
    qreal verticalOffset;
    int collisionFilterInfo;
    qreal forwardAlignFraction;
    qreal sidewaysAlignFraction;
    qreal sidewaysSampleWidth;
    bool lockFeetWhenPlanted;
    bool useCharacterUpVector;
    bool isQuadrupedNarrow;
};

#endif // HKBFOOTIKDRIVERINFO_H
