#ifndef HKBCOMBINETRANSFORMSMODIFIER_H
#define HKBCOMBINETRANSFORMSMODIFIER_H

#include "hkbmodifier.h"

class hkbCombineTransformsModifier final: public hkbModifier
{
    friend class CombineTransformsModifierUI;
public:
    hkbCombineTransformsModifier(HkxFile *parent, long ref = 0);
    hkbCombineTransformsModifier& operator=(const hkbCombineTransformsModifier&) = delete;
    hkbCombineTransformsModifier(const hkbCombineTransformsModifier &) = delete;
    ~hkbCombineTransformsModifier();
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
    mutable std::mutex mutex;
};

#endif // HKBCOMBINETRANSFORMSMODIFIER_H
