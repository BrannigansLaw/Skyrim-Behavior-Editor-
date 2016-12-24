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
public:
    CharacterFile(MainWindow *window, const QString & name);
    virtual ~CharacterFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    HkxObjectExpSharedPtr * findCharacterData(long ref);
    HkxObjectExpSharedPtr * findCharacterPropertyValues(long ref);
    hkbCharacterData * getCharacterData() const;
    QString getRootBehaviorPath() const;
    QString getRigName() const;
    QStringList getRigBoneNames() const;
    QString getRagdollName() const;
    QStringList getRagdollBoneNames() const;
    QStringList getAnimationNames() const;
    void setSkeletonFile(SkeletonFile *skel);
protected:
    bool parse();
    bool link();
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
