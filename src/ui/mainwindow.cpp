#include "mainwindow.h"
#include "src/filetypes/animationfile.h"
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
#include "src/ui/hkxclassesui/behaviorui/animationsui.h"
#include "src/ui/hkxclassesui/animationcacheui.h"

#include <QtWidgets>

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
      newProjectA(new QAction("Create New Project", this)),
      saveA(new QAction("Save Viewed Behavior File", this)),
      saveProjectA(new QAction("Save Project", this)),
      exitA(new QAction("Exit", this)),
      viewM(new QMenu("View", this)),
      expandA(new QAction("Expand All", this)),
      collapseA(new QAction("Collapse All", this)),
      viewAnimationCacheA(new QAction("View Animation Cache", this)),
      refocusA(new QAction("Refocus On Selected Item", this)),
      settingsM(new QMenu("Settings", this)),
      mergeM(new QMenu("Merge", this)),
      mergeBehaviorsA(new QAction("Merge Two Behavior Files", this)),
      mergeProjectsA(new QAction("Merge Two Projects", this)),
      setPathToGameFolderA(new QAction("Set Path To Game Directory", this)),
      setGameModeA(new QAction("Set Game Mode", this)),
      tabs(new QTabWidget),
      projectFile(nullptr),
      characterFile(nullptr),
      skeletonFile(nullptr),
      projectUI(new ProjectUI(nullptr)),
      iconGBLyt(new QVBoxLayout(this)),
      behaviorGraphViewGB(new QGroupBox("Behavior Graph")),
      objectDataSA(new QScrollArea),
      eventsWid(new EventsUI("Events")),
      variablesWid(new BehaviorVariablesUI("Behavior Variables")),
      objectDataWid(new HkDataUI("Object Data")),
      logGB(new QGroupBox("Debug Log")),
      logGBLyt(new QVBoxLayout(this)),
      lastFileSelected("C:/"),
      animationCacheUI(nullptr)
      //animationCacheSA(new QScrollArea)
{
    //setStyleSheet("QComboBox {background: yellow};QWidget {background: darkGray}");
    projectUI->setDisabled(true);
    hkxcmdPath = QDir::currentPath()+"/hkxcmd.exe";
#ifdef QT_DEBUG
    hkxcmdPath = "c:/users/wayne/desktop/hkxcmd.exe";
#endif
    animationCacheUI = new AnimationCacheUI();
    animationCacheUI->setWindowTitle("Project Animation Cache Data!");
    animationCacheUI->setVisible(false);

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
    fileM->addAction(newProjectA);
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
    setGameModeA->setCheckable(true);
    settingsM->addAction(setPathToGameFolderA);
    settingsM->addAction(setGameModeA);
    mergeM->addAction(mergeBehaviorsA);
    mergeM->addAction(mergeProjectsA);
    viewM->addAction(expandA);
    viewM->addAction(collapseA);
    viewM->addAction(refocusA);
    viewM->addAction(viewAnimationCacheA);
    topMB->setMaximumHeight(50);
    topMB->addMenu(fileM);
    topMB->addMenu(viewM);
    topMB->addMenu(settingsM);
    topMB->addMenu(mergeM);
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
    objectDataSA->setVisible(false);
    //animationCacheSA->setWidget(animationCacheUI);
    //animationCacheSA->setVisible(false);
    //animationCacheSA->setWindowFlags();
    //objectDataSA->setStyleSheet("QScrollArea {background-color:cyan}");
    //variablesWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    //eventsWid->setMaximumSize(size().width()*0.4, size().height()*0.25);
    logGB->setMaximumSize(size().width()*0.6, size().height()*0.1);
    if (!findGameDirectory(QString("Skyrim"), skyrimDirectory)){
        writeToLog("The TESV executable was not found!");
    }
    if (!findGameDirectory(QString("Skyrim Special Edition"), skyrimSpecialEdtionDirectory)){
        writeToLog("The SSE executable was not found!");
    }else{
        skyrimBehaviorUpdateToolFullPath = skyrimSpecialEdtionDirectory+"/Tools/HavokBehaviorPostProcess/HavokBehaviorPostProcess.exe";
        if (!QFile(skyrimBehaviorUpdateToolFullPath).exists()){
            WARNING_MESSAGE(QString("The tool for the conversion of Havok 32bit to Havok 64bit \"HavokBehaviorPostProcess.exe\" was not found!\n\nTo obtain it, you need to download the Creation Kit for Skyrim Special Edition!"));
        }
    }
    connect(newProjectA, SIGNAL(triggered(bool)), this, SLOT(createNewProject()), Qt::UniqueConnection);
    connect(openPackedProjectA, SIGNAL(triggered(bool)), this, SLOT(openPackedProject()), Qt::UniqueConnection);
    connect(openUnpackedProjectA, SIGNAL(triggered(bool)), this, SLOT(openUnpackedProject()), Qt::UniqueConnection);
    connect(saveA, SIGNAL(triggered(bool)), this, SLOT(save()), Qt::UniqueConnection);
    connect(exportToSkyrimDirA, SIGNAL(triggered(bool)), this, SLOT(packAndExportProjectToSkyrimDirectory()), Qt::UniqueConnection);
    connect(exportCurrentFileA, SIGNAL(triggered(bool)), this, SLOT(packAndExportFileToSkyrimDirectory()), Qt::UniqueConnection);
    connect(saveProjectA, SIGNAL(triggered(bool)), this, SLOT(saveProject()), Qt::UniqueConnection);
    connect(expandA, SIGNAL(triggered(bool)), this, SLOT(expandBranches()), Qt::UniqueConnection);
    connect(collapseA, SIGNAL(triggered(bool)), this, SLOT(collapseBranches()), Qt::UniqueConnection);
    connect(viewAnimationCacheA, SIGNAL(triggered(bool)), this, SLOT(viewAnimationCache()), Qt::UniqueConnection);
    connect(refocusA, SIGNAL(triggered(bool)), this, SLOT(refocus()), Qt::UniqueConnection);
    connect(exitA, SIGNAL(triggered(bool)), this, SLOT(exit()), Qt::UniqueConnection);
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(changedTabs(int)), Qt::UniqueConnection);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(openFile(QModelIndex)), this, SLOT(openBehaviorFile(QModelIndex)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(addBehavior(bool)), this, SLOT(addNewBehavior(bool)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(openAnimation(QString)), this, SLOT(openAnimationFile(QString)), Qt::UniqueConnection);
    connect(projectUI, SIGNAL(animationRemoved(int)), this, SLOT(removeAnimation(int)), Qt::UniqueConnection);
    connect(setGameModeA, SIGNAL(changed()), this, SLOT(setGameMode()), Qt::UniqueConnection);
    connect(setPathToGameFolderA, SIGNAL(toggled(bool)), this, SLOT(setPathToGameDirectory()), Qt::UniqueConnection);
    connect(mergeBehaviorsA, SIGNAL(triggered(bool)), this, SLOT(mergeBehaviors()), Qt::UniqueConnection);
    connect(mergeProjectsA, SIGNAL(triggered(bool)), this, SLOT(mergeProjects()), Qt::UniqueConnection);
}

