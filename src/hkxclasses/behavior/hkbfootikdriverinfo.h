#ifndef HKBFOOTIKDRIVERINFO_H
#define HKBFOOTIKDRIVERINFO_H

#include "src/hkxclasses/hkxobject.h"

class hkbFootIkDriverInfo: public HkxObject
{
    friend class FootIkDriverInfoUI;
    friend class FootIkDriverInfoLegUI;
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
    void addLeg();
    void removeLegAt(int index);
    hkbFootIkDriverInfo& operator=(const hkbFootIkDriverInfo&);
    hkbFootIkDriverInfo(const hkbFootIkDriverInfo &);
private:
    struct hkbFootIkDriverInfoLeg{
        hkbFootIkDriverInfoLeg(hkbFootIkDriverInfo *par = nullptr)
            : parent(par),
              footPlantedAnkleHeightMS(0),
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

        hkbFootIkDriverInfo *parent;
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

    hkbFootIkDriverInfoLeg * getLegAt(int index);

    static uint refCount;
    static QString classname;
    QList <hkbFootIkDriverInfoLeg> legs;
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
