#ifndef HKBATTRIBUTEMODIFIER_H
#define HKBATTRIBUTEMODIFIER_H

#include "hkbmodifier.h"

class hkbAttributeModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbAttributeModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbAttributeModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbAttributeModifier& operator=(const hkbAttributeModifier&);
    hkbAttributeModifier(const hkbAttributeModifier &);
private:
    struct hkAttributeMod{
        hkAttributeMod()
            : attributeIndex(-1),
              attributeValue(0)
        {
            //
        }

        int attributeIndex;
        qreal attributeValue;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    QList <hkAttributeMod> assignments;
};

#endif // HKBATTRIBUTEMODIFIER_H