MainWindow::~MainWindow(){
    //
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
                if (graphindex < projectFile->behaviorFiles.size() && graphindex < behaviorGraphs.size() && graphindex > -1){
                    objectDataWid->changeCurrentDataWidget(nullptr);
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
                    FATAL_RUNTIME_ERROR("MainWindow::changedTabs(): The tab index is out of sync with the behavior files or behavior graphs!");
                }
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("MainWindow::changedTabs(): No project opened!");
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
            if (tabs->count() < 2){
                WARNING_MESSAGE("No behavior file is currently being viewed!!!")
            }else{
                FATAL_RUNTIME_ERROR("MainWindow::expandBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!");
            }
        }
    }else{
        writeToLog("MainWindow::expandBranches(): No project opened!");
    }
}

void MainWindow::collapseBranches(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
            behaviorGraphs.at(index)->contractAllBranches();
            behaviorGraphs.at(index)->selectRoot();
        }else{
            if (tabs->count() < 2){
                WARNING_MESSAGE("No behavior file is currently being viewed!!!")
            }else{
                FATAL_RUNTIME_ERROR("MainWindow::collapseBranches() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!");
            }
        }
    }else{
        writeToLog("MainWindow::collapseBranches(): No project opened!");
    }
}

void MainWindow::refocus(){
    if (projectFile){
        int index = tabs->currentIndex() - 1;
        if (index != -1){
            if (index >= 0 && index < projectFile->behaviorFiles.size() && index < behaviorGraphs.size()){
                if (!behaviorGraphs.at(index)->refocus()){
                    writeToLog("MainWindow::refocus(): No behavior graph item is currently selected!");
                }
            }else{
                if (tabs->count() < 2){
                    WARNING_MESSAGE("No behavior file is currently being viewed!!!")
                }else{
                    FATAL_RUNTIME_ERROR("MainWindow::refocus() failed!\nThe tab index is out of sync with the behavior files or behavior graphs!");
                }
            }
        }else{
            writeToLog("MainWindow::refocus(): No behavior graph is currently viewed!");
        }
    }else{
        writeToLog("MainWindow::refocus(): No project opened!");
    }
}

