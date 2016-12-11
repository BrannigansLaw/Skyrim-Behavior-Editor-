#ifndef CHARACTERFILE_H
#define CHARACTERFILE_H

#include "src/filetypes/hkxfile.h"

class CharacterFile: public HkxFile
{
    friend class hkbCharacterData;
    friend class MainWindow;
public:
    CharacterFile(MainWindow *window, const QString & name);
    virtual ~CharacterFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    HkxObjectExpSharedPtr * findCharacterData(long ref);
    HkxObjectExpSharedPtr * findCharacterPropertyValues(long ref);
    QString getRootBehaviorPath() const;
    QString getRigName() const;
    QString getRagdollName() const;
protected:
    bool parse();
    bool link();
private:
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
