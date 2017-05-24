#include "projectui.h"
#include "src/ui/hkxclassesui/behaviorui/characterpropertiesui.h"
#include "src/ui/hkxclassesui/behaviorui/skeletonui.h"
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
      enableFootIKCB(new QCheckBox("Enable Foot IK")),
      enableHandIKCB(new QCheckBox("Enable Hand IK")),
      footIK(new FootIkDriverInfoUI),
      handIK(new HandIkDriverInfoUI),
      fileSys(new QFileSystemModel),
      fileView(new QListView)
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
    lyt->setSizeConstraint(QLayout::SetNoConstraint);
    fileView->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored, QSizePolicy::DefaultType));
    lyt->addWidget(fileView, 0, 0, 2, 1);
    lyt->addWidget(characterProperties, 0, 1, 2, 3);
    lyt->addWidget(skeleton, 0, 4, 2, 2);
    lyt->addWidget(enableFootIKCB, 2, 0, 2, 3);
    lyt->addWidget(enableHandIKCB, 2, 3, 2, 3);
    lyt->addWidget(footIK, 3, 0, 2, 3);
    lyt->addWidget(handIK, 3, 3, 2, 3);
    setLayout(lyt);
    connect(fileView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(openFile(QModelIndex)));
    connect(enableFootIKCB, SIGNAL(clicked(bool)), this, SLOT(toggleFootIK(bool)));
    connect(enableHandIKCB, SIGNAL(clicked(bool)), this, SLOT(toggleHandIK(bool)));
}

ProjectUI::~ProjectUI()
{
    //
}

void ProjectUI::setFilePath(const QString & path){
    lastFileSelectedPath = path+"\\behaviors";
}

void ProjectUI::setProject(ProjectFile *file){
    project = file;
    if (file){
        fileView->setRootIndex(fileSys->setRootPath(lastFileSelectedPath));
    }
}

void ProjectUI::loadData(){
    if (project && project->character && project->character->skeleton && !project->character->skeleton->skeletons.isEmpty()){
        setTitle(project->fileName());
        characterProperties->loadData(project->character->getCharacterData());
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