void MainWindow::setGameMode(){
    //
    //
    //
    //
}

void MainWindow::setPathToGameDirectory(){
    bool ok;
    QString path = QInputDialog::getText(this, tr("Set path to game directory!"), tr("Path:"), QLineEdit::Normal, skyrimDirectory, &ok);
    if (ok && path != ""){
        skyrimDirectory = path;
    }
}

void MainWindow::save(){
    saveFile(getBehaviorGraphIndex(tabs->tabText(tabs->currentIndex())));
}

void MainWindow::viewAnimationCache(){
    if (projectFile){
        animationCacheUI->loadData(projectFile);
        animationCacheUI->setVisible(true);
        //animationCacheSA->setVisible(true);
    }else{
        writeToLog("MainWindow::viewAnimationCache(): No project opened!");
    }
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
                        writeToLog(QString("Failed to remove to old backup file "+backupPath.section("/",-1,-1)+"!").toLocal8Bit().data());
                    }
                }
                if (!projectFile->behaviorFiles.at(index)->copy(projectFile->behaviorFiles.at(index)->fileName(), filename)){
                    writeToLog("Backup failed!");
                }else{
                    if (!projectFile->behaviorFiles.at(index)->remove()){
                        writeToLog(QString("Failed to remove to old file "+backupPath.section("/",-1,-1)+"!").toLocal8Bit().data());
                    }
                    dialog.setProgress("Backup finished...", 20);
                }
                dialog.setProgress("Writing to file...", 30);
                projectFile->behaviorFiles.at(index)->write();
                projectFile->behaviorFiles.at(index)->setIsChanged(false);
                dialog.setProgress("Behavior file saved!!!", dialog.maximum());
            }
        }else{
            writeToLog("MainWindow::saveFile(): No behavior file is currently viewed!");
        }
    }else{
        writeToLog("MainWindow::saveFile(): No project opened!");
    }
}

void MainWindow::saveProject(){
    if (projectFile){
        ProgressDialog dialog("Saving project "+projectFile->fileName().section("/", -1, -1)+"...", "", 0, 100, this);
        if (projectFile->getIsChanged()){
            projectFile->write();
            projectFile->setIsChanged(false);
        }
        dialog.setProgress(projectFile->fileName()+" saved...", 10);
        QFile animData(lastFileSelectedPath+"/animationdatasinglefile.txt");
        if (animData.exists()){
            if (!animData.remove()){
                writeToLog("Failed to remove to old animData file!");
            }
        }
        if (!projectFile->skyrimAnimData->write(lastFileSelectedPath+"/animationdatasinglefile.txt")){
            writeToLog("Failed to write animationdatasinglefile.txt!");
        }
        QFile animSetData(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (animSetData.exists()){
            if (!animSetData.remove()){
                writeToLog("Failed to remove to old animSetData file!");
            }
        }
        projectFile->skyrimAnimSetData->write(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (characterFile){
            if (characterFile->getIsChanged()){
                characterFile->write();
                characterFile->setIsChanged(false);
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
                        FATAL_RUNTIME_ERROR("MainWindow::saveProject(): The tab index is out of sync with the behavior files or a nullptr pointer in either the behavior or behavior graph list was encountered!!");
                        return;
                    }
                }
                dialog.setProgress("Project saved!!!", dialog.maximum());
            }else{
                FATAL_RUNTIME_ERROR("Project is missing a skeleton file!");
            }
        }else{
            FATAL_RUNTIME_ERROR("Project is missing a character file!");
        }
    }else{
        writeToLog("No project open!");
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
            writeToLog("Failed to convert and export the files to the correct folders in the Skyrim directory!");
            return;
        }
        exportAnimationData();
        dialog.setProgress("Project export sucessful!!", dialog.maximum());
        writeToLog("\n-------------------------\nTime taken to export the project is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    }else{
        writeToLog("No project open!");
    }
}

