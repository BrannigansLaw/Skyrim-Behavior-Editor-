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
    friend class CharacterFile;
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
    QStringList getCharacterPropertyTypenames() const;
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
            hkRole(): role("ROLE_DEFAULT"), flags("0"){}
            QString role;
            QString flags;
        };

        hkRole role;
        QString type;
    };

public:
    hkVariableType getCharacterPropertyTypeAt(int index) const;

private:
    struct hkCharacterControllerInfo{
        hkCharacterControllerInfo(): capsuleHeight(0), capsuleRadius(0), collisionFilterInfo(0){}
        qreal capsuleHeight;
        qreal capsuleRadius;
        int collisionFilterInfo;
        HkxSharedPtr characterControllerCinfo;
    };

    static uint refCount;
    static QString classname;
    hkCharacterControllerInfo characterControllerInfo;
    hkQuadVariable modelUpMS;
    hkQuadVariable modelForwardMS;
    hkQuadVariable modelRightMS;
    QVector <hkVariableInfo> characterPropertyInfos;
    QVector <int> numBonesPerLod;
    HkxSharedPtr characterPropertyValues;
    HkxSharedPtr footIkDriverInfo;
    HkxSharedPtr handIkDriverInfo;
    HkxSharedPtr stringData;
    HkxSharedPtr mirroredSkeletonInfo;
    qreal scale;
};

#endif // HKBCHARACTERDATA_H
