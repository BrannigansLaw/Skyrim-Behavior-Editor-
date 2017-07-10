#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QtWidgets>

#include "src/hkxclasses/hkxobject.h"

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

class PlainTextEdit: public QPlainTextEdit
{
    Q_OBJECT
public:
    PlainTextEdit(QWidget* parent = 0)
        : QPlainTextEdit(parent)
    {
        //
    }

    QSize sizeHint() const{
        return QSize(120, 40);
    }
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    static QMessageBox::StandardButton yesNoDialogue(const QString & message);
    MainWindow();
    virtual ~MainWindow();
    void writeToLog(const QString & message, bool isError = false);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
private:
    PlainTextEdit *debugLog;
    QString hkxcmdPath;
    QString skyrimDirectory;
    QGridLayout *topLyt;
    QMenuBar *topMB;
    QMenu *fileM;
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
    QTabWidget *tabs;
    ProjectFile *projectFile;
    CharacterFile *characterFile;
    SkeletonFile *skeletonFile;
    QList <BehaviorFile *> behaviorFiles;
    QList <BehaviorGraphView *> behaviorGraphs;
    ProjectUI *projectUI;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    HkDataUI *objectDataWid;
    QScrollArea *objectDataSA;
    BehaviorVariablesUI *variablesWid;
    EventsUI *eventsWid;
    QGroupBox *logGB;
    QVBoxLayout *logGBLyt;
    QString lastFileSelected;
    QString lastFileSelectedPath;
private slots:
    void openPackedProject();
    void openUnpackedProject();
    void openBehaviorFile(const QModelIndex & index);
    void expandBranches();
    void collapseBranches();
    void refocus();
    void save();
    void saveProject();
    void packAndExportProjectToSkyrimDirectory();
    void packAndExportFileToSkyrimDirectory();
    //void saveAs();
    void exit();
    void changedTabs(int index);
    void closeTab(int index);
private:
    enum HKXCMD_RETURN{
        HKXCMD_SUCCESS = 0
    };
    void openProject(QString &filepath);
    void saveFile(int index);
    bool openBehavior(const QString & filename);
    bool exitProgram();
    bool findSkyrimDirectory();
    MainWindow::HKXCMD_RETURN hkxcmd(const QString &filepath, const QString &outputDirectory, const QString &flags = "-f SAVE_CONCISE");
    //void drawIcons();
    void readSettings();
    void writeSettings();
    QMessageBox::StandardButton closeAllDialogue();
    QMessageBox::StandardButton closeFileDialogue();
    bool closeAll();
};

#endif // MAINWINDOW_H
