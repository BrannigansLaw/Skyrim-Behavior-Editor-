#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "src/filetypes/hkxfile.h"

class CharacterFile;

class ProjectFile: public HkxFile
{
    friend class hkbProjectData;
    friend class MainWindow;
    friend class ProjectUI;
public:
    ProjectFile(MainWindow *window, const QString & name);
    virtual ~ProjectFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    HkxObjectExpSharedPtr * findProjectData(long ref);
    HkxObjectExpSharedPtr * findProjectStringData(long ref);
    QString getCharacterFilePathAt(int index) const;
    void setCharacterFile(CharacterFile *file);
protected:
    bool parse();
    bool link();
private:
    CharacterFile *character;
    HkxObjectExpSharedPtr stringData;
    HkxObjectExpSharedPtr projectData;
    long largestRef;
};

#endif // PROJECTFILE_H
