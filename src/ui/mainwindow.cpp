#include "mainwindow.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/hkxclassesui/hkdataui.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorvariablesui.h"
#include "src/ui/hkxclassesui/behaviorui/characterpropertiesui.h"
#include "src/ui/hkxclassesui/behaviorui/eventsui.h"
#include "src/ui/hkxclassesui/projectui.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"

#include <QtWidgets>

#define MAX_REFERENCED_BEHAVIOR_FILES 30
#define CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING "WARNING: There are unsaved changes to the project files currently open!\nAre you sure you want to close them without saving?"
#define CONFIRM_CLOSE_FILE_WITHOUT_SAVING "WARNING: There are unsaved changes to the behavior file currently open!\nAre you sure you want to close it without saving?"

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new PlainTextEdit(this)),
      openProjectA(new QAction("Open Project", this)),
      //openBehaviorA(new QAction("Open Behavior", this)),
      fileM(new QMenu("File", this)),
      saveA(new QAction("Save", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      tabs(new QTabWidget),
      projectFile(NULL),
      characterFile(NULL),
      skeletonFile(NULL),
      projectUI(new ProjectUI(NULL)),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      objectDataSA(new QScrollArea),
      eventsWid(new EventsUI("Events")),
      variablesWid(new BehaviorVariablesUI("Behavior Variables")),
      objectDataWid(new HkDataUI("Object Data")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      progressD(new QProgressDialog(this)),
      lastFileSelected("C:/")
{
    progressD->setMinimumSize(QSize(800, 200));
    progressD->setWindowModality(Qt::WindowModal);
    progressD->cancel();
    openProjectA->setStatusTip("Open a hkx project file!");
    openProjectA->setShortcut(QKeySequence::Open);
    //openBehaviorA->setStatusTip("Open a hkx behavior file!");
    //openBehaviorA->setShortcut(QKeySequence::Paste);
    saveA->setStatusTip("Save file!");
    saveA->setShortcut(QKeySequence::Save);
    fileM->addAction(openProjectA);
    //fileM->addAction(openBehaviorA);
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
    objectDataSA->setWidgetResizable(true);
    topLyt->addWidget(topMB, 0, 0, 1, 10);
    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 6);
    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
    topLyt->addWidget(eventsWid, 7, 6, 4, 2);
    topLyt->addWidget(variablesWid, 7, 8, 4, 2);
    topLyt->addWidget(logGB, 10, 0, 1, 6);
    tabs->setTabsClosable(true);
    readSettings();
    setLayout(topLyt);
    objectDataSA->setWidget(objectDataWid);
    //variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    //eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.6, size().height()*0.1);
    connect(openProjectA, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    //connect(openBehaviorA, SIGNAL(triggered(bool)), this, SLOT(openBehaviorFile()));
    connect(saveA, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()));
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)));
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(projectUI, SIGNAL(openFile(QModelIndex)), this, SLOT(openBehaviorFile(QModelIndex)));
}

MainWindow::~MainWindow(){
    //
}

QMessageBox::StandardButton MainWindow::closeAllDialogue(){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Skyrim Behavior Tool", CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING, QMessageBox::Yes | QMessageBox::SaveAll | QMessageBox::Cancel);
    if (ret == QMessageBox::SaveAll){
        saveAll();
    }
    return ret;
}

QMessageBox::StandardButton MainWindow::closeFileDialogue(){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Skyrim Behavior Tool", CONFIRM_CLOSE_FILE_WITHOUT_SAVING, QMessageBox::Yes | QMessageBox::Save | QMessageBox::Cancel);
    if (ret == QMessageBox::Save){
        save();
    }
    return ret;
}

