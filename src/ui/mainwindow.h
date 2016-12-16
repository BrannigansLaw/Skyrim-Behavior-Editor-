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
class CharacterPropertiesUI;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    void writeToLog(const QString & message, bool isError = false);
    void setProgressData(const QString & message, int value);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    QPlainTextEdit *debugLog;
    QGridLayout *topLyt;
    QMenuBar *topMB;
    QAction *openProjectA;
    //QAction *openBehaviorA;
    QMenu *fileM;
    QAction *saveA;
    QMenu *viewM;
    QAction *expandA;
    QAction *collapseA;
    QTabWidget *tabs;
    ProjectFile *projectFile;
    CharacterFile *characterFile;
    SkeletonFile *skeletonFile;
    QList <BehaviorFile *> behaviorFiles;
    QList <BehaviorGraphView *> behaviorGraphs;
    CharacterPropertiesUI *characterPropertiesWid;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    HkDataUI *objectDataWid;
    QScrollArea *objectDataSA;
    BehaviorVariablesUI *variablesWid;
    EventsUI *eventsWid;
    QGroupBox *logGB;
    QVBoxLayout *logGBLyt;
    QProgressDialog *progressD;
    QString lastFileSelected;
private slots:
    void openProject();
    //void openBehaviorFile();
    void expandBranches();
    void collapseBranches();
    void save();
    void saveAll();
    //void saveAs();
    //void exit();
    void changedTabs(int index);
    void closeTab(int index);
private:
    bool openBehavior(const QString & filename, QProgressDialog *dialog);
    void setProgressData(const QString & message, int max, int min, int value);
    bool exitProgram();
    //void drawIcons();
    void readSettings();
    void writeSettings();
    QMessageBox::StandardButton closeAllDialogue();
    QMessageBox::StandardButton closeFileDialogue();
    bool closeAll();
};

#endif // MAINWINDOW_H
