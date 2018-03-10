#ifndef BEHAVIORFILE_H
#define BEHAVIORFILE_H

#include "src/filetypes/hkxfile.h"

class hkbBehaviorGraph;
class CharacterFile;
class hkbModifier;
class ProjectFile;
class SkyrimClipGeneratoData;
class hkbBehaviorReferenceGenerator;

class BehaviorFile: public HkxFile
{
    friend class hkbBehaviorGraphData;
    friend class BehaviorGraphView;
    friend class hkbBehaviorGraph;
    friend class MainWindow;
    friend class HkDataUI;
    friend class ProjectFile;
public:
    BehaviorFile(MainWindow *window, ProjectFile *projectfile, CharacterFile *characterData, const QString & name);
    virtual ~BehaviorFile();
    HkxSharedPtr * findHkxObject(long ref);
    HkxSharedPtr * findGenerator(long ref);
    HkxSharedPtr * findGeneratorChild(long ref);
    HkxSharedPtr * findModifier(long ref);
    HkxSharedPtr * findBehaviorGraph(long ref);
    QVector<int> removeGeneratorData();
    QVector<int> removeGeneratorChildrenData();
    QVector<int> removeModifierData();
    QVector<int> removeOtherData();
    int getIndexOfGenerator(const HkxSharedPtr & obj) const;
    bool setGeneratorData(HkxSharedPtr & ptrToSet, int index);
    hkbGenerator * getGeneratorDataAt(int index);
    int getIndexOfModifier(const HkxSharedPtr & obj) const;
    bool setModifierData(HkxSharedPtr & ptrToSet, int index);
    hkbModifier * getModifierDataAt(int index);
    QStringList getVariableTypenames() const;
    QStringList getEventNames() const;
    QStringList getVariableNames() const;
    QStringList getGeneratorNames() const;
    QStringList getGeneratorTypeNames() const;
    QStringList getModifierNames() const;
    QStringList getModifierTypeNames() const;
    int getCharacterPropertyIndexFromBehavior(const QString & name) const;
    int getCharacterPropertyIndex(const QString &name) const;
    int findCharacterPropertyIndexFromCharacter(int indexOfBehaviorProperty) const;
    QStringList getCharacterPropertyNames() const;
    QStringList getCharacterPropertyTypenames() const;
    hkVariableType getCharacterPropertyTypeAt(int index) const;
    hkVariableType getVariableTypeAt(int index) const;
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    QStringList getAllReferencedBehaviorFilePaths() const;
    QStringList getRagdollBoneNames() const;
    QStringList getRigBoneNames() const;
    int getNumberOfBones(bool ragdoll = false) const;
    int addCharacterProperty(int index);
    QString getVariableNameAt(int index) const;
    QString getEventNameAt(int index) const;
    QString getCharacterPropertyNameAt(int index, bool fromBehaviorFile) const;
    QStringList getAnimationNames() const;
    QString getAnimationNameAt(int index) const;
    QStringList getLocalFrameNames() const;
    QStringList getAllBehaviorFileNames() const;
    void setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time);
    void setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, int eventid);
    bool isClipGenNameTaken(const QString & name) const;
    bool isClipGenNameAvailable(const QString & name) const;
    bool addClipGenToAnimationData(const QString & name);
    bool removeClipGenFromAnimData(const QString & animationname, const QString &clipname, const QString &variablename = "");
    void setClipNameAnimData(const QString &oldclipname, const QString &newclipname);
    void setAnimationIndexAnimData(int index, const QString &clipGenName);
    void setPlaybackSpeedAnimData(const QString & clipGenName, qreal speed);
    void setCropStartAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
    void setCropEndAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
    void appendClipTriggerToAnimData(const QString & clipGenName);
    void removeClipTriggerToAnimDataAt(const QString & clipGenName, int index);
    QString isEventReferenced(int eventindex) const;
    void updateEventIndices(int index);
    QString isVariableReferenced(int variableindex) const;
    void updateVariableIndices(int index);
    void removeUnreferencedFiles(const hkbBehaviorReferenceGenerator *gentoignore);
    QStringList getReferencedBehaviors(const hkbBehaviorReferenceGenerator *gentoignore) const;
    void removeAllData();
protected:
    bool parse();
    bool link();
private:
    QVector<HkxObject *> merge(BehaviorFile *recessivefile);
    QVector <HkxObject *> merge(QVector <HkxObject *> dominantobjects);
    void generateDefaultCharacterData();
    void generateNewBehavior();
    HkxObject * getBehaviorGraphData() const;
    hkbBehaviorGraph * getBehaviorGraph() const;
    HkxObject * getRootStateMachine() const;
    //TreeGraphicsItem * getRootIcon() const;
    void write();
    void removeBindings(int varIndex);
private:
    ProjectFile *project;
    CharacterFile *character;
    HkxSharedPtr behaviorGraph;
    HkxSharedPtr stringData;
    HkxSharedPtr variableValues;
    HkxSharedPtr graphData;
    QList <HkxSharedPtr> generators;
    QList <HkxSharedPtr> generatorChildren;
    QList <HkxSharedPtr> modifiers;
    QList <HkxSharedPtr> otherTypes;
    QStringList referencedBehaviors;
    long largestRef;
};

#endif // BEHAVIORFILE_H
