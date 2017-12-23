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
#include "src/ui/genericdatawidgets.h"

#include <QtWidgets>

#include <thread>
#include <mutex>

#define MAX_REFERENCED_BEHAVIOR_FILES 30
#define CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING "WARNING: There are unsaved changes to the project, character or behavior files currently open!\nAre you sure you want to close them without saving?"
#define CONFIRM_CLOSE_FILE_WITHOUT_SAVING "WARNING: There are unsaved changes to the behavior file currently open!\nAre you sure you want to close it without saving?"

MainWindow::MainWindow()
    :
      topLyt(new QGridLayout(this)),
      topMB(new QMenuBar(this)),
      debugLog(new PlainTextEdit(this)),
      openPackedProjectA(new QAction("Open Packed Project", this)),
      openUnpackedProjectA(new QAction("Open Unpacked Project", this)),
      exportToSkyrimDirA(new QAction("Export Project To Skyrim Directory", this)),
      exportCurrentFileA(new QAction("Export Current File To Skyrim Directory", this)),
      fileM(new QMenu("File", this)),
      saveA(new QAction("Save Viewed Behavior File", this)),
      saveProjectA(new QAction("Save Project", this)),
      exitA(new QAction("Exit", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      refocusA(new QAction("Refocus On Selected Item", this)),
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
      lastFileSelected("C:/")
{
    //setStyleSheet("QComboBox {background: yellow};QWidget {background: darkGray}");
    projectUI->setDisabled(true);

#ifdef MY_DEBUG
    hkxcmdPath = "c:/users/wayne/desktop/hkxcmd.exe";
#else
    hkxcmdPath = QDir::currentPath()+"/hkxcmd.exe";
#endif

    openPackedProjectA->setStatusTip("Open a hkx project file!");
    //openPackedProjectA->setShortcut(QKeySequence::Open);
    openUnpackedProjectA->setStatusTip("Open a hkx xml project file!");
    //openUnpackedProjectA->setShortcut(QKeySequence::Open);
    exportToSkyrimDirA->setStatusTip("Pack and export the current project to the working Skyrim directory!");
    //exportToSkyrimDirA->setShortcut(QKeySequence::Copy);
    exportCurrentFileA->setStatusTip("Pack and export the current file to the working Skyrim directory!");
    //exportCurrentFileA->setShortcut(QKeySequence::Paste);
    saveA->setStatusTip("Save file!");
    //saveA->setShortcut(QKeySequence::Save);
    saveProjectA->setStatusTip("Save project!");
    //saveProjectA->setShortcut(QKeySequence::SaveAs);
    exitA->setStatusTip("Exit!");
    //exitA->setShortcut(QKeySequence::Close);
    fileM->addAction(openPackedProjectA);
    fileM->addAction(openUnpackedProjectA);
    fileM->addAction(exportToSkyrimDirA);
    fileM->addAction(exportCurrentFileA);
    fileM->addAction(saveA);
    fileM->addAction(saveProjectA);
    fileM->addAction(exitA);
    expandA->setStatusTip("Expand all branches!");
    //expandA->setShortcut(QKeySequence::ZoomIn);
    collapseA->setStatusTip("Collapse all branches!");
    //collapseA->setShortcut(QKeySequence::ZoomOut);
    refocusA->setStatusTip("Centers the behavior view on the selected item!");
    //refocusA->setShortcut(QKeySequence::Back);
    viewM->addAction(expandA);
    viewM->addAction(collapseA);
    viewM->addAction(refocusA);
    topMB->setMaximumHeight(50);
    topMB->addMenu(fileM);
    topMB->addMenu(viewM);
    logGBLyt->addWidget(debugLog);
    logGB->setLayout(logGBLyt);
    eventsWid->setHkDataUI(objectDataWid);
    variablesWid->setHkDataUI(objectDataWid);
    objectDataWid->setEventsVariablesAnimationsUI(eventsWid, variablesWid, projectUI->animations);
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
    //objectDataSA->setStyleSheet("QScrollArea {background-color:cyan}");
    //variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    //eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.6, size().height()*0.1);
    if (!findSkyrimDirectory()){
        (qWarning("The TESV executable was not found!"));
    }
    connect(openPackedProjectA, SIGNAL(triggered(bool)), this, SLOT(openPackedProject()), Qt::UniqueConnection);
    connect(openUnpackedProjectA, SIGNAL(triggered(bool)), this, SLOT(openUnpackedProject()), Qt::UniqueConnection);
    connect(saveA, SIGNAL(triggered(bool)), this, SLOT(save()), Qt::UniqueConnection);
    connect(exportToSkyrimDirA, SIGNAL(triggered(bool)), this, SLOT(packAndExportProjectToSkyrimDirectory()), Qt::UniqueConnection);
    connect(exportCurrentFileA, SIGNAL(triggered(bool)), this, SLOT(packAndExportFileToSkyrimDirectory()), Qt::UniqueConnection);
    connect(saveProjectA, SIGNAL(triggered(bool)), this, SLOT(saveProject()), Qt::UniqueConnection);
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()), Qt::UniqueConnection);
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()), Qt::UniqueConnection);
    connect(refocusA, SIGNAL(triggered(bool)), this, SLOT(refocus()), Qt::UniqueConnection);
    connect(exitA, SIGNAL(triggered(bool)), this, SLOT(exit()), Qt::UniqueConnection);
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)), Qt::UniqueConnection);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(openFile(QModelIndex)), this, SLOT(openBehaviorFile(QModelIndex)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(addBehavior(bool)), this, SLOT(addNewBehavior(bool)), Qt::UniqueConnection);
}

