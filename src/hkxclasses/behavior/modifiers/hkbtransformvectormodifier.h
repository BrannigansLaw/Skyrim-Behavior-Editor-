#ifndef HKBTRANSFORMVECTORMODIFIER_H
#define HKBTRANSFORMVECTORMODIFIER_H

#include "hkbmodifier.h"

class hkbTransformVectorModifier final: public hkbModifier
{
    friend class TransformVectorModifierUI;
public:
    hkbTransformVectorModifier(HkxFile *parent, long ref = 0);
    hkbTransformVectorModifier& operator=(const hkbTransformVectorModifier&) = delete;
    hkbTransformVectorModifier(const hkbTransformVectorModifier &) = delete;
    ~hkbTransformVectorModifier();
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
    hkQuadVariable rotation;
    hkQuadVariable translation;
    hkQuadVariable vectorIn;
    hkQuadVariable vectorOut;
    bool rotateOnly;
    bool inverse;
    bool computeOnActivate;
    bool computeOnModify;
    mutable std::mutex mutex;
};

#endif // HKBTRANSFORMVECTORMODIFIER_H
