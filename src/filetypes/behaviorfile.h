#ifndef BEHAVIORFILE_H
#define BEHAVIORFILE_H

#include <mutex>
#include <condition_variable>

#include "src/filetypes/hkxfile.h"

class hkbBehaviorGraph;
class CharacterFile;
class hkbModifier;
class ProjectFile;
class SkyrimClipGeneratoData;
class hkbBehaviorReferenceGenerator;
class hkbBoneWeightArray;
class hkbStateMachine;
class DataIconManager;

class BehaviorFile final: public HkxFile
{
    friend class ProjectFile;
    friend class MainWindow;
public:
    BehaviorFile(MainWindow *window, ProjectFile *projectfile, CharacterFile *characterData, const QString & name);
    BehaviorFile& operator=(const BehaviorFile&) = delete;
    BehaviorFile(const BehaviorFile &) = delete;
    ~BehaviorFile();
public:
    bool doesBehaviorExist(const QString &behaviorname) const;
    int getIndexOfGenerator(const HkxSharedPtr & obj) const;
    int getIndexOfModifier(const HkxSharedPtr & obj) const;
    QStringList getVariableTypenames() const;
    QStringList getEventNames() const;
    int getNumberOfEvents() const;
    QStringList getVariableNames() const;
    int getNumberOfVariables() const;
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
    QStringList getAllReferencedBehaviorFilePaths() const;
    QStringList getRagdollBoneNames() const;
    QStringList getRigBoneNames() const;
    int getNumberOfBones(bool ragdoll = false) const;
    QString getVariableNameAt(int index) const;
    QString getEventNameAt(int index) const;
    QString getCharacterPropertyNameAt(int index, bool fromBehaviorFile) const;
    QStringList getAnimationNames() const;
    QString getAnimationNameAt(int index) const;
    QStringList getLocalFrameNames() const;
    QStringList getAllBehaviorFileNames() const;
    bool isClipGenNameTaken(const QString & name) const;
    bool isClipGenNameAvailable(const QString & name) const;
    QString isEventReferenced(int eventindex) const;
    QStringList getReferencedBehaviors(const hkbBehaviorReferenceGenerator *gentoignore) const;
    qreal getAnimationDurationFromAnimData(const QString & animationname) const;
    QString isVariableReferenced(int variableindex) const;
    HkxSharedPtr *findHkxObject(long ref);
    HkxSharedPtr * findGenerator(long ref);
    HkxSharedPtr * findModifier(long ref);
    HkxSharedPtr * findBehaviorGraph(long ref);
    void mergeEventIndices(int oldindex, int newindex);
    void mergeVariableIndices(int oldindex, int newindex);
    HkxSharedPtr getVariableValues() const;
    HkxSharedPtr getStringData() const;
    HkxSharedPtr getGraphData() const;
    HkxObject * getBehaviorGraphData() const;
    bool isNameUniqueInProject(HkxObject *object) const;
    bool existsInBehavior(HkDynamicObject *object, int startindex = 0) const;
    hkbStateMachine * findRootStateMachineFromBehavior(const QString behaviorname) const;
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    hkbGenerator * getGeneratorDataAt(int index);
    hkbModifier * getModifierDataAt(int index);
    QVector<int> removeGeneratorData();
    QVector<int> removeModifierData();
    QVector<int> removeOtherData();
    int addCharacterProperty(int index);
    void updateEventIndices(int index);
    void updateVariableIndices(int index);
    void setFocusGeneratorIcon(int index);
    void setFocusModifierIcon(int index);
    hkbBehaviorGraph * getBehaviorGraph() const;
private:
    void removeUnreferencedFiles(const hkbBehaviorReferenceGenerator *gentoignore);
    bool removeClipGenFromAnimData(const QString & animationname, const QString &clipname, const QString &variablename = "");
    void setClipNameAnimData(const QString &oldclipname, const QString &newclipname);
    void setAnimationIndexAnimData(int index, const QString &clipGenName);
    void setPlaybackSpeedAnimData(const QString & clipGenName, qreal speed);
    void setCropStartAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
    void setCropEndAmountLocalTimeAnimData(const QString & clipGenName, qreal time);
    void appendClipTriggerToAnimData(const QString & clipGenName);
    void removeClipTriggerToAnimDataAt(const QString & clipGenName, int index);
    bool addClipGenToAnimationData(const QString & name);
    void setLocalTimeForClipGenAnimData(const QString &clipname, int triggerindex, qreal time);
    void setEventNameForClipGenAnimData(const QString &clipname, int triggerindex, int eventid);
    void removeAllData();
    bool setGeneratorData(HkxSharedPtr & ptrToSet, int index);
    bool setModifierData(HkxSharedPtr & ptrToSet, int index);
    QStringList getErrors() const;
    void getCharacterPropertyBoneWeightArray(const QString &name, hkbBoneWeightArray *ptrtosetdata) const;
    bool parse();
    void write();
    void generateDefaultCharacterData();
    void generateNewBehavior();
    void mergedWrite();
    QVector<DataIconManager *> merge(BehaviorFile *recessivefile, int &taskCount, std::mutex &mutex, std::condition_variable &conditionVar);
    QVector<DataIconManager *> merge(BehaviorFile *recessivefile);
protected:
    bool link();
private:
    QVector<int> removeDataNoLock(QVector<HkxSharedPtr> &objects);
    QStringList getRefedAnimations() const;
    QString detectErrorsMT(int &taskcount, std::mutex &mutex, std::condition_variable &conditionVar);
    QString detectErrors();
    void mergeObjects(QVector<DataIconManager *> &recessiveobjects);
    hkbStateMachine *getRootStateMachine() const;
    void removeBindings(int varIndex);
    bool checkForDuplicateReferencesNumbersNoLock() const;
private:
    ProjectFile *project;
    CharacterFile *character;
    HkxSharedPtr behaviorGraph;
    HkxSharedPtr stringData;
    HkxSharedPtr variableValues;
    HkxSharedPtr graphData;
    QVector <HkxSharedPtr> generators;
    QVector <HkxSharedPtr> modifiers;
    QVector <HkxSharedPtr> otherTypes;
    QStringList referencedBehaviors;
    long largestRef;
    QStringList errorList;
    mutable std::mutex mutex;
};

#endif // BEHAVIORFILE_H
