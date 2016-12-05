#ifndef HKBTRANSFORMVECTORMODIFIER_H
#define HKBTRANSFORMVECTORMODIFIER_H

#include "hkbmodifier.h"

class hkbTransformVectorModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbTransformVectorModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbTransformVectorModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbTransformVectorModifier& operator=(const hkbTransformVectorModifier&);
    hkbTransformVectorModifier(const hkbTransformVectorModifier &);
private:
    static uint refCount;
    static QString classname;
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
};

#endif // HKBTRANSFORMVECTORMODIFIER_H
