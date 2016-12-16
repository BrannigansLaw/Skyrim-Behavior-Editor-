#ifndef HKBCHARACTERDATA_H
#define HKBCHARACTERDATA_H

#include <QVector>

#include "src/hkxclasses/hkxobject.h"

class hkbCharacterData: public HkxObject
{
    friend class BehaviorGraphView;
    friend class BehaviorVariablesUI;
    friend class EventsUI;
    friend class CharacterPropertiesUI;
public:
    hkbCharacterData(HkxFile *parent, long ref = 0);
    virtual ~hkbCharacterData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    QStringList getVariableNames() const;
    bool evaulateDataValidity();
    static QString getClassname();
    hkQuadVariable getQuadVariable(int index, bool *ok) const;
    HkxObject * getVariantVariable(int index) const;
    hkVariableType getVariableTypeAt(int index) const;
    bool write(HkxXMLWriter *writer);
    QStringList getCharacterPropertyNames() const;
private:
    hkbCharacterData& operator=(const hkbCharacterData&);
    hkbCharacterData(const hkbCharacterData &);
private:
    void addVariable(hkVariableType type, const QString & name);
    void addVariable(hkVariableType type);
    void removeVariable(int index);
    void setVariableNameAt(int index, const QString & name);
    void setWordVariableValueAt(int index, int value);
    void setQuadVariableValueAt(int index, hkQuadVariable value);
private:
    static QStringList Type;    //See hkVariableType...
    struct hkVariableInfo
    {
        hkVariableInfo(){}
        struct hkRole
        {
            QString role;
            QString flags;
        };

        hkRole role;
        QString type;
    };

    struct hkCharacterControllerInfo{
        hkCharacterControllerInfo(): capsuleHeight(0), capsuleRadius(0), collisionFilterInfo(0){}
        qreal capsuleHeight;
        qreal capsuleRadius;
        int collisionFilterInfo;
        HkxObjectExpSharedPtr characterControllerCinfo;
    };

    static uint refCount;
    static QString classname;
    hkCharacterControllerInfo characterControllerInfo;
    hkQuadVariable modelUpMS;
    hkQuadVariable modelForwardMS;
    hkQuadVariable modelRightMS;
    QVector <hkVariableInfo> characterPropertyInfos;
    QVector <int> numBonesPerLod;
    HkxObjectExpSharedPtr characterPropertyValues;
    HkxObjectExpSharedPtr footIkDriverInfo;
    HkxObjectExpSharedPtr handIkDriverInfo;
    HkxObjectExpSharedPtr stringData;
    HkxObjectExpSharedPtr mirroredSkeletonInfo;
    qreal scale;
};

#endif // HKBCHARACTERDATA_H
