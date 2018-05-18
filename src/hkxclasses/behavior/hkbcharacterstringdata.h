#ifndef HKBCHARACTERSTRINGDATA_H
#define HKBCHARACTERSTRINGDATA_H

#include <QStringList>

#include "src/hkxclasses/hkxobject.h"

class HkxFile;

class hkbCharacterStringData: public HkxObject
{
    friend class BehaviorGraphView;
    friend class hkbCharacterData;
    friend class CharacterFile;
    friend class CharacterPropertiesUI;
    friend class BehaviorFile;
    friend class AnimationsUI;
public:
    hkbCharacterStringData(HkxFile *parent, long ref = 0);
    virtual ~hkbCharacterStringData();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    QStringList getAnimationNames() const;
    QString getCharacterPropertyNameAt(int index) const;
    void addAnimation(const QString &name = "");
    QString getAnimationNameAt(int index) const;
    int getAnimationIndex(const QString & name) const;
    bool merge(hkbCharacterStringData *obj);
private:
    hkbCharacterStringData& operator=(const hkbCharacterStringData&);
    hkbCharacterStringData(const hkbCharacterStringData &);
private:
    static uint refCount;
    static QString classname;
    QStringList deformableSkinNames;
    QStringList rigidSkinNames;
    QStringList animationNames;
    QStringList animationFilenames;
    QStringList characterPropertyNames;
    QStringList retargetingSkeletonMapperFilenames;
    QStringList lodNames;
    QStringList mirroredSyncPointSubstringsA;
    QStringList mirroredSyncPointSubstringsB;
    QString name;
    QString rigName;
    QString ragdollName;
    QString behaviorFilename;
};

#endif // HKBCHARACTERSTRINGDATA_H
