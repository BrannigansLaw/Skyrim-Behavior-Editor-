#include "projectui.h"
#include "src/ui/hkxclassesui/behaviorui/characterpropertiesui.h"
#include "src/ui/hkxclassesui/behaviorui/skeletonui.h"
#include "src/ui/hkxclassesui/behaviorui/animationsui.h"
#include "src/ui/hkxclassesui/behaviorui/footikdriverinfoui.h"
#include "src/ui/hkxclassesui/behaviorui/handikdriverinfoui.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"
#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/animation/hkaskeleton.h"

#include <QGridLayout>
#include <QFileSystemModel>
#include <QListView>
#include <QSizePolicy>

#include "src/ui/genericdatawidgets.h"

ProjectUI::ProjectUI(ProjectFile *file)
    : project(file),
      lyt(new QGridLayout),
      characterProperties(new CharacterPropertiesUI("Character Properties")),
      skeleton(new SkeletonUI("Skeleton")),
      animations(new AnimationsUI("Animations")),
      enableFootIKCB(new CheckBox("Enable Foot IK")),
      enableHandIKCB(new CheckBox("Enable Hand IK")),
      footIK(new FootIkDriverInfoUI),
      handIK(new HandIkDriverInfoUI),
      fileSys(new QFileSystemModel),
      fileView(new QListView),
      addBehaviorFile(new QPushButton("Add New Behavior")),
      initializeWithCharacterData(new CheckBox("Use Character Events/Variables"))
{
    if (file){
        fileView->setRootIndex(fileSys->setRootPath(lastFileSelectedPath));
    }else{
        fileView->setRootIndex(fileSys->setRootPath(QDir::currentPath()));
    }
    fileSys->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    QStringList filters;
    filters << "*.hkx";
    fileSys->setNameFilters(filters);
    fileSys->setNameFilterDisables(false);
    fileView->setModel(fileSys);
    //fileView->setMaximumSize(100, 400);
    //lyt->setSizeConstraint(QLayout::SetNoConstraint);
    //fileView->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum, QSizePolicy::CheckBox));
    lyt->addWidget(addBehaviorFile, 0, 0, 1, 1);
    //lyt->addWidget(initializeWithCharacterData, 0, 1, 1, 1);
    lyt->addWidget(fileView, 1, 0, 2, 1);
    lyt->addWidget(characterProperties, 0, 2, 2, 1);
    lyt->addWidget(skeleton, 0, 4, 2, 1);
    lyt->addWidget(enableFootIKCB, 3, 0, 1, 1);
    lyt->addWidget(enableHandIKCB, 3, 2, 1, 1);
    lyt->addWidget(footIK, 4, 0, 2, 1);
    lyt->addWidget(handIK, 4, 2, 2, 1);
    lyt->addWidget(animations, 4, 4, 2, 1);
    setLayout(lyt);
    connect(fileView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(openFile(QModelIndex)), Qt::UniqueConnection);
    connect(enableFootIKCB, SIGNAL(clicked(bool)), this, SLOT(toggleFootIK(bool)), Qt::UniqueConnection);
    connect(enableHandIKCB, SIGNAL(clicked(bool)), this, SLOT(toggleHandIK(bool)), Qt::UniqueConnection);
    connect(addBehaviorFile, SIGNAL(released()), this, SLOT(addNewBehaviorFile()), Qt::UniqueConnection);
}

ProjectUI::~ProjectUI()
{
    //
}

void ProjectUI::setFilePath(const QString & path){
    lastFileSelectedPath = path;
}

void ProjectUI::setDisabled(bool disable){
    if (disable){
        skeleton->setEnabled(false);
        animations->setEnabled(false);
        footIK->setEnabled(false);
        handIK->setEnabled(false);
        enableFootIKCB->setEnabled(false);
        enableHandIKCB->setEnabled(false);
    }else{
        skeleton->setEnabled(true);
        animations->setEnabled(true);
        footIK->setEnabled(true);
        handIK->setEnabled(true);
        enableFootIKCB->setEnabled(true);
        enableHandIKCB->setEnabled(true);
    }
}

void ProjectUI::setProject(ProjectFile *file){
    project = file;
    if (project && project->character){
        fileView->setRootIndex(fileSys->setRootPath(lastFileSelectedPath+"/"+project->character->getBehaviorDirectoryName()));
    }
}

void ProjectUI::loadData(){
    if (project && project->character && project->character->skeleton && !project->character->skeleton->skeletons.isEmpty()){
        setTitle(project->fileName());
        characterProperties->loadData(project->character->getCharacterData());
        int index = project->skyrimAnimData->getProjectIndex(project->fileName().section("/", -1, -1));
        if (index == -1){
            (qFatal("ProjectUI::loadData(): getProjectIndex failed!"));
        }
        animations->loadData(project->character->stringData.data(), project->skyrimAnimData->animData.at(index));//UNSAFE....
        skeleton->loadData(project->character->skeleton->skeletons.first().data());
        if (project->character->footIkDriverInfo.data()){
            footIK->loadData(project->character->footIkDriverInfo.data());
            enableFootIKCB->setChecked(true);
        }else{
            footIK->setDisabled(true);
        }
        footIK->loadBoneList(static_cast<hkaSkeleton *>(project->character->skeleton->skeletons.first().data())->getBoneNames());
        if (project->character->handIkDriverInfo.data()){
            handIK->loadData(project->character->handIkDriverInfo.data());
            enableHandIKCB->setChecked(true);
        }else{
            handIK->setDisabled(true);
        }
        handIK->loadBoneList(static_cast<hkaSkeleton *>(project->character->skeleton->skeletons.first().data())->getBoneNames());
    }
}

void ProjectUI::toggleFootIK(bool toggle){
    footIK->setEnabled(toggle);
    if (project && project->character && !project->character->footIkDriverInfo.data()){
        if (toggle){
            project->character->addFootIK();
            footIK->loadData(project->character->footIkDriverInfo.data());
        }else{
            project->character->disableFootIK();
        }
    }
}

void ProjectUI::toggleHandIK(bool toggle){
    handIK->setEnabled(toggle);
    if (project && project->character && !project->character->handIkDriverInfo.data()){
        if (toggle){
            project->character->addHandIK();
            handIK->loadData(project->character->handIkDriverInfo.data());
        }else{
            project->character->disableHandIK();
        }
    }
}

void ProjectUI::addNewBehaviorFile(){
    emit addBehavior(initializeWithCharacterData->isChecked());
}
