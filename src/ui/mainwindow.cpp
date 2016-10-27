#include "mainwindow.h"
#include "fileselectwindow.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/hkdataui.h"
#include "src/ui/behaviorvariablesui.h"
#include "src/ui/eventsui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"

#include <QtWidgets>

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new QPlainTextEdit(this)),
      openA(new QAction("Open Project", this)),
      fileM(new QMenu("File", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      behaviorGraphViewIV(NULL),
      dirViewFSW(new FileSelectWindow("Select a hkx file to open!")),
      hkxFile(NULL),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      objectDataSA(new QScrollArea),
      eventsWid(new EventsUI("Events")),
      variablesWid(new BehaviorVariablesUI("Behavior Variables")),
      objectDataWid(new HkDataUI("Object Data")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      drawGraph(true),
      progressD(/*new QProgressDialog(this)*/NULL)
{
    //logGB->setMinimumHeight(300);
    //logGB->setMinimumWidth(300);
    openA->setStatusTip("Open a hkx project file!");
    openA->setShortcut(QKeySequence::Open);
    fileM->addAction(openA);
    expandA->setStatusTip("Expand all branches!");
    expandA->setShortcut(QKeySequence::ZoomIn);
    viewM->addAction(expandA);
    collapseA->setStatusTip("Collapse all branches!");
    collapseA->setShortcut(QKeySequence::ZoomOut);
    viewM->addAction(collapseA);
    topMB->setMaximumHeight(50);
    topMB->addMenu(fileM);
    topMB->addMenu(viewM);
    logGBLyt->addWidget(debugLog);
    logGB->setLayout(logGBLyt);
    eventsWid->setHkDataUI(objectDataWid);
    variablesWid->setHkDataUI(objectDataWid);
    objectDataWid->setEventsVariablesUI(eventsWid, variablesWid);
    topLyt->addWidget(topMB, 0, 0, 1, 10);
    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 6, 6);
    //topLyt->addWidget(objectDataGB, 1, 6, 6, 4);
    objectDataSA->setWidgetResizable(true);
    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
    topLyt->addWidget(eventsWid, 7, 0, 4, 3);
    topLyt->addWidget(variablesWid, 7, 3, 4, 3);
    topLyt->addWidget(logGB, 7, 6, 3, 4);
    //progressD->hide();
    readSettings();
    setLayout(topLyt);
    objectDataSA->setWidget(objectDataWid);
    variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.4, size().height()*0.25);
    connect(openA, SIGNAL(triggered(bool)), this, SLOT(openDirView()));
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()));
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()));
    connect(dirViewFSW, SIGNAL(selectFile(QString)), this, SLOT(openHkxfile(QString)));
}

MainWindow::~MainWindow(){
    //
}

void MainWindow::expandBranches(){
    if (behaviorGraphViewIV){
        behaviorGraphViewIV->expandBranch(behaviorGraphViewIV->rootIcon, true);
    }
}

void MainWindow::collapseBranches(){
    if (behaviorGraphViewIV){
        behaviorGraphViewIV->contractBranch(behaviorGraphViewIV->rootIcon, true);
        behaviorGraphViewIV->rootIcon->setSelected();
    }
}

void MainWindow::setProgressData(const QString & message, int max, int min, int value){
    if (progressD){
        progressD->setWindowTitle("Opening "+dirViewFSW->getSelectedFilename());
        progressD->setLabelText(message);
        progressD->setMaximum(max);
        progressD->setMinimum(min);
        progressD->setValue(value);
        progressD->resize(geometry().width()*0.15, geometry().height()*0.05);
        progressD->open();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

void MainWindow::setProgressData(const QString & message, int value){
    if (progressD){
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        progressD->setLabelText(message);
        progressD->setValue(value);
        //progressD->resize(geometry().width()*0.15, geometry().height()*0.15);
        //progressD->open();
    }
}

void MainWindow::writeToLog(const QString &message, bool isError){
    QString str;
    QString temp = "\n#########################################";
    if (!isError){
        str = "WARNING!!!-->";
    }else{
        str = "ERROR!!!-->";
    }
    debugLog->appendPlainText(temp+"\n"+str+message+temp);
    QTextCursor tempTC(debugLog->document());
    tempTC.movePosition(QTextCursor::End);
    debugLog->setTextCursor(tempTC);
}

void MainWindow::openDirView(){
    dirViewFSW->show();
}

void MainWindow::openHkxfile(QString name){
    QTime t;
    t.start();
    if (progressD){
        delete progressD;
    }
    progressD = new QProgressDialog(this);
    progressD->setMinimumSize(QSize(800, 200));
    progressD->setWindowModality(Qt::WindowModal);
    setProgressData("Opening file...", 0, 100, 0);
    drawGraph = true;
    if (!hkxFile){
        setProgressData("Beginning XML parse...", 5);
        hkxFile = new BehaviorFile(this, name);
        if (!drawGraph){
            delete hkxFile;
            hkxFile = NULL;
            return;
        }
        if (!behaviorGraphViewIV){
            behaviorGraphViewIV = new BehaviorGraphView(objectDataWid, hkxFile);
        }
        //behaviorGraphViewIV->setBehaviorFile(hkxFile);
        behaviorGraphViewGB->setTitle(name);
        setProgressData("Drawing Behavior Graph...", 60);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
        }
    }else{
        setProgressData("Deleting previously loaded Behavior Graph...", 0);
        delete hkxFile;
        delete behaviorGraphViewIV;
        hkxFile = NULL;
        behaviorGraphViewIV = NULL;
        behaviorGraphViewGB->setTitle(name);
        setProgressData("Beginning XML parse...", 5);
        hkxFile = new BehaviorFile(this, name);
        if (!drawGraph){
            delete hkxFile;
            hkxFile = NULL;
            return;
        }
        if (!behaviorGraphViewIV){
            behaviorGraphViewIV = new BehaviorGraphView(objectDataWid, hkxFile);
        }
        //behaviorGraphViewIV->setBehaviorFile(hkxFile);
        setProgressData("Drawing Behavior Graph...", 90);
        if (!behaviorGraphViewIV->drawBehaviorGraph()){
            writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
        }
    }
    setProgressData("Loading Variables...", 95);
    variablesWid->loadData(hkxFile->getBehaviorGraphData());
    setProgressData("Loading Events...", 99);
    eventsWid->loadData(hkxFile->getBehaviorGraphData());
    iconGBLyt->addWidget(behaviorGraphViewIV);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    progressD->setValue(100);
    progressD->done(0);
    progressD->close();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+dirViewFSW->getSelectedFilename()+"\" is approximately "+QString::number(t.elapsed()/1000)+" seconds\n-------------------------\n");
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
