#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

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

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    QVBoxLayout *topLyt;
    QHBoxLayout *mainLyt;
    QMenuBar *topMB;
    QAction *openA;
    QMenu *openM;
    FileSelectWindow *dirViewFSW;
    BehaviorFile *hkxFile;
    BehaviorGraphView *behaviorGraphViewIV;
    QGroupBox *behaviorGraphViewGB;
    QVBoxLayout *iconGBLyt;
    QGroupBox *objectDataGB;
private slots:
    void openDirView();
    void openHkxfile(QString name);
private:
    bool exitProgram();
    //void drawIcons();
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_H