MainWindow::~MainWindow(){
    /*for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
        delete projectFile->behaviorFiles.at(i);
    }*/
    delete characterFile;
    delete skeletonFile;
    delete projectFile;
}

QMessageBox::StandardButton MainWindow::yesNoDialogue(const QString & message){
    return QMessageBox::warning(0, "Skyrim Behavior Tool", message, QMessageBox::Yes | QMessageBox::No);
}

QMessageBox::StandardButton MainWindow::closeAllDialogue(){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, "Skyrim Behavior Tool", CONFIRM_CLOSE_PROJECT_WITHOUT_SAVING, QMessageBox::Yes | QMessageBox::SaveAll | QMessageBox::Cancel);
    if (ret == QMessageBox::SaveAll){
        saveProject();
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
    //ProgressDialog dialog("Changing viewed behavior...", "", 0, 100, this);
    if (projectFile){
        if (!projectFile->behaviorFiles.isEmpty()){
            if (index <= 0){
                variablesWid->hide();
                eventsWid->hide();
                objectDataSA->hide();
                topLyt->removeWidget(objectDataSA);
                topLyt->removeWidget(eventsWid);
                topLyt->removeWidget(variablesWid);
                topLyt->removeWidget(behaviorGraphViewGB);
                topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 10);
            }else{
                int graphindex = getBehaviorGraphIndex(tabs->tabText(index));
                if (graphindex < projectFile->behaviorFiles.size() && graphindex < behaviorGraphs.size()){
                    objectDataWid->changeCurrentDataWidget(NULL);
                    objectDataWid->loadBehaviorView(behaviorGraphs.at(graphindex));
                    variablesWid->clear();
                    variablesWid->loadData(projectFile->behaviorFiles.at(graphindex)->getBehaviorGraphData());
                    eventsWid->clear();
                    eventsWid->loadData(projectFile->behaviorFiles.at(graphindex)->getBehaviorGraphData());
                    topLyt->removeWidget(behaviorGraphViewGB);
                    topLyt->addWidget(behaviorGraphViewGB, 1, 0, 9, 6);
                    topLyt->addWidget(objectDataSA, 1, 6, 6, 4);
                    topLyt->addWidget(eventsWid, 7, 6, 4, 2);
                    topLyt->addWidget(variablesWid, 7, 8, 4, 2);
                    variablesWid->show();
                    eventsWid->show();
                    objectDataSA->show();
                }else{
                    (qFatal("MainWindow::changedTabs(): The tab index is out of sync with the behavior files or behavior graphs!"));
                }
            }
        }
    }else{
        (qFatal("MainWindow::changedTabs(): No project opened!"));
    }
    //dialog.setValue(//dialog.maximum());
    //dialog.done(0);
}

void MainWindow::expandBranches(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
            behaviorGraphs.at(index)->expandAllBranches();
        }else{
            (qFatal("MainWindow::expandBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
        }
    }else{
        (qWarning("MainWindow::expandBranches(): No project opened!"));
    }
}