void MainWindow::exportAnimationData(){
    QFile animData(lastFileSelectedPath+"/animationdatasinglefile.txt");
    if (animData.exists()){
        QDir dir(skyrimDirectory+"/data/meshes");
        if (dir.exists()){
            if (!animData.copy(skyrimDirectory+"/data/meshes/animationdatasinglefile.txt")){
                writeToLog("Failed to export animationdatasinglefile.txt to the game directory!");
            }
        }else{
            if (QDir().mkdir(skyrimDirectory+"/data/meshes")){
                if (!animData.copy(skyrimDirectory+"/data/meshes/animationdatasinglefile.txt")){
                    writeToLog("Failed to export animationdatasinglefile.txt to the game directory!");
                }
            }else{
                writeToLog(QString("Failed to create directory: "+skyrimDirectory+"/data/meshes"+"!").toLocal8Bit().data());
            }
        }
    }else{
        writeToLog("animationdatasinglefile.txt was not found in the project directory!");
    }
    QFile animSetData(lastFileSelectedPath+"/animationsetdatasinglefile.txt");
    if (animSetData.exists()){
        QDir dir(skyrimDirectory+"/data/meshes");
        if (dir.exists()){
            if (!animSetData.copy(skyrimDirectory+"/data/meshes/animationsetdatasinglefile.txt")){
                writeToLog("Failed to export animationsetdatasinglefile.txt to the game directory!");
            }
        }else{
            if (QDir().mkdir(skyrimDirectory+"/data/meshes")){
                if (!animSetData.copy(skyrimDirectory+"/data/meshes/animationsetdatasinglefile.txt")){
                    writeToLog("Failed to export animationsetdatasinglefile.txt to the game directory!");
                }
            }else{
                writeToLog(QString("Failed to create directory: "+skyrimDirectory+"/data/meshes"+"!").toLocal8Bit().data());
            }
        }
    }else{
        writeToLog("animationsetdatasinglefile.txt was not found in the project directory!");
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
                writeToLog("Failed to convert and export the behavior directory to the correct folder in the Skyrim directory!");
                return;
            }
            exportAnimationData();
            //dialog.setProgress("Behavior file exported sucessfully!", dialog.maximum());
            writeToLog("\n-------------------------\nTime taken to export the file is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
        }
    }else{
        writeToLog("No project open!");
    }
}

void MainWindow::mergeBehaviors(){
    //
    //
    //
    //
}

void MainWindow::mergeProjects(){
    ProjectFile *dominantProject;
    ProjectFile *recessiveProject;
    QString recessivefilename;
    QString dominantfilename = QFileDialog::getOpenFileName(this, tr("Select dominant project file..."), QDir::currentPath(), tr("hkx Files (*.hkx)"));
    if (dominantfilename != ""){
        openProject(dominantfilename, false);
        dominantProject = projectFile;
        recessivefilename  = QFileDialog::getOpenFileName(this, tr("Select recessive project file..."), QDir::currentPath(), tr("hkx Files (*.hkx)"));
        openProject(dominantfilename, false);
        recessiveProject = projectFile;
        if (!dominantProject->merge(recessiveProject)){
            WARNING_MESSAGE("The attempt to merge projects failed!");
        }
    }
    //
    //
    //
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
                FATAL_RUNTIME_ERROR("MainWindow::closeTab(): The tab index is out of sync with the behavior files or behavior graphs!");
            }
        }
    }else{
        writeToLog("No project open!");
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
        writeToLog("MainWindow::generateUniqueBehaviorName(): Null characterFile!");
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
                    FATAL_RUNTIME_ERROR("MainWindow::addNewBehavior(): The behavior graph was drawn incorrectly!");
                }
                tabs->setCurrentIndex(tabs->count() - 1);
                dialog.setProgress("Writing to file...", 50);
                projectFile->behaviorFiles.last()->setIsChanged(true);
                save();
                //???
                //projectFile->behaviorFiles.last()->toggleChanged(true);
                //saveFile(tabs->count() - 2);
                dialog.setProgress("Behavior file creation sucessful!!", dialog.maximum());
                return;
            }else{
                writeToLog("Name generation failed!");
            }
        }else{
            writeToLog("No character file open!");
        }
    }else{
        writeToLog("No project open!");
    }
    dialog.setProgress("Behavior file creation failed!!", dialog.maximum());
}

