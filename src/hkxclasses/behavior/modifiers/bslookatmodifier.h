#ifndef BSLOOKATMODIFIER_H
#define BSLOOKATMODIFIER_H

#include "hkbmodifier.h"

class BSLookAtModifier final: public hkbModifier
{
    friend class BSBoneUI;
    friend class BSLookAtModifierUI;
public:
    BSLookAtModifier(HkxFile *parent, long ref = 0);
    BSLookAtModifier& operator=(const BSLookAtModifier&) = delete;
    BSLookAtModifier(const BSLookAtModifier &) = delete;
    ~BSLookAtModifier();
    static const QString getClassname();
    QString getName() const;
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
    int getNumberOfBones() const;
    int getNumberOfEyeBones() const;
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
    QVector <HkxObject *> getChildrenOtherTypes() const;
    bool merge(HkxObject *recessiveObject);
private:
    struct BsBone{
        BsBone(): index(-1), limitAngleDegrees(0), onGain(0), offGain(0), enabled(true){}
        int index;
        hkQuadVariable fwdAxisLS;
        qreal limitAngleDegrees;
        qreal onGain;
        qreal offGain;
        bool enabled;
    };
private:
    static uint refCount;
    static const QString classname;
    long userData;
    QString name;
    bool enable;
    bool lookAtTarget;
    QVector <BsBone> bones;
    QVector <BsBone> eyeBones;
    qreal limitAngleDegrees;
    qreal limitAngleThresholdDegrees;
    bool continueLookOutsideOfLimit;
    qreal onGain;
    qreal offGain;
    bool useBoneGains;
    hkQuadVariable targetLocation;
    bool targetOutsideLimits;
    int id;
    HkxSharedPtr payload;
    bool lookAtCamera;
    qreal lookAtCameraX;
    qreal lookAtCameraY;
    qreal lookAtCameraZ;
    mutable std::mutex mutex;
};

#endif // BSLOOKATMODIFIER_H
