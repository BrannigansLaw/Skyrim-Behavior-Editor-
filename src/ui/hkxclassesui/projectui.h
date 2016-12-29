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
class QCheckBox;

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
private slots:
    void toggleFootIK(bool toggle);
    void toggleHandIK(bool toggle);
private:
    ProjectFile *project;
    QGridLayout *lyt;
    CharacterPropertiesUI *characterProperties;
    //AnimationsUI *animations;
    SkeletonUI *skeleton;
    QCheckBox *enableFootIKCB;
    QCheckBox *enableHandIKCB;
    FootIkDriverInfoUI *footIK;
    HandIkDriverInfoUI *handIK;
    QFileSystemModel *fileSys;
    QListView *fileView;
    QString lastFileSelectedPath;
};

#endif // PROJECTUI_H
