#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"
#include "generators.h"
#include "behaviorgraphui.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

MainWindow::MainWindow(){
    topLyt = new QVBoxLayout(this);
    topMB = new QMenuBar(this);
    openA = new QAction("Open Project", this);
    openA->setStatusTip("Open a hkx project file!");
    openA->setShortcut(QKeySequence::Open);
    openM =new QMenu("File");
    openM->addAction(openA);
    topMB->addMenu(openM);
    topLyt->addWidget(topMB);
    dirViewFSW = new FileSelectWindow("Select a hkx file to open!");
    hkxFile = NULL;
    iconGBLyt = new QVBoxLayout;
    behaviorGraphViewGB = new QGroupBox("Behavior Graph");
    //BehaviorGraphViewGB->setLayout(iconGBLyt);
    readSettings();
    setLayout(topLyt);
    connect(openA, SIGNAL(triggered(bool)), this, SLOT(openDirView()));
    connect(dirViewFSW, SIGNAL(selectFile(QString)), this, SLOT(openHkxfile(QString)));
}

MainWindow::~MainWindow(){
    //
}

void MainWindow::openDirView(){
    dirViewFSW->show();
}

void MainWindow::openHkxfile(QString name){
    if (!hkxFile){
        hkxFile = new BehaviorFile(name);
        behaviorGraphViewIV = new BehaviorGraphView(hkxFile);
        behaviorGraphViewGB->setTitle(name);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            //
        }
    }else{
        delete hkxFile;
        delete behaviorGraphViewIV;
        behaviorGraphViewGB->setTitle(name);
        hkxFile = new BehaviorFile(name);
        behaviorGraphViewIV = new BehaviorGraphView(hkxFile);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            //
        }
    }
    iconGBLyt->addWidget(behaviorGraphViewIV);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    topLyt->addWidget(behaviorGraphViewGB);
}

void MainWindow::readSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool MainWindow::exitProgram()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"), "Quit?", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        return true;
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (exitProgram()) {
        writeSettings();
        event->accept();
    }else {
        event->ignore();
    }
}
