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
    void setProgressData(const QString & message, int value);
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
private:
    PlainTextEdit *debugLog;
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
    ProjectUI *projectUI;
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
    QString lastFileSelectedPath;
private slots:
    void openProject();
    void openBehaviorFile(const QModelIndex & index);
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
