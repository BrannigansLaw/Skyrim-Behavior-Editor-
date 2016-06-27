#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QStringBuilder>
#include <QtWidgets>

#include "hkobject.h"

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
    QMenu *openM;
    FileSelectWindow *dirViewFSW;
    BehaviorFile *hkxFile;
    BehaviorGraphView *behaviorGraphViewIV;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    QGroupBox *objectDataGB;
    QGroupBox *eventsGB;
    QGroupBox *variablesGB;
    QGroupBox *logGB;
    QVBoxLayout *logGBLyt;
    QProgressDialog *progressD;
    bool drawGraph;
private slots:
    void openDirView();
    void openHkxfile(QString name);
private:
    void setProgressData(const QString & message, int max, int min, int value);
    bool exitProgram();
    //void drawIcons();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
