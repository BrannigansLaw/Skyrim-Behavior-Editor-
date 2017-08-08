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
    friend class BehaviorFile;
public:
    CharacterFile(MainWindow *window, const QString & name);
    virtual ~CharacterFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    QString getRootObjectReferenceString();
    //QString getRelativeRootBehaviorPath() const;
    QString getBehaviorDirectoryName() const;
    QString getRigName() const;
    QStringList getRigBoneNames() const;
    int getNumberOfBones(bool ragdoll = false) const;
    QString getRagdollName() const;
    QStringList getRagdollBoneNames() const;
    QStringList getAnimationNames() const;
    QString getCharacterPropertyNameAt(int index) const;
    int getCharacterPropertyIndex(const QString & name) const;
    QStringList getCharacterPropertyNames() const;
    QStringList getCharacterPropertyTypenames() const;
    hkVariableType getCharacterPropertyTypeAt(int index) const;
protected:
    bool parse();
    bool link();
private:
    void write();
    void setSkeletonFile(SkeletonFile *skel);
    HkxSharedPtr * findCharacterData(long ref);
    HkxSharedPtr * findCharacterPropertyValues(long ref);
    hkbCharacterData * getCharacterData() const;
    void addFootIK();
    void addHandIK();
    void disableFootIK();
    void disableHandIK();
private:
    SkeletonFile *skeleton;
    HkxSharedPtr characterData;
    HkxSharedPtr characterPropertyValues;
    HkxSharedPtr footIkDriverInfo;
    HkxSharedPtr handIkDriverInfo;
    HkxSharedPtr stringData;
    HkxSharedPtr mirroredSkeletonInfo;
    QList <HkxSharedPtr> boneWeightArrays;
    long largestRef;
};

#endif // CHARACTERFILE_H
