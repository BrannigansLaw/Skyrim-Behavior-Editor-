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

class ProjectUI: public QGroupBox
{
    Q_OBJECT
public:
    ProjectUI(ProjectFile *file);
    virtual ~ProjectUI();
    void setProject(ProjectFile *file);
    void loadData();
    void setFilePath(const QString & path);
signals:
    void openFile(const QModelIndex & index);
protected:
private:
    ProjectFile *project;
    QGridLayout *lyt;
    CharacterPropertiesUI *characterProperties;
    //AnimationsUI *animations;
    SkeletonUI *skeleton;
    FootIkDriverInfoUI *footIK;
    //HandIKDriverUI *handIK;
    QFileSystemModel *fileSys;
    QListView *fileView;
    QString lastFileSelectedPath;
};

#endif // PROJECTUI_H