void MainWindow::collapseBranches(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
            behaviorGraphs.at(index)->contractAllBranches();
            behaviorGraphs.at(index)->selectRoot();
        }else{
            (qFatal("MainWindow: collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
        }
    }else{
        (qWarning("MainWindow::collapseBranches(): No project opened!"));
    }
}

void MainWindow::refocus(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index != -1){
            if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
                if (!behaviorGraphs.at(index)->refocus()){
                    (qWarning("MainWindow::refocus(): No behavior graph item is currently selected!"));
                }
            }else{
                (qFatal("MainWindow::refocus() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!"));
            }
        }else{
            (qWarning("MainWindow::refocus(): No behavior graph is currently viewed!"));
        }
    }else{
        (qWarning("MainWindow::refocus(): No project opened!"));
    }
}

void MainWindow::save(){
    saveFile(getBehaviorGraphIndex(tabs->tabText(tabs->currentIndex())));
}

void MainWindow::saveFile(int index){
    if (projectFile){
        QString backupPath;
        if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
            if (projectFile->behaviorFiles.at(index)->getIsChanged()){
                ProgressDialog dialog("Saving behavior: "+projectFile->behaviorFiles.at(index)->fileName()+"...", "", 0, 100, this);
                backupPath = QDir::currentPath()+"/Backup";
                QDir dir(backupPath);
                if (!dir.exists()) {
                    dir.mkpath(backupPath);
                }
                dialog.setProgress("Creating backup file...", 10);
                QString filename = backupPath+"\\"+projectFile->behaviorFiles.at(index)->fileName().section("/",-1,-1);
                QFile backup(filename);
                if (backup.exists()){
                    if (!backup.remove()){
                        (qWarning(QString("Failed to remove to old backup file "+backupPath.section("/",-1,-1)+"!").toLocal8Bit().data()));
                    }
                }
                if (!projectFile->behaviorFiles.at(index)->copy(projectFile->behaviorFiles.at(index)->fileName(), filename)){
                    (qWarning("Backup failed!"));
                }else{
                    if (!projectFile->behaviorFiles.at(index)->remove()){
                        (qWarning(QString("Failed to remove to old file "+backupPath.section("/",-1,-1)+"!").toLocal8Bit().data()));
                    }
                    dialog.setProgress("Backup finished...", 20);
                }
                dialog.setProgress("Writing to file...", 30);
                projectFile->behaviorFiles.at(index)->write();
                projectFile->behaviorFiles.at(index)->toggleChanged(false);
                dialog.setProgress("Behavior file saved!!!", dialog.maximum());
            }
        }else{
            (qWarning("MainWindow::saveFile(): No behavior file is currently viewed!"));
        }
    }else{
        (qWarning("MainWindow::saveFile(): No project opened!"));
    }
}

