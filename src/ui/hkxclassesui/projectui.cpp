#include "projectui.h"
#include "src/ui/hkxclassesui/behaviorui/characterpropertiesui.h"
#include "src/ui/hkxclassesui/behaviorui/skeletonui.h"
#include "src/ui/hkxclassesui/behaviorui/footikdriverinfoui.h"
#include "src/filetypes/projectfile.h"
#include "src/filetypes/characterfile.h"
#include "src/filetypes/skeletonfile.h"
#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/animation/hkaskeleton.h"

#include <QGridLayout>
#include <QFileSystemModel>
#include <QListView>

ProjectUI::ProjectUI(ProjectFile *file)
    : project(file),
      lyt(new QGridLayout),
      characterProperties(new CharacterPropertiesUI("Character Properties")),
      skeleton(new SkeletonUI("Skeleton")),
      footIK(new FootIkDriverInfoUI),
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
    lyt->addWidget(fileView, 0, 0, 2, 2);
    lyt->addWidget(footIK, 2, 0, 2, 2);
    lyt->addWidget(characterProperties, 0, 2, 2, 2);
    lyt->addWidget(skeleton, 2, 2, 2, 2);
    setLayout(lyt);
    connect(fileView, SIGNAL(doubleClicked(QModelIndex)), this, SIGNAL(openFile(QModelIndex)));
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
        footIK->loadData(project->character->footIkDriverInfo.data());
    }
}
