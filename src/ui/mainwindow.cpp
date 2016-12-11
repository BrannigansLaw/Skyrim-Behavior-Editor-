#include "mainwindow.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/ui/hkxclassesui/hkdataui.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorvariablesui.h"
#include "src/ui/hkxclassesui/behaviorui/eventsui.h"
#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"

#include <QtWidgets>

#define MAX_REFERENCED_BEHAVIOR_FILES 30

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new QPlainTextEdit(this)),
      openProjectA(new QAction("Open Project", this)),
      openBehaviorA(new QAction("Open Behavior", this)),
      fileM(new QMenu("File", this)),
      saveA(new QAction("Save", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      tabs(new QTabWidget),
      projectFile(NULL),
      characterFile(NULL),
      skeletonFile(NULL),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      objectDataSA(new QScrollArea),
      eventsWid(new EventsUI("Events")),
      variablesWid(new BehaviorVariablesUI("Behavior Variables")),
      objectDataWid(new HkDataUI("Object Data")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      progressD(/*new QProgressDialog(this)*/NULL)
{
    //logGB->setMinimumHeight(300);
    //logGB->setMinimumWidth(300);
    openProjectA->setStatusTip("Open a hkx project file!");
    openProjectA->setShortcut(QKeySequence::Open);
    openBehaviorA->setStatusTip("Open a hkx behavior file!");
    openBehaviorA->setShortcut(QKeySequence::Paste);
    saveA->setStatusTip("Save file!");
    saveA->setShortcut(QKeySequence::Save);
    fileM->addAction(openProjectA);
    fileM->addAction(openBehaviorA);
    fileM->addAction(saveA);
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
    iconGBLyt->addWidget(tabs);
    behaviorGraphViewGB->setLayout(iconGBLyt);
    topLyt->addWidget(topMB, 0, 0, 1, 10);
    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 6, 6);
    objectDataSA->setWidgetResizable(true);
    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
    topLyt->addWidget(eventsWid, 7, 0, 4, 3);
    topLyt->addWidget(variablesWid, 7, 3, 4, 3);
    topLyt->addWidget(logGB, 7, 6, 3, 4);
    tabs->setTabsClosable(true);
    readSettings();
    setLayout(topLyt);
    objectDataSA->setWidget(objectDataWid);
    variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.4, size().height()*0.25);
    connect(openProjectA, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    connect(openBehaviorA, SIGNAL(triggered(bool)), this, SLOT(openBehaviorFile()));
    connect(saveA, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()));
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)));
}

MainWindow::~MainWindow(){
    //
}