void MainWindow::changedTabs(int index){
    index--;
    if (index < behaviorFiles.size() && index < behaviorGraphs.size()){
        objectDataWid->changeCurrentDataWidget(NULL);
        progressD->open(this, "");
        if (index == -1){
            variablesWid->hide();
            eventsWid->hide();
            objectDataSA->hide();
            topLyt->removeWidget(objectDataSA);
            topLyt->removeWidget(eventsWid);
            topLyt->removeWidget(variablesWid);
            topLyt->removeWidget(behaviorGraphViewGB);
            topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 10);
        }else if (index >= 0){
            objectDataWid->setBehaviorView(behaviorGraphs.at(index));
            setProgressData("Loading Variables...", 0);
            variablesWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
            setProgressData("Loading Events...", 99);
            eventsWid->loadData(behaviorFiles.at(index)->getBehaviorGraphData());
            topLyt->removeWidget(behaviorGraphViewGB);
            topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 6);
            topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
            topLyt->addWidget(eventsWid, 7, 6, 4, 2);
            topLyt->addWidget(variablesWid, 7, 8, 4, 2);
            variablesWid->show();
            eventsWid->show();
            objectDataSA->show();
            progressD->setValue(100);
            progressD->done(0);
            progressD->close();
        }
    }else{
        writeToLog("MainWindow: changedTabs() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
    progressD->close();
}

void MainWindow::expandBranches(){
    int index = tabs->currentIndex() - 1;
    if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
        behaviorGraphs.at(index)->expandAllBranches();
    }else{
        writeToLog("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
}

void MainWindow::collapseBranches(){
    int index = tabs->currentIndex() - 1;
    if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
        behaviorGraphs.at(index)->contractAllBranches();
        behaviorGraphs.at(index)->selectRoot();
    }else{
        writeToLog("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
    }
}

void MainWindow::save(){
    int index = tabs->currentIndex() - 1;
    if (index >= 0 && index < behaviorFiles.size() && index < behaviorGraphs.size()){
        behaviorFiles.at(index )->write();
        behaviorFiles.at(index)->toggleChanged(false);
    }else{
        writeToLog("MainWindow: save() failed!\nThe tab index is out of sync with the behavior files!", true);
    }
}

void MainWindow::saveAll(){
    for (int i = 0; i < behaviorFiles.size(); i++){
        if (behaviorFiles.at(i) && i < behaviorGraphs.size() && behaviorGraphs.at(i)){
            behaviorFiles.at(i)->write();
            behaviorFiles.at(i)->toggleChanged(false);
        }else{
            writeToLog("MainWindow: save() failed!\nThe tab index is out of sync with the behavior files or a NULL pointer in either the behavior or behavior graph list was encountered!", true);
        }
    }
}

void MainWindow::closeTab(int index){
    if (projectFile && index == 0){
        closeAll();
    }else{
        index--;
        if (index >= 0 && index < behaviorGraphs.size() && (!behaviorFiles.at(index)->getIsChanged() || closeFileDialogue() != QMessageBox::Cancel)){
            if (index < behaviorFiles.size()){
                if (behaviorGraphs.at(index)){
                    delete behaviorGraphs.at(index);
                }
                behaviorGraphs.removeAt(index);
                if (behaviorFiles.at(index)){
                    delete behaviorFiles.at(index);
                }
                behaviorFiles.removeAt(index);
            }
        }else{
            writeToLog("MainWindow: closeTab()\nThe tab index is out of sync with the behavior files or behavior graphs!", true);
        }
    }
}

bool MainWindow::closeAll(){
    for (int i = 0; i < behaviorGraphs.size(); i++){
        if (behaviorFiles.at(i)->getIsChanged()){
            if (closeAllDialogue() != QMessageBox::Cancel){
                tabs->clear();
                for (int j = 0; j < behaviorGraphs.size(); j++){
                    if (behaviorGraphs.at(j)){
                        delete behaviorGraphs.at(j);
                    }
                }
                behaviorGraphs.clear();
                for (int j = 0; j < behaviorFiles.size(); j++){
                    if (behaviorFiles.at(j)){
                        delete behaviorFiles.at(j);
                    }
                }
                behaviorFiles.clear();
                if (projectFile){
                    delete projectFile;
                    projectFile = NULL;
                }
                if (characterFile){
                    delete characterFile;
                    characterFile = NULL;
                }
                if (skeletonFile){
                    delete skeletonFile;
                    skeletonFile = NULL;
                }
                return true;
            }else{
                return false;
            }
        }
    }
    return false;
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx project file..."), lastFileSelected, tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    lastFileSelected = filename;
    QTime t;
    t.start();
    progressD->open(this, "");
    setProgressData("Opening project...", 0, 100, 0);
    objectDataWid->changeCurrentDataWidget(NULL);
    setProgressData("Loading project data...", 2);
    int time = t.elapsed();
    projectFile = new ProjectFile(this, filename);
    if (!projectFile->parse()){
        writeToLog("MainWindow: openProject() failed!!!\nThe project file "+filename+" could not be parsed!!!", true);
        delete projectFile;
        projectFile = NULL;
        progressD->close();
        return;
    }
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+filename+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    setProgressData("Loading character data...", 1);
    lastFileSelectedPath = filename;
    int temp = filename.lastIndexOf("/");
    lastFileSelectedPath.remove(temp, lastFileSelectedPath.size() - temp);
    projectUI->setFilePath(lastFileSelectedPath);
    time = t.elapsed();
    characterFile = new CharacterFile(this, lastFileSelectedPath+"\\"+projectFile->getCharacterFilePathAt(0));
    if (!characterFile->parse()){
        writeToLog("MainWindow: openProject() failed!!!\nThe character file "+projectFile->getCharacterFilePathAt(0)+" could not be parsed!!!", true);
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        progressD->close();
        return;
    }
    projectFile->setCharacterFile(characterFile);
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"\\"+projectFile->getCharacterFilePathAt(0)+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    setProgressData("Loading skeleton data...", 6);
    time = t.elapsed();
    skeletonFile = new SkeletonFile(this, lastFileSelectedPath+"\\"+characterFile->getRigName());
    if (!skeletonFile->parse()){
        writeToLog("MainWindow: openProject() failed!!!\nThe skeleton file "+characterFile->getRigName()+" could not be parsed!!!", true);
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        delete skeletonFile;
        skeletonFile = NULL;
        progressD->close();
        return;
    }
    characterFile->setSkeletonFile(skeletonFile);
    tabs->addTab(projectUI, "Character Data");
    projectUI->setProject(projectFile);
    projectUI->loadData();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"\\"+characterFile->getRigName()+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    /*setProgressData("Loading behavior files...", 10);
    int progress = 10;
    bool failed = false;
    QStringList behaviors(characterFile->getRootBehaviorPath());
    QStringList behaviorsOpened;
    QString opened;
    time = t.elapsed();
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
                                          "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
                time = t.elapsed();
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
    //setProgressData("Loading Variables...", 98);
    //variablesWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    //setProgressData("Loading Events...", 99);
    //eventsWid->loadData(behaviorFiles.last()->getBehaviorGraphData());*/
    progressD->setValue(100);
    progressD->done(0);
    progressD->close();
    debugLog->appendPlainText("\n-------------------------\nTime taken to open project \""+filename+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
}

bool MainWindow::openBehavior(const QString & filename, QProgressDialog *dialog){
    if (filename == ""){
        return false;
    }
    progressD->open(this, "");
    setProgressData("Opening file...", 0, 100, 0);
    objectDataWid->changeCurrentDataWidget(NULL);
    setProgressData("Beginning XML parse...", 5);
    behaviorFiles.append(new BehaviorFile(this, characterFile, filename));
    if (!behaviorFiles.last()->parse()){
        delete behaviorFiles.last();
        behaviorFiles.removeLast();
        progressD->close();
        return false;
    }
    behaviorGraphs.append(new BehaviorGraphView(objectDataWid, behaviorFiles.last()));
    tabs->addTab(behaviorGraphs.last(), filename.split("/").last());
    setProgressData("Drawing Behavior Graph...", 60);
    if (!behaviorGraphs.last()->drawGraph(static_cast<DataIconManager *>(behaviorFiles.last()->getBehaviorGraph()))){
        writeToLog("MainWindow: drawBehaviorGraph() failed!\nThe behavior graph was drawn incorrectly!", true);
    }
    //setProgressData("Loading Variables...", 95);
    //variablesWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    //setProgressData("Loading Events...", 99);
    //eventsWid->loadData(behaviorFiles.last()->getBehaviorGraphData());
    dialog->setValue(100);
    dialog->done(0);
    dialog->close();
    tabs->setCurrentIndex(tabs->count() - 1);
    return true;
}

void MainWindow::openBehaviorFile(const QModelIndex & index){
    QString fileName = index.data().toString();
    QTime t;
    t.start();
    if (openBehavior(lastFileSelectedPath+"\\behaviors\\"+fileName, progressD)){
        debugLog->appendPlainText("\n-------------------------\nTime taken to open behavior \""+fileName+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    }else{
        writeToLog("MainWindow: openBehaviorFile() failed!\nThe behavior file "+fileName+" failded to open correctly!", true);
    }
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
