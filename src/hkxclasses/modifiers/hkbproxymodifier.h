#ifndef HKBPROXYMODIFIER_H
#define HKBPROXYMODIFIER_H

#include "hkbmodifier.h"

class hkbProxyModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbProxyModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbProxyModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbProxyModifier& operator=(const hkbProxyModifier&);
    hkbProxyModifier(const hkbProxyModifier &);
private:
    struct hkProxyInfo{
        hkProxyInfo()
            : dynamicFriction(0),
              staticFriction(0),
              keepContactTolerance(0),
              keepDistance(0),
              contactAngleSensitivity(0),
              userPlanes(0),
              maxCharacterSpeedForSolver(0),
              characterStrength(0),
              characterMass(0),
              maxSlope(0),
              penetrationRecoverySpeed(0),
              maxCastIterations(0),
              refreshManifoldInCheckSupport(false)
        {
            //
        }

        qreal dynamicFriction;
        qreal staticFriction;
        qreal keepContactTolerance;
        hkQuadVariable up;
        qreal keepDistance;
        qreal contactAngleSensitivity;
        int userPlanes;
        qreal maxCharacterSpeedForSolver;
        qreal characterStrength;
        qreal characterMass;
        qreal maxSlope;
        qreal penetrationRecoverySpeed;
        int maxCastIterations;
        bool refreshManifoldInCheckSupport;
    };

    static QStringList PhantomType;  //PhantomType (PHANTOM_TYPE_SIMPLE=0;PHANTOM_TYPE_CACHING=1)
    static QStringList LinearVelocityMode;   //LinearVelocityMode (LINEAR_VELOCITY_MODE_WORLD=0;LINEAR_VELOCITY_MODE_MODEL=1)
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkProxyInfo proxyInfo;
    hkQuadVariable linearVelocity;
    qreal horizontalGain;
    qreal verticalGain;
    qreal maxHorizontalSeparation;
    qreal limitHeadingDegrees;
    qreal maxVerticalSeparation;
    qreal verticalDisplacementError;
    qreal verticalDisplacementErrorGain;
    qreal maxVerticalDisplacement;
    qreal minVerticalDisplacement;
    qreal capsuleHeight;
    qreal capsuleRadius;
    qreal maxSlopeForRotation;
    int collisionFilterInfo;
    QString phantomType;
    QString linearVelocityMode;
    bool ignoreIncomingRotation;
    bool ignoreCollisionDuringRotation;
    bool ignoreIncomingTranslation;
    bool includeDownwardMomentum;
    bool followWorldFromModel;
    bool isTouchingGround;
};

#endif // HKBPROXYMODIFIER_H
