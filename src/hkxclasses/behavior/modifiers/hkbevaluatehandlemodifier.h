#ifndef HKBEVALUATEHANDLEMODIFIER_H
#define HKBEVALUATEHANDLEMODIFIER_H

#include "hkbmodifier.h"

class hkbEvaluateHandleModifier final: public hkbModifier
{
    friend class EvaluateHandleModifierUI;
public:
    hkbEvaluateHandleModifier(HkxFile *parent, long ref = 0);
    hkbEvaluateHandleModifier& operator=(const hkbEvaluateHandleModifier&) = delete;
    hkbEvaluateHandleModifier(const hkbEvaluateHandleModifier &) = delete;
    ~hkbEvaluateHandleModifier();
public:
    QString getName() const;
    static const QString getClassname();
private:
    bool readData(const HkxXmlReader & reader, long & index);
    bool link();
    void unlink();
    QString evaluateDataValidity();
    bool write(HkxXMLWriter *writer);
private:
    static uint refCount;
    static const QString classname;
    static const QStringList HandleChangeMode;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr handle;
    hkQuadVariable handlePositionOut;
    hkQuadVariable handleRotationOut;
    bool isValidOut;
    qreal extrapolationTimeStep;
    qreal handleChangeSpeed;
    QString handleChangeMode;
    mutable std::mutex mutex;
};

#endif // HKBEVALUATEHANDLEMODIFIER_H
