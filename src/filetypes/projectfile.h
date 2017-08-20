#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "src/filetypes/hkxfile.h"

class CharacterFile;
class BehaviorFile;

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
    //QString getRootObjectReferenceString();
    QString getCharacterFilePathAt(int index) const;
    void setCharacterFile(CharacterFile *file);
    bool isClipGenNameTaken(const QString & name) const;
protected:
    bool parse();
    bool link();
private:
    HkxSharedPtr * findProjectData(long ref);
    HkxSharedPtr * findProjectStringData(long ref);
private:
    CharacterFile *character;
    QList <BehaviorFile *> behaviorFiles;
    HkxSharedPtr stringData;
    HkxSharedPtr projectData;
    long largestRef;
};

#endif // PROJECTFILE_H
