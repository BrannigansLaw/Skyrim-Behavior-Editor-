#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "src/filetypes/hkxfile.h"
#include "src/animData/skyrimanimdata.h"
#include "src/animSetData/skyrimanimsetdata.h"

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
    bool readAnimationData(const QString &filename);
    bool readAnimationSetData(const QString & filename);
    bool removeClipGenFromAnimData(const QString & name);
    bool removeAnimationFromAnimData(const QString & name);
    bool appendClipGeneratorAnimData(const QString & name);
    void setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time);
    void setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, const QString &eventname);
    void setClipNameAnimData(const QString &oldclipname, const QString &newclipname);
    void setAnimationIndexForClipGen(int index, const QString &clipGenName);
    void setPlaybackSpeedAnimData(const QString & clipGenName, qreal speed);
    void setCropStartAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
    void setCropEndAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
protected:
    bool parse();
    bool link();
    //SkyrimAnimData *getAnimData() const;
private:
    HkxSharedPtr * findProjectData(long ref);
    HkxSharedPtr * findProjectStringData(long ref);
private:
    CharacterFile *character;
    QList <BehaviorFile *> behaviorFiles;
    HkxSharedPtr stringData;
    HkxSharedPtr projectData;
    long largestRef;
    SkyrimAnimData *skyrimAnimData;
    SkyrimAnimSetData skyrimAnimSetData;
    int projectIndex;
    QString projectName;
};

#endif // PROJECTFILE_H
