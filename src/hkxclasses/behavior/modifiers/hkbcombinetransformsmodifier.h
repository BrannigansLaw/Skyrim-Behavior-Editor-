#ifndef HKBCOMBINETRANSFORMSMODIFIER_H
#define HKBCOMBINETRANSFORMSMODIFIER_H

#include "hkbmodifier.h"

class hkbCombineTransformsModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbCombineTransformsModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbCombineTransformsModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbCombineTransformsModifier& operator=(const hkbCombineTransformsModifier&);
    hkbCombineTransformsModifier(const hkbCombineTransformsModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkQuadVariable translationOut;
    hkQuadVariable rotationOut;
    hkQuadVariable leftTranslation;
    hkQuadVariable leftRotation;
    hkQuadVariable rightTranslation;
    hkQuadVariable rightRotation;
    bool invertLeftTransform;
    bool invertRightTransform;
    bool invertResult;
};

#endif // HKBCOMBINETRANSFORMSMODIFIER_H