void MainWindow::saveProject(){
    if (projectFile){
        ProgressDialog dialog("Saving project "+projectFile->fileName().section("/", -1, -1)+"...", "", 0, 100, this);
        if (projectFile->getIsChanged()){
            projectFile->write();
            projectFile->toggleChanged(false);
        }
        dialog.setProgress(projectFile->fileName()+" saved...", 10);
        QFile animData(lastFileSelectedPath+"/animationdatasinglefile.txt");
        if (animData.exists()){
            if (!animData.remove()){
                (qWarning("Failed to remove to old animData file!"));
            }
        }
        if (!projectFile->skyrimAnimData->write(lastFileSelectedPath+"/animationdatasinglefile.txt")){
            (qWarning("Failed to write animationdatasinglefile.txt!"));
        }
        QFile animSetData(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (animSetData.exists()){
            if (!animSetData.remove()){
                (qWarning("Failed to remove to old animSetData file!"));
            }
        }
        projectFile->skyrimAnimSetData.write(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (characterFile){
            if (characterFile->getIsChanged()){
                characterFile->write();
                characterFile->toggleChanged(false);
            }
            dialog.setProgress(characterFile->fileName().section("/", -1, -1)+" saved...", 20);
            if (skeletonFile){
                /*if (skeletonFile->getIsChanged()){
                    skeletonFile->write();
                    skeletonFile->toggleChanged(false);
                }*/
                dialog.setProgress("Saving all unsaved behavior files...", 30);
                for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
                    if (projectFile->behaviorFiles.at(i)/* && i < behaviorGraphs.size() && behaviorGraphs.at(i)*/){
                        if (projectFile->behaviorFiles.at(i)->getIsChanged()){
                            saveFile(i);
                        }
                    }else{
                        (qFatal("MainWindow::saveProject(): The tab index is out of sync with the behavior files or a NULL pointer in either the behavior or behavior graph list was encountered!!"));
                        return;
                    }
                }
                dialog.setProgress("Project saved!!!", dialog.maximum());
            }else{
                (qFatal("Project is missing a skeleton file!"));
            }
        }else{
            (qFatal("Project is missing a character file!"));
        }
    }else{
        (qWarning("No project open!"));
    }
}

void MainWindow::packAndExportProjectToSkyrimDirectory(){
    if (projectFile){
        QTime t;
        t.start();
        writeToLog("\n-------------------------\nExporting the current project to the Skyrim game directory...");
        ProgressDialog dialog("Exporting the current project to the Skyrim game directory...", "", 0, 100, this);
        QString path = skyrimDirectory+"/data/meshes/actors";
        QString projectFolder = path+"/"+lastFileSelectedPath.section("/", -1, -1);
        dialog.setProgress("Converting entire project...This may take a few seconds...", 20);
        if (hkxcmd(lastFileSelectedPath, projectFolder) != HKXCMD_SUCCESS){
            (qWarning("Failed to convert and export the files to the correct folders in the Skyrim directory!"));
            return;
        }
        exportAnimationData();
        dialog.setProgress("Project export sucessful!!", dialog.maximum());
        writeToLog("\n-------------------------\nTime taken to export the project is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    }else{
        (qWarning("No project open!"));
    }
}

void MainWindow::exportAnimationData() const{
    QFile animData(lastFileSelectedPath+"/animationdatasinglefile.txt");
    if (animData.exists()){
        QDir dir(skyrimDirectory+"/data/meshes");
        if (dir.exists()){
            if (!animData.copy(skyrimDirectory+"/data/meshes")){
                (qWarning("Failed to export animationdatasinglefile.txt to the game directory!"));
            }
        }else{
            if (QDir().mkdir(skyrimDirectory+"/data/meshes")){
                if (!animData.copy(skyrimDirectory+"/data/meshes")){
                    (qWarning("Failed to export animationdatasinglefile.txt to the game directory!"));
                }
            }else{
                (qWarning(QString("Failed to create directory: "+skyrimDirectory+"/data/meshes"+"!").toLocal8Bit().data()));
            }
        }
    }else{
        (qWarning("animationdatasinglefile.txt was not found in the project directory!"));
    }
    QFile animSetData(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
    if (animSetData.exists()){
        QDir dir(skyrimDirectory+"/data/meshes");
        if (dir.exists()){
            if (!animSetData.copy(skyrimDirectory+"/data/meshes")){
                (qWarning("Failed to export animationsetdatasinglefile.txt to the game directory!"));
            }
        }else{
            if (QDir().mkdir(skyrimDirectory+"/data/meshes")){
                if (!animSetData.copy(skyrimDirectory+"/data/meshes")){
                    (qWarning("Failed to export animationsetdatasinglefile.txt to the game directory!"));
                }
            }else{
                (qWarning(QString("Failed to create directory: "+skyrimDirectory+"/data/meshes"+"!").toLocal8Bit().data()));
            }
        }
    }else{
        (qWarning("animationsetdatasinglefile.txt was not found in the project directory!"));
    }
}

void MainWindow::packAndExportFileToSkyrimDirectory(){
    if (projectFile){
        if (characterFile){
            QTime t;
            t.start();
            writeToLog("\n-------------------------\nExporting the current file to the Skyrim game directory...");
            //ProgressDialog dialog("Exporting the current file to the Skyrim game directory...", "", 0, 100, this);
            QString path = skyrimDirectory+"/data/meshes/actors";
            QString projectFolder = path+"/"+lastFileSelectedPath.section("/", -1, -1);
            QString filename = tabs->tabText(tabs->currentIndex());
            QString temppath = projectFolder+"/"+characterFile->getBehaviorDirectoryName()+"/"+filename;
            if (hkxcmd(lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName()+"/"+filename, temppath) != HKXCMD_SUCCESS){
                //dialog.setProgress("Behavior file export failed!", dialog.maximum());
                (qWarning("Failed to convert and export the behavior directory to the correct folder in the Skyrim directory!"));
                return;
            }
            exportAnimationData();
            //dialog.setProgress("Behavior file exported sucessfully!", dialog.maximum());
            writeToLog("\n-------------------------\nTime taken to export the file is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
        }
    }else{
        (qWarning("No project open!"));
    }
}

void MainWindow::exit(){
    /*if (exitProgram()) {
        writeSettings();
    }*/
    QApplication::exit(0);
}

void MainWindow::closeTab(int index){
    objectDataWid->unloadDataWidget();
    if (projectFile){
        if (index == 0 && (projectFile->behaviorFiles.isEmpty() || (!projectFile->behaviorFiles.at(index)->getIsChanged() || closeAllDialogue() != QMessageBox::Cancel))){
            closeAll();
        }else{
            index--;
            if (index >= 0 && index < behaviorGraphs.size() && (!projectFile->behaviorFiles.at(index)->getIsChanged() || closeFileDialogue() != QMessageBox::Cancel)){
                tabs->removeTab(index + 1);
                //behaviorGraphs.move(index, behaviorGraphs.size() - 1);
            }else{
                (qFatal("MainWindow::closeTab(): The tab index is out of sync with the behavior files or behavior graphs!"));
            }
        }
    }else{
        (qWarning("No project open!"));
    }
}

QString MainWindow::generateUniqueBehaviorName(){
    if (characterFile){
        QString name = "Behavior_0.hkx";
        int num = 1;
        int index;
        QStringList behaviornames;
        QDirIterator it(lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName());
        while (it.hasNext()){
            if (QFileInfo(it.next()).fileName().contains(".hkx")){
                behaviornames.append(it.fileInfo().fileName());
            }
        }
        for (int i = 0; i < behaviornames.size(); i++){
            if (behaviornames.at(i) == name){
                index = name.lastIndexOf('_');
                if (index > -1){
                    name.remove(index + 1, name.size());
                }
                name.append(QString::number(num)+".hkx");
                num++;
                if (num > 1){
                    i = 0;
                }
            }
        }
        return lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName()+"/"+name;
    }else{
        (qWarning("MainWindow::generateUniqueBehaviorName(): Null characterFile!"));
    }
    return "";
}

void MainWindow::addNewBehavior(bool initData){
    ProgressDialog dialog("Creating new behavior file in the current project...", "", 0, 100, this);
    objectDataWid->unloadDataWidget();
    if (projectFile){
        if (characterFile){
            QString filename = generateUniqueBehaviorName();
            if (filename != ""){
                projectFile->behaviorFiles.append(new BehaviorFile(this, projectFile, characterFile, filename));
                if (initData){
                    projectFile->behaviorFiles.last()->generateDefaultCharacterData();
                }else{
                    projectFile->behaviorFiles.last()->generateNewBehavior();
                }
                dialog.setProgress("Drawing behavior graph...", 50);
                behaviorGraphs.append(new BehaviorGraphView(objectDataWid, projectFile->behaviorFiles.last()));
                tabs->addTab(behaviorGraphs.last(), filename.section("/", -1, -1));
                if (!behaviorGraphs.last()->drawGraph(static_cast<DataIconManager *>(projectFile->behaviorFiles.last()->getBehaviorGraph()))){
                    (qFatal("MainWindow::addNewBehavior(): The behavior graph was drawn incorrectly!"));
                }
                tabs->setCurrentIndex(tabs->count() - 1);
                dialog.setProgress("Writing to file...", 50);
                //???
                //projectFile->behaviorFiles.last()->toggleChanged(true);
                //saveFile(tabs->count() - 2);
                dialog.setProgress("Behavior file creation sucessful!!", dialog.maximum());
                return;
            }else{
                (qWarning("Name generation failed!"));
            }
        }else{
            (qWarning("No character file open!"));
        }
    }else{
        (qWarning("No project open!"));
    }
    dialog.setProgress("Behavior file creation failed!!", dialog.maximum());
}

void MainWindow::openProject(QString & filepath){
    if (tabs->count() > 0){
        closeAll();
    }
    lastFileSelected = filepath;
    QTime t;
    t.start();
    objectDataWid->changeCurrentDataWidget(NULL);
    int index = filepath.lastIndexOf("\\", -1);
    if (index > -1){
        lastFileSelectedPath = filepath.remove(index, filepath.size() - index);
    }else{
        index = filepath.lastIndexOf("/", -1);
        if (index > -1){
            lastFileSelectedPath = filepath.remove(index, filepath.size() - index);
        }else{
            lastFileSelectedPath = filepath;
        }
    }
    int time = t.elapsed();
    projectFile = new ProjectFile(this, lastFileSelected);
    {
        std::thread thread(&ProjectFile::readAnimationSetData, projectFile, lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (!projectFile->readAnimationData(lastFileSelectedPath+"/animationdatasinglefile.txt")){
            (qFatal("MainWindow::openProject(): The project animation data file could not be parsed!!!"));
            thread.join();
            return;
        }
        thread.join();
    }
    ProgressDialog dialog("Opening project..."+projectFile->fileName().section("/", -1, -1), "", 0, 100, this);
    if (!projectFile->parse()){
        (qFatal(QString("MainWindow::openProject(): The project file "+lastFileSelected+" could not be parsed!!!\nYou have tried to open non-project file or the project file is corrupted!").toLocal8Bit().data()));
        delete projectFile;
        projectFile = NULL;
        return;
    }
    writeToLog("\n-------------------------\nTime taken to open file \""+filepath+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    dialog.setProgress("Loading character data...", 10);
    projectUI->setFilePath(lastFileSelectedPath);
    time = t.elapsed();
    characterFile = new CharacterFile(this, lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0));
    if (!characterFile->parse()){
        (qFatal(QString("MainWindow::openProject(): The character file "+projectFile->getCharacterFilePathAt(0)+" could not be parsed!!!").toLocal8Bit().data()));
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        return;
    }
    dialog.setProgress("Character data loaded sucessfully!!!", 40);
    projectFile->setCharacterFile(characterFile);
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0)+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    time = t.elapsed();
    dialog.setProgress("Loading skeleton data...", 50);
    skeletonFile = new SkeletonFile(this, lastFileSelectedPath+"/"+characterFile->getRigName());
    if (!skeletonFile->parse()){
        (qFatal(QString("MainWindow::openProject(): The skeleton file "+characterFile->getRigName()+" could not be parsed!!!").toLocal8Bit().data()));
        delete projectFile;
        projectFile = NULL;
        delete characterFile;
        characterFile = NULL;
        delete skeletonFile;
        skeletonFile = NULL;
        return;
    }
    dialog.setProgress("Skeleton data loaded sucessfully!!!", 90);
    characterFile->setSkeletonFile(skeletonFile);
    tabs->addTab(projectUI, "Character Data");
    projectUI->setProject(projectFile);
    projectUI->loadData();
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+characterFile->getRigName()+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    projectUI->setDisabled(false);
    std::vector <std::thread> threads;
    QDirIterator it(lastFileSelectedPath+"/"+characterFile->getBehaviorDirectoryName());
    QString behavior;
    QStringList behaviornames;
    while (it.hasNext()){
        behavior = it.next();
        if (behavior.contains(".hkx")){
            behaviornames.append(behavior);
        }
    }
    //This also reads files that may not belong to the current project! See dog!!
    //Read files...
    for (int i = 0; i < behaviornames.size();){
        for (uint j = 0; j < std::thread::hardware_concurrency(); j++){
            if (threads.size() < std::thread::hardware_concurrency()){
                if (i < behaviornames.size()){
                    threads.push_back(std::thread(&MainWindow::openBehavior, this, behaviornames.at(i), false));
                    i++;
                }else{
                    break;
                }
            }else{
                for (int k = 0; k < threads.size(); k++){
                    if (threads.at(k).joinable()){
                        threads.at(k).join();
                    }else{
                        (qFatal(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data()));
                    }
                }
                threads.clear();
            }
        }
    }
    for (int k = 0; k < threads.size(); k++){
        if (threads.at(k).joinable()){
            threads.at(k).join();
        }else{
            (qFatal(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data()));
        }
    }
    threads.clear();
    //Draw graphs...
    for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
        behaviorGraphs.append(new BehaviorGraphView(objectDataWid, projectFile->behaviorFiles.at(i)));
        //tabs->addTab(behaviorGraphs.last(), QString(behaviornames.at(i)).section("/", -1, -1));
    }
    for (int i = 0; i < behaviornames.size();){
        for (uint j = 0; j < std::thread::hardware_concurrency(); j++){
            if (threads.size() < std::thread::hardware_concurrency()){
                if (i < behaviornames.size() && i < behaviorGraphs.size()){
                    threads.push_back(std::thread(&TreeGraphicsView::drawGraph, behaviorGraphs[i], static_cast<DataIconManager *>(projectFile->behaviorFiles.at(i)->getBehaviorGraph()), false));
                    i++;
                }else{
                    break;
                }
            }else{
                for (int k = 0; k < threads.size(); k++){
                    if (threads.at(k).joinable()){
                        threads.at(k).join();
                    }else{
                        (qFatal(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data()));
                    }
                }
                threads.clear();
            }
        }
    }
    for (int k = 0; k < threads.size(); k++){
        if (threads.at(k).joinable()){
            threads.at(k).join();
        }else{
            (qFatal(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data()));
        }
    }
    threads.clear();
    writeToLog("\n-------------------------\nTime taken to open project \""+filepath+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    dialog.setProgress("Project loaded sucessfully!!!", dialog.maximum());
    //changedTabs(0);
}

bool MainWindow::closeAll(){
    if (projectFile){
        bool unsavedChanges = false;
        for (int i = 0; i < behaviorGraphs.size(); i++){
            if (projectFile->behaviorFiles.at(i)->getIsChanged()){
                unsavedChanges = true;
            }
        }
        if (characterFile->getIsChanged()){
            unsavedChanges = true;
        }
        if (!unsavedChanges || closeAllDialogue() != QMessageBox::Cancel){
            projectUI->setDisabled(true);
            objectDataWid->unloadDataWidget();
            tabs->clear();
            for (int j = 0; j < behaviorGraphs.size(); j++){
                if (behaviorGraphs.at(j)){
                    delete behaviorGraphs.at(j);
                }
            }
            behaviorGraphs.clear();
            for (int j = 0; j < projectFile->behaviorFiles.size(); j++){
                if (projectFile->behaviorFiles.at(j)){
                    delete projectFile->behaviorFiles.at(j);
                }
            }
            projectFile->behaviorFiles.clear();
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
    }else{
        return true;
    }
}

void MainWindow::writeToLog(const QString &message, bool isError){
    QString str;
    QString temp = "\n#########################################";
    if (!isError){
        //str = "WARNING!!!-->";
    }else{
        str = "ERROR!!!-->";
    }
    debugLog->appendPlainText(temp+"\n"+str+message+temp);
    QTextCursor tempTC(debugLog->document());
    tempTC.movePosition(QTextCursor::End);
    debugLog->setTextCursor(tempTC);
}

void MainWindow::openPackedProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx project file..."), lastFileSelected, tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    if (hkxcmd(filename.section("/", 0, -2), "", "-v:xml") != HKXCMD_SUCCESS){
        (qWarning("Failed to convert and export the files to the correct folders in the Skyrim directory!"));
        return;
    }
    //remove all hkx files then rename the rest to hkx files...
    {
        QDirIterator it(filename.section("/", 0, -2), QDirIterator::Subdirectories);
        QString temp;
        while (it.hasNext()){
            temp = it.next();
            if (temp.contains(".hkx")){
                if (!QDir(temp).remove(temp)){
                    (qFatal(QString("MainWindow::openPackedProject(): The file "+temp+" could not be removed!!!").toLocal8Bit().data()));
                    return;
                }
            }
        }
    }
    {
        QDirIterator it(filename.section("/", 0, -2), QDirIterator::Subdirectories);
        QString temp;
        while (it.hasNext()){
            temp = it.next();
            if (temp.contains("-out.xml")){
                if (!QDir(temp).rename(temp, QString(temp).replace("-out.xml", ".hkx"))){
                    (qFatal(QString("MainWindow::openPackedProject(): The file "+temp+" could not be renamed!!!").toLocal8Bit().data()));
                    return;
                }
            }else if (temp.contains(".xml")){
                if (!QDir(temp).rename(temp, QString(temp).replace(".xml", ".hkx"))){
                    (qFatal(QString("MainWindow::openPackedProject(): The file "+temp+" could not be renamed!!!").toLocal8Bit().data()));
                    return;
                }
            }
        }
    }
    openProject(filename);
}

void MainWindow::openUnpackedProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx xml project file..."), lastFileSelected, tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    openProject(filename);
}

bool MainWindow::openBehavior(const QString & filename, bool checkisopen){
    if (filename != ""){
        if (projectFile){
            if (checkisopen){
                for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
                    if (projectFile->behaviorFiles.at(i)->fileName() == filename){
                        (qWarning("MainWindow::openBehavior(): The selected behavior file is already open!"));
                        return true;
                    }
                }
            }else{
                objectDataWid->changeCurrentDataWidget(NULL);
            }
            std::mutex mu;
            BehaviorFile *ptr = new BehaviorFile(this, projectFile, characterFile, filename);
            mu.lock();
            projectFile->behaviorFiles.append(ptr);
            ptr = projectFile->behaviorFiles.last();
            mu.unlock();
            if (!ptr->parse()){
                mu.lock();
                delete projectFile->behaviorFiles.last();
                projectFile->behaviorFiles.removeLast();
                mu.unlock();
                return false;
            }
            return true;
        }else{
            (qFatal("MainWindow::openBehavior(): No project is opened!"));
        }
    }else{
        (qFatal("MainWindow::openBehavior(): Filename is empty!"));
    }
    return false;
}

void MainWindow::openBehaviorFile(const QModelIndex & index){
    QString fileName = index.data().toString();
    for (int j = 0; j < behaviorGraphs.size(); j++){
        if (fileName.compare(behaviorGraphs.at(j)->getBehaviorFilename().section("/", -1, -1), Qt::CaseInsensitive) == 0){
            objectDataWid->loadBehaviorView(behaviorGraphs.at(j));
            tabs->addTab(behaviorGraphs.at(j), fileName.section("/", -1, -1));
            tabs->setCurrentIndex(tabs->count() - 1);
            return;
        }
    }
    (qFatal("MainWindow::openBehaviorFile(): The selected behavior file was not found!"));
}

int MainWindow::getBehaviorGraphIndex(const QString & filename) const{
    for (int j = 0; j < behaviorGraphs.size(); j++){
        if (filename.compare(behaviorGraphs.at(j)->getBehaviorFilename().section("/", -1, -1), Qt::CaseInsensitive) == 0){
            if (j >= projectFile->behaviorFiles.size() || filename.compare(projectFile->behaviorFiles.at(j)->fileName().section("/", -1, -1), Qt::CaseInsensitive) != 0){
                (qFatal("MainWindow::getBehaviorGraphIndex(): The index is invalid!"));
            }
            return j;
        }
    }
    return -1;
}

void MainWindow::readSettings(){
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    QPoint pos = settings.value("pos", QPoint(500, 500)).toPoint();
    QSize size = settings.value("size", QSize(900, 900)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings(){
    QSettings settings("QtProject", "Skyrim Behavior Editor");
    settings.beginGroup("Main Window");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool MainWindow::findSkyrimDirectory(){
    QFileInfoList drives = QDir::drives();
    QString driveName;
    QString path;
    QDir dir;
    for (int i = 0; i < drives.size(); i++){
        driveName = drives.at(i).absolutePath();
        dir.setPath(driveName);
        path = driveName+"Program Files/Steam/steamapps/common/Skyrim";
        if (dir.exists(path)){
            skyrimDirectory = path;
            return true;
        }else{
            path = driveName+"Steam/steamapps/common/Skyrim";
            if (dir.exists(path)){
                skyrimDirectory = path;
                return true;
            }else{
                path = driveName+"Steam Games/steamapps/common/Skyrim";
                if (dir.exists(path)){
                    skyrimDirectory = path;
                    return true;
                }
            }
        }
    }
    return false;
}

MainWindow::HKXCMD_RETURN MainWindow::hkxcmd(const QString &filepath, const QString &outputDirectory, const QString &flags){
    QString command;
    command = "\""+hkxcmdPath+"\" convert \""+filepath+"\" \""+outputDirectory+"\" "+flags;
    command.replace("/", "\\");
    //command = "cmd /c "+command;
    QProcess processHKXCMD;
    HKXCMD_RETURN value = (HKXCMD_RETURN)processHKXCMD.execute(command);
    if (value != HKXCMD_SUCCESS){
        writeToLog("MainWindow: packHKX() failed!\nThe command \""+command+"\" failed!", true);
    }
    return value;
}

bool MainWindow::exitProgram(){
    QMessageBox::StandardButton ret;
    if (closeAll()){
        ret = QMessageBox::warning(this, tr("Application"), "Close the application?", QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok){
            return true;
        }else if (ret == QMessageBox::Cancel){
            return false;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event){
    writeSettings();
    if (exitProgram()) {
        writeSettings();
        event->accept();
    }else {
        event->ignore();
    }
}

void MainWindow::paintEvent(QPaintEvent *){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
