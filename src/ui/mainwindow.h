#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QtWidgets>

#include "src/hkxclasses/hkxobject.h"

#include <thread>
#include <mutex>
#include <condition_variable>

//class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorGraphView;
class QPlainTextEdit;
class HkDataUI;
class BehaviorVariablesUI;
class EventsUI;
class ProjectFile;
class BehaviorFile;
class CharacterFile;
class ProjectFile;
class SkeletonFile;
class ProjectUI;
class AnimationCacheUI;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    static QMessageBox::StandardButton yesNoDialogue(const QString & message);
    MainWindow();
    virtual ~MainWindow();
    void removeBehaviorGraphs(const QStringList & filenames);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
private:
    QString hkxcmdPath;
    QString skyrimDirectory;
    QString skyrimSpecialEdtionDirectory;
    QString skyrimBehaviorUpdateToolFullPath;
    QGridLayout *topLyt;
    QMenuBar *topMB;
    QMenu *fileM;
    QAction *newProjectA;
    QAction *openPackedProjectA;
    QAction *openUnpackedProjectA;
    QAction *saveA;
    QAction *exportToSkyrimDirA;
    QAction *exportCurrentFileA;
    QAction *saveProjectA;
    QAction *exitA;
    QMenu *viewM;
    QAction *expandA;
    QAction *collapseA;
    QAction *refocusA;
    QAction *viewAnimationCacheA;
    QMenu *mergeM;
    QAction *mergeBehaviorsA;
    QAction *mergeProjectsA;
    QMenu *settingsM;
    QAction *setPathToGameFolderA;
    QAction *setGameModeA;
    QTabWidget *tabs;
    ProjectFile *projectFile;
    CharacterFile *characterFile;
    SkeletonFile *skeletonFile;
    QList <BehaviorGraphView *> behaviorGraphs;
    ProjectUI *projectUI;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    HkDataUI *objectDataWid;
    QScrollArea *objectDataSA;
    BehaviorVariablesUI *variablesWid;
    EventsUI *eventsWid;
    QString lastFileSelected;
    QString lastFileSelectedPath;
    std::mutex mutex;
    std::condition_variable conditionVar;
    AnimationCacheUI *animationCacheUI;
private slots:
    void createNewProject();
    void openPackedProject();
    void openUnpackedProject();
    void openBehaviorFile(const QModelIndex & index);
    void openAnimationFile(const QString &animationname);
    void removeAnimation(int index);
    void expandBranches();
    void collapseBranches();
    void refocus();
    void setGameMode();
    void setPathToGameDirectory();
    void save();
    void viewAnimationCache();
    void saveProject();
    void packAndExportProjectToSkyrimDirectory();
    void exportAnimationData() ;
    void packAndExportFileToSkyrimDirectory();
    void mergeBehaviors();
    void mergeProjects();
    void exit();
    void changedTabs(int index);
    void closeTab(int index);
    void addNewBehavior(bool initData);
private:
    enum HKXCMD_RETURN{
        HKXCMD_SUCCESS = 0
    };
    QString generateUniqueBehaviorName();
    BehaviorFile *openBehaviorForMerger(QString & filepath);
    void openProject(QString &filepath, bool loadui = true, bool loadanimdata = true);
    void saveFile(int index, int &taskCount);
    bool openBehavior(const QString & filename, int &taskCount, bool checkisopen = true);
    bool exitProgram();
    bool findGameDirectory(const QString &gamename, QString &gamedirectory);
    void convertProject(const QString &filepath, const QString &newpath = "", const QString &flags = "-v:xml");
    HKXCMD_RETURN hkxcmd(const QString &filepath, const QString &outputDirectory, int &taskcount, const QString &flags = "-f SAVE_CONCISE");
    int getBehaviorGraphIndex(const QString & filename);
    void readSettings();
    void writeSettings();
    QMessageBox::StandardButton closeAllDialogue();
    QMessageBox::StandardButton closeFileDialogue();
    bool closeAll();
};

#endif // MAINWINDOW_H