void MainWindow::changedTabs(int index){
    if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
        objectDataWid->setBehaviorView(behaviorGraphs.at(index));
        objectDataWid->changeCurrentDataWidget(NULL);
        if (progressD){
            delete progressD;
        }
        progressD = new QProgressDialog(this);
        progressD->setMinimumSize(QSize(800, 200));
        progressD->setWindowModality(Qt::WindowModal);
        setProgressData("Loading Variables...", 0);
        variablesWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
        setProgressData("Loading Events...", 50);
        eventsWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
        progressD->setValue(100);
        progressD->done(0);
        progressD->close();
    }else{
        writeToLog("MainWindow: changedTabs() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
}

void MainWindow::expandBranches(){
    if (tabs->currentIndex() >= 0 && tabs->currentIndex() < behaviorFiles.size() && tabs->currentIndex() < behaviorGraphs.size()){
        behaviorGraphs.at(tabs->currentIndex())->expandBranch(behaviorGraphs.at(tabs->currentIndex())->rootIcon, true);
    }else{
        writeToLog("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
}

void MainWindow::collapseBranches(){
    if (tabs->currentIndex() >= 0 && tabs->currentIndex() < behaviorFiles.size() && tabs->currentIndex() < behaviorGraphs.size()){
        behaviorGraphs.at(tabs->currentIndex())->contractBranch(behaviorGraphs.at(tabs->currentIndex())->rootIcon, true);
        behaviorGraphs.at(tabs->currentIndex())->rootIcon->setSelected();
    }else{
        writeToLog("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
}

void MainWindow::save(){
    if (tabs->currentIndex() >= 0 && tabs->currentIndex() < behaviorFiles.size()){
        behaviorFiles.at(tabs->currentIndex())->write();
    }else{
        writeToLog("MainWindow: save() failed!\nThe tab index is out of sync with the behavior files!", true);
    }
}

void MainWindow::setProgressData(const QString & message, int max, int min, int value){
    if (progressD){
        //progressD->setWindowTitle("Opening "+dirViewFSW->getSelectedFilename());
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

void MainWindow::openProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx project file..."), "C:/", tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    QTime t;
    t.start();
    if (progressD){
        delete progressD;
    }
    progressD = new QProgressDialog(this);
    progressD->setMinimumSize(QSize(800, 200));
    progressD->setWindowModality(Qt::WindowModal);
    setProgressData("Opening project...", 0, 100, 0);
    objectDataWid->changeCurrentDataWidget(NULL);
    setProgressData("Beginning XML parse...", 5);
    projectFile = new ProjectFile(this, filename);
    if (!projectFile->parse()){
        delete projectFile;
        projectFile = NULL;
        progressD->close();
        return;
    }
    setProgressData("Loading project data...", 15);
    QString path = filename;
    int temp = filename.lastIndexOf("/");
    path.remove(temp, path.size() - temp);
    characterFile = new CharacterFile(this, path+"\\"+projectFile->getCharacterFilePathAt(0));
    if (!characterFile->parse()){
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        progressD->close();
        return;
    }
    setProgressData("Loading behavior files...", 10);
    int progress = 10;
    bool failed = false;
    QStringList behaviors(characterFile->getRootBehaviorPath());
    QStringList behaviorsOpened;
    QString opened;
    for (int i = 0; i < behaviors.size(), i < MAX_REFERENCED_BEHAVIOR_FILES; i++){
        if (i >= behaviors.size()){
            break;
        }
        opened = path+"/"+behaviors.at(i);
        if (!behaviorsOpened.contains(opened)){
            if (openBehavior(opened, progressD)){
                behaviorsOpened.append(opened);
                behaviors = behaviors + behaviorFiles.last()->getAllReferencedBehaviorFilePaths();
                behaviors.removeDuplicates();
                setProgressData("Behavior file"+behaviors.at(i)+" loaded...", progress += 3);
                debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+opened+
                                          "\" is approximately "+QString::number(t.elapsed()/1000)+" seconds\n-------------------------\n");
            }else{
                failed = true;
            }
        }
    }
    if (behaviorFiles.isEmpty() || failed){
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        progressD->close();
        for (int j = 0; j < behaviorFiles.size(); j++){
            if (behaviorFiles.at(j)){
                delete behaviorFiles.at(j);
            }
            if (j < behaviorGraphs.size()){
                if (behaviorFiles.at(j)){
                    delete behaviorGraphs.at(j);
                }
            }
        }
        behaviorFiles.clear();
        behaviorGraphs.clear();
        return;
    }
    setProgressData("Loading Variables...", 98);
    variablesWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    setProgressData("Loading Events...", 99);
    eventsWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    progressD->setValue(100);
    progressD->done(0);
    progressD->close();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open project \""+filename+
                              "\" is approximately "+QString::number(t.elapsed()/1000)+" seconds\n-------------------------\n");
}

bool MainWindow::openBehavior(const QString & filename, QProgressDialog *dialog){
    if (filename == "" || !dialog){
        return false;
    }
    QTime t;
    t.start();
    setProgressData("Opening file...", 0, 100, 0);
    objectDataWid->changeCurrentDataWidget(NULL);
    setProgressData("Beginning XML parse...", 5);
    behaviorFiles.append(new BehaviorFile(this, filename));
    if (!behaviorFiles.last()->parse()){
        delete behaviorFiles.last();
        behaviorFiles.removeLast();
        progressD->close();
        return false;
    }
    behaviorGraphs.append(new BehaviorGraphView(objectDataWid, behaviorFiles.last()));
    tabs->addTab(behaviorGraphs.last(), filename.split("/").last());
    setProgressData("Drawing Behavior Graph...", 60);
    if (!behaviorGraphs.last()->draw(static_cast<DataIconManager *>(behaviorFiles.last()->getBehaviorGraph()), static_cast<DataIconManager *>(behaviorFiles.last()->getRootStateMachine()))){
        writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
    }
    setProgressData("Loading Variables...", 95);
    variablesWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    setProgressData("Loading Events...", 99);
    eventsWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    dialog->setValue(100);
    dialog->done(0);
    dialog->close();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+filename+"\" is approximately "+QString::number(t.elapsed()/1000)+" seconds\n-------------------------\n");
    return true;
}

void MainWindow::openBehaviorFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open hkx behavior file..."), "C:/", tr("hkx Files (*.hkx)"));
    openBehavior(fileName, progressD);
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