void MainWindow::openProject(QString & filepath, bool loadUI){
    if (tabs->count() > 0){
        closeAll();
    }
    lastFileSelected = filepath;
    QTime t;
    t.start();
    objectDataWid->changeCurrentDataWidget(nullptr);
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
    //Get behavior filenames...
    QDirIterator it(lastFileSelectedPath+"/behaviors"); //Wont work for dogs, wolves!!!
    QString behavior;
    QStringList behaviornames;
    QStringList behaviornamesanimdata;
    while (it.hasNext()){
        behavior = it.next();
        if (behavior.contains(".hkx")){
            behaviornames.append(behavior);
            behaviornamesanimdata.append(behavior.section("/", -2, -1).replace("/", "\\"));
        }
    }
    projectFile = new ProjectFile(this, lastFileSelected);
    {
        std::thread thread(&ProjectFile::readAnimationSetData, projectFile, lastFileSelectedPath+"/animationsetdatasinglefile.txt");
        if (!projectFile->readAnimationData(lastFileSelectedPath+"/animationdatasinglefile.txt", behaviornamesanimdata)){
            FATAL_RUNTIME_ERROR("MainWindow::openProject(): The project animation data file could not be parsed!!!");
        }
        thread.join();
    }
    ProgressDialog dialog("Opening project..."+projectFile->fileName().section("/", -1, -1), "", 0, 100, this);
    if (!projectFile->parse()){
        FATAL_RUNTIME_ERROR(QString("MainWindow::openProject(): The project file "+lastFileSelected+" could not be parsed!!!\nYou have tried to open non-project file or the project file is corrupted!").toLocal8Bit().data());
    }
    writeToLog("\n-------------------------\nTime taken to open file \""+filepath+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    dialog.setProgress("Loading character data...", 10);
    if (loadUI){
        projectUI->setFilePath(lastFileSelectedPath);
    }
    time = t.elapsed();
    characterFile = new CharacterFile(this, projectFile, lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0));
    if (!characterFile->parse()){
        FATAL_RUNTIME_ERROR(QString("MainWindow::openProject(): The character file "+projectFile->getCharacterFilePathAt(0)+" could not be parsed!!!").toLocal8Bit().data());
    }
    dialog.setProgress("Character data loaded sucessfully!!!", 40);
    projectFile->setCharacterFile(characterFile);
    projectFile->loadEncryptedAnimationNames();
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+projectFile->getCharacterFilePathAt(0)+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    time = t.elapsed();
    dialog.setProgress("Loading skeleton data...", 50);
    skeletonFile = new SkeletonFile(this, lastFileSelectedPath+"/"+characterFile->getRigName());
    if (!skeletonFile->parse()){
        FATAL_RUNTIME_ERROR(QString("MainWindow::openProject(): The skeleton file "+characterFile->getRigName()+" could not be parsed!!!").toLocal8Bit().data());
    }
    dialog.setProgress("Skeleton data loaded sucessfully!!!", 90);
    characterFile->setSkeletonFile(skeletonFile);
    if (loadUI){
        tabs->addTab(projectUI, "Character Data");
        projectUI->setProject(projectFile);
        projectUI->loadData();
        projectUI->setDisabled(false);
    }
    writeToLog("\n-------------------------\nTime taken to open file \""+lastFileSelectedPath+"/"+characterFile->getRigName()+
                               "\" is approximately "+QString::number(t.elapsed() - time)+" milliseconds\n-------------------------\n");
    //Start reading behavior files...
    //This also reads files that may not belong to the current project! See dog!!
    std::vector <std::thread> threads;
    std::unique_lock<std::mutex> locker(mutex);
    int taskCount = behaviornames.size();
    int previousCount = taskCount;
    int behaviorIndex = 0;
    //Read files...
    for (uint i = 0; i < std::thread::hardware_concurrency(), behaviorIndex < behaviornames.size(); i++, behaviorIndex++){
        threads.push_back(std::thread(&MainWindow::openBehavior, this, behaviornames.at(behaviorIndex), std::ref(taskCount), false));
        threads.back().detach();
    }
    while (taskCount > 0){
        if (taskCount < previousCount){
            previousCount = taskCount;
            if (behaviorIndex < behaviornames.size()){
                threads.push_back(std::thread(&MainWindow::openBehavior, this, behaviornames.at(behaviorIndex), std::ref(taskCount), false));
                threads.back().detach();
                behaviorIndex++;
            }
        }
        conditionVar.wait(locker);
    }
    threads.clear();
    if (loadUI){
        for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
            behaviorGraphs.append(new BehaviorGraphView(objectDataWid, projectFile->behaviorFiles.at(i)));
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
                            FATAL_RUNTIME_ERROR(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data());
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
                FATAL_RUNTIME_ERROR(QString("MainWindow::openProject(): Thread "+QString::number(k)+" failed to join!!!").toLocal8Bit().data());
            }
        }
        threads.clear();
    }
    projectFile->generateAnimClipDataForProject();
    writeToLog("\n-------------------------\nTime taken to open project \""+filepath+
                              "\" is approximately "+QString::number(t.elapsed())+" milliseconds\n-------------------------\n");
    dialog.setProgress("Project loaded sucessfully!!!", dialog.maximum());
    //changedTabs(0);
}

