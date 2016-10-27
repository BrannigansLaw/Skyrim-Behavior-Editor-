#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QtWidgets>

#include "src/hkxclasses/hkxobject.h"

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorFile;
class BehaviorGraphView;
class QPlainTextEdit;
class HkDataUI;
class BehaviorVariablesUI;
class EventsUI;

class MainWindow : public QWidget
{
    Q_OBJECT
    friend class BehaviorFile;
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
    QAction *openA;
    QMenu *fileM;
    QMenu *viewM;
    QAction *expandA;
    QAction *collapseA;
    FileSelectWindow *dirViewFSW;
    BehaviorFile *hkxFile;
    BehaviorGraphView *behaviorGraphViewIV;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    //QGroupBox *objectDataGB;
    HkDataUI *objectDataWid;
    QScrollArea *objectDataSA;
    BehaviorVariablesUI *variablesWid;
    EventsUI *eventsWid;
    QGroupBox *logGB;
    QVBoxLayout *logGBLyt;
    QProgressDialog *progressD;
    bool drawGraph;
private slots:
    void openDirView();
    void openHkxfile(QString name);
    void expandBranches();
    void collapseBranches();
private:
    void setProgressData(const QString & message, int max, int min, int value);
    bool exitProgram();
    //void drawIcons();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H