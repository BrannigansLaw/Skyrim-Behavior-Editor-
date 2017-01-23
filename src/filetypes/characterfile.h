#ifndef CHARACTERFILE_H
#define CHARACTERFILE_H

#include "src/filetypes/hkxfile.h"

class SkeletonFile;
class hkbCharacterData;

class CharacterFile: public HkxFile
{
    friend class hkbCharacterData;
    friend class MainWindow;
    friend class ProjectUI;
    friend class hkbVariableValueSet;
    friend class hkRootLevelContainer;
public:
    CharacterFile(MainWindow *window, const QString & name);
    virtual ~CharacterFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    QString getRootObjectReferenceString();
    QString getRootBehaviorPath() const;
    QString getRigName() const;
    QStringList getRigBoneNames() const;
    QString getRagdollName() const;
    QStringList getRagdollBoneNames() const;
    QStringList getAnimationNames() const;
    QString getCharacterPropertyNameAt(int index) const;
    QStringList getCharacterPropertyNames() const;
    QStringList getCharacterPropertyTypenames() const;
    hkVariableType getCharacterPropertyTypeAt(int index) const;
protected:
    bool parse();
    bool link();
private:
    void write();
    void setSkeletonFile(SkeletonFile *skel);
    HkxObjectExpSharedPtr * findCharacterData(long ref);
    HkxObjectExpSharedPtr * findCharacterPropertyValues(long ref);
    hkbCharacterData * getCharacterData() const;
    void addFootIK();
    void addHandIK();
    void disableFootIK();
    void disableHandIK();
private:
    SkeletonFile *skeleton;
    HkxObjectExpSharedPtr characterData;
    HkxObjectExpSharedPtr characterPropertyValues;
    HkxObjectExpSharedPtr footIkDriverInfo;
    HkxObjectExpSharedPtr handIkDriverInfo;
    HkxObjectExpSharedPtr stringData;
    HkxObjectExpSharedPtr mirroredSkeletonInfo;
    QList <HkxObjectExpSharedPtr> boneWeightArrays;
    long largestRef;
};

#endif // CHARACTERFILE_H