bool MainWindow::openBehavior(const QString & filename, int & taskCount, bool checkisopen){
    bool result = false;
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    if (filename != ""){
        if (projectFile){
            if (checkisopen){
                for (int i = 0; i < projectFile->behaviorFiles.size(); i++){
                    if (projectFile->behaviorFiles.at(i)->fileName() == filename){
                        writeToLog("MainWindow::openBehavior(): The selected behavior file is already open!");
                        lock.lock();
                        taskCount--;
                        lock.unlock();
                        conditionVar.notify_one();
                        return true;
                    }
                }
            }else{
                //lock.lock();
                objectDataWid->changeCurrentDataWidget(nullptr);
                //lock.unlock();
            }
            BehaviorFile *ptr = new BehaviorFile(this, projectFile, characterFile, filename);
            lock.lock();
            projectFile->behaviorFiles.append(ptr);
            ptr = projectFile->behaviorFiles.last();
            lock.unlock();
            if (!ptr->parse()){
                lock.lock();
                delete projectFile->behaviorFiles.last();
                projectFile->behaviorFiles.removeLast();
                lock.unlock();
            }else{
                result = true;
            }
        }else{
            writeToLog("MainWindow::openBehavior(): No project is opened!");
        }
    }else{
        writeToLog("MainWindow::openBehavior(): Filename is empty!");
    }
    lock.lock();
    taskCount--;
    lock.unlock();
    conditionVar.notify_one();
    return result;
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
                projectFile = nullptr;
            }
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}

