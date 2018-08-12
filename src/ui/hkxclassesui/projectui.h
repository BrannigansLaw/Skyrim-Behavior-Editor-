#ifndef PROJECTUI_H
#define PROJECTUI_H

#include <QGroupBox>

class QGridLayout;
class CharacterPropertiesUI;
class SkeletonUI;
class ProjectFile;
class QFileSystemModel;
class QListView;
class FootIkDriverInfoUI;
class HandIkDriverInfoUI;
class CheckBox;
class AnimationsUI;
class QPushButton;
class AnimationCacheUI;

class ProjectUI: public QGroupBox
{
    Q_OBJECT
    //friend class MainWindow;
public:
    ProjectUI(ProjectFile *file);
    virtual ~ProjectUI();
    void setProject(ProjectFile *file);
    void loadData();
    void setFilePath(const QString & path);
    void setDisabled(bool disable);
    AnimationsUI *getAnimations() const;
signals:
    void openFile(const QModelIndex & index);
    void addBehavior(bool initData);
    void openAnimation(const QString & filename);
    void animationRemoved(int index);
protected:
private slots:
    void toggleFootIK(bool toggle);
    void toggleHandIK(bool toggle);
    void addNewBehaviorFile();
private:
    ProjectFile *project;
    QGridLayout *lyt;
    AnimationCacheUI *animationCacheUI;
    CharacterPropertiesUI *characterProperties;
    AnimationsUI *animations;
    SkeletonUI *skeleton;
    CheckBox *enableFootIKCB;
    CheckBox *enableHandIKCB;
    FootIkDriverInfoUI *footIK;
    HandIkDriverInfoUI *handIK;
    QFileSystemModel *fileSys;
    QListView *fileView;
    QPushButton *addBehaviorFile;
    CheckBox *initializeWithCharacterData;
    QString lastFileSelectedPath;
};

#endif // PROJECTUI_H
