#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "src/filetypes/hkxfile.h"

class CharacterFile;

class ProjectFile: public HkxFile
{
    friend class MainWindow;
    friend class ProjectUI;
    friend class hkbProjectData;
    friend class hkRootLevelContainer;
public:
    ProjectFile(MainWindow *window, const QString & name);
    virtual ~ProjectFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    QString getCharacterFilePathAt(int index) const;
    void setCharacterFile(CharacterFile *file);
protected:
    bool parse();
    bool link();
private:
    HkxObjectExpSharedPtr * findProjectData(long ref);
    HkxObjectExpSharedPtr * findProjectStringData(long ref);
private:
    CharacterFile *character;
    HkxObjectExpSharedPtr stringData;
    HkxObjectExpSharedPtr projectData;
    long largestRef;
};

#endif // PROJECTFILE_H