void MainWindow::removeBehaviorGraphs(const QStringList & filenames){
    for (auto i = 0; i < behaviorGraphs.size(); i++){
        for (auto j = 0; j < filenames.size(); j++){
            if (behaviorGraphs.at(i)->behavior->fileName() == filenames.at(j)){
                behaviorGraphs.at(i)->deleteAllObjectBranches();
                delete behaviorGraphs.at(i);
                behaviorGraphs.removeAt(i);
                projectFile->deleteBehaviorFile(filenames.at(j));
                i = 0;
                j = filenames.size();
            }
        }
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
    //std::unique_lock<std::mutex> lock(std::mutex);
    debugLog->appendPlainText(temp+"\n"+str+message+temp);
    QTextCursor tempTC(debugLog->document());
    tempTC.movePosition(QTextCursor::End);
    debugLog->setTextCursor(tempTC);
    //lock.unlock();
}

void MainWindow::openPackedProject(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx project file..."), lastFileSelected, tr("hkx Files (*.hkx)"));
    if (filename == ""){
        return;
    }
    if (hkxcmd(filename.section("/", 0, -2), "", "-v:xml") != HKXCMD_SUCCESS){
        writeToLog("Failed to convert and export the files to the correct folders in the Skyrim directory!");
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
                    FATAL_RUNTIME_ERROR(QString("MainWindow::openPackedProject(): The file "+temp+" could not be removed!!!").toLocal8Bit().data());
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
                    FATAL_RUNTIME_ERROR(QString("MainWindow::openPackedProject(): The file "+temp+" could not be renamed!!!").toLocal8Bit().data());
                    return;
                }
            }else if (temp.contains(".xml")){
                if (!QDir(temp).rename(temp, QString(temp).replace(".xml", ".hkx"))){
                    FATAL_RUNTIME_ERROR(QString("MainWindow::openPackedProject(): The file "+temp+" could not be renamed!!!").toLocal8Bit().data());
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
    FATAL_RUNTIME_ERROR("MainWindow::openBehaviorFile(): The selected behavior file was not found!");
}

void MainWindow::openAnimationFile(const QString &animationname){
    if (hkxcmd(animationname, animationname, "-v:xml") != HKXCMD_SUCCESS){
        writeToLog("Failed to convert the animation file!");
        return;
    }
    AnimationFile *ptr = new AnimationFile(this, QString(animationname).replace(".hkx", "-out.hkx"));
    if (!ptr->parse()){
        FATAL_RUNTIME_ERROR("MainWindow::openAnimationFile(): The selected animation file was not opened!");
    }
    projectFile->setAnimationIndexDuration(-1, characterFile->getNumberOfAnimations() - 1, ptr->getDuration());
    projectFile->addEncryptedAnimationName(animationname);
    ptr->remove();
    delete ptr;
}

void MainWindow::removeAnimation(int index){
    projectFile->removeEncryptedAnimationName(index);
}

int MainWindow::getBehaviorGraphIndex(const QString & filename){
    for (int j = 0; j < behaviorGraphs.size(); j++){
        if (filename.compare(behaviorGraphs.at(j)->getBehaviorFilename().section("/", -1, -1), Qt::CaseInsensitive) == 0){
            if (j >= projectFile->behaviorFiles.size() || filename.compare(projectFile->behaviorFiles.at(j)->fileName().section("/", -1, -1), Qt::CaseInsensitive) != 0){
                writeToLog("MainWindow::getBehaviorGraphIndex(): The index is invalid!");
                return -1;
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

bool MainWindow::findGameDirectory(const QString & gamename, QString & gamedirectory){
    QFileInfoList drives = QDir::drives();
    QString driveName;
    QString path;
    QDir dir;
    bool value = false;
    for (int i = 0; i < drives.size(); i++){
        driveName = drives.at(i).absolutePath();
        dir.setPath(driveName);
        path = driveName+"Program Files/Steam/steamapps/common/"+gamename;
        if (dir.exists(path)){
            gamedirectory = path;
            value = true;
        }else{
            path = driveName+"Steam/steamapps/common/"+gamename;
            if (dir.exists(path)){
                gamedirectory = path;
                value = true;
            }else{
                path = driveName+"Steam Games/steamapps/common/"+gamename;
                if (dir.exists(path)){
                    gamedirectory = path;
                    value = true;
                }
            }
        }
    }
    return value;
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

void MainWindow::createNewProject(){
    /*
     * (1): select skeleton
     * (2): name project
     * (3): generate project
     * (4): check if name is availible
     * (5): search directory for animation text files
     * (6): if none use from tool directory
     * (7): set up directory
     * (8): generate character and root behavior file
     * (9): connect to skeleton
     * (10): reset selected files to project file
     * (11): generate new project in animation text files
     * (12): Load UI
*/
    if (!projectFile || (projectFile && closeAll())){
        QString filename = QFileDialog::getOpenFileName(this, tr("Select hkx skeleton file..."), QDir::currentPath(), tr("hkx Files (*.hkx)"));
        if (filename != ""){
            objectDataWid->changeCurrentDataWidget(nullptr);
            lastFileSelected = filename;
            lastFileSelectedPath = filename.section("/", 0, -2);
            skeletonFile = new SkeletonFile(this, filename);
            if (!skeletonFile->parse()){
                FATAL_RUNTIME_ERROR(QString("MainWindow::createNewProject(): The skeleton file "+filename.section("/", -1, -1)+" could not be parsed!!!").toLocal8Bit().data());
            }
            bool ok;
            QString projectname = QInputDialog::getText(this, tr("Set project name!"), tr("Project name:"), QLineEdit::Normal, "", &ok);
            if (ok && projectname != "" && !projectname.contains(QRegExp("^[a-z-A-Z\\]+$"))){
                QString projectDirectoryPath = lastFileSelectedPath+"/"+projectname;
                {
                    QDir projectDirectory(lastFileSelectedPath);
                    if (!projectDirectory.mkdir(projectDirectoryPath) && !QDir(projectDirectoryPath).exists()){
                        FATAL_RUNTIME_ERROR(QString("MainWindow::createNewProject(): Attempt to create '"+projectDirectoryPath+"' directory failed!!!").toUtf8().data());
                    }
                }
                QString relativecharacterpath = "characters/"+projectname+"character.hkx";
                QDir projectDirectory(projectDirectoryPath);
                projectUI->setFilePath(projectDirectoryPath);
                if (projectDirectory.exists()){
                    QStringList list = {"animations", "behaviors", "character assets", "characters"};
                    for (auto i = 0; i < list.size(); i++){
                        if (!projectDirectory.mkdir(list.at(i)) && !QDir(projectDirectoryPath+"/"+list.at(i)).exists()){
                            FATAL_RUNTIME_ERROR(QString("MainWindow::createNewProject(): Attempt to create '"+list.at(i)+"' directory failed!!!").toUtf8().data());
                        }
                    }
                    if (!projectDirectory.rename(lastFileSelected, projectDirectoryPath+"/character assets/"+skeletonFile->fileName().section("/", -1, -1))){
                        FATAL_RUNTIME_ERROR("MainWindow::createNewProject(): Attempt to move skeleton file failed!!!");
                    }
                    projectFile = new ProjectFile(this, projectDirectoryPath+"/"+projectname+".hkx", true, "characters\\"+projectname+"character.hkx");
                    {
                        std::thread thread(&ProjectFile::readAnimationSetData, projectFile, lastFileSelectedPath+"/animationsetdatasinglefile.txt");
                        if (!projectFile->readAnimationData(lastFileSelectedPath+"/animationdatasinglefile.txt", QStringList("Behaviors\\Master.hkx"))){
                            FATAL_RUNTIME_ERROR("MainWindow::createNewProject(): The project animation data file could not be parsed!!!");
                        }
                        thread.join();
                    }
                    if (!projectFile->isProjectNameTaken()){
                        lastFileSelected = projectFile->fileName();
                        lastFileSelectedPath = projectFile->fileName().section("/", 0, -2);
                        characterFile = new CharacterFile(this, projectFile, projectDirectoryPath+"/"+relativecharacterpath, true, "character assets\\"+skeletonFile->fileName().section("/", -1, -1));
                        projectFile->setCharacterFile(characterFile);
                        characterFile->setSkeletonFile(skeletonFile);
                        projectFile->behaviorFiles.append(new BehaviorFile(this, projectFile, characterFile, projectDirectoryPath+"/behaviors/Master.hkx"));
                        projectFile->behaviorFiles.last()->generateDefaultCharacterData();
                        projectFile->addProjectToAnimData();
                        projectUI->setProject(projectFile);
                        projectUI->loadData();
                        projectUI->setDisabled(false);
                        behaviorGraphs.append(new BehaviorGraphView(objectDataWid, projectFile->behaviorFiles.last()));
                        tabs->addTab(projectUI, "Character Data");
                        tabs->addTab(behaviorGraphs.last(), projectFile->behaviorFiles.last()->fileName().section("/", -1, -1));
                        if (!behaviorGraphs.last()->drawGraph(static_cast<DataIconManager *>(projectFile->behaviorFiles.last()->getBehaviorGraph()))){
                            FATAL_RUNTIME_ERROR("MainWindow::createNewProject(): The behavior graph was drawn incorrectly!");
                        }
                        tabs->setCurrentIndex(tabs->count() - 1);
                        saveProject();
                    }else{
                        closeAll();
                        WARNING_MESSAGE(QString("MainWindow::createNewProject(): The chosen project name is taken! Choose another project name!"));
                    }
                }else{
                    FATAL_RUNTIME_ERROR("MainWindow::createNewProject(): Invalid 'lastFileSelectedPath'!!!");
                }
            }else{
                closeAll();
                WARNING_MESSAGE(QString("MainWindow::createNewProject(): Invalid project name!"));
            }
        }
    }
}
