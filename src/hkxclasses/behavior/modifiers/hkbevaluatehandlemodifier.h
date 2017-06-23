#ifndef HKBEVALUATEHANDLEMODIFIER_H
#define HKBEVALUATEHANDLEMODIFIER_H

#include "hkbmodifier.h"

class hkbEvaluateHandleModifier: public hkbModifier
{
    friend class BehaviorGraphView;
    friend class EvaluateHandleModifierUI;
public:
    hkbEvaluateHandleModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbEvaluateHandleModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbEvaluateHandleModifier& operator=(const hkbEvaluateHandleModifier&);
    hkbEvaluateHandleModifier(const hkbEvaluateHandleModifier &);
private:
    static QStringList HandleChangeMode;    //HandleChangeMode (HANDLE_CHANGE_MODE_ABRUPT=0;HANDLE_CHANGE_MODE_CONSTANT_VELOCITY=1)
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    HkxSharedPtr handle;   //Always NULL???
    hkQuadVariable handlePositionOut;
    hkQuadVariable handleRotationOut;
    bool isValidOut;
    qreal extrapolationTimeStep;
    qreal handleChangeSpeed;
    QString handleChangeMode;
};

#endif // HKBEVALUATEHANDLEMODIFIER_H
