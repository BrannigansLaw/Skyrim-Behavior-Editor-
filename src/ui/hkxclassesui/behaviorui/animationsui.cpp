#include "animationsui.h"
#include "src/ui/skyrimanimationdataui.h"

#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbcharacterstringdata.h"
#include "src/ui/genericdatawidgets.h"
#include "src/animData/skyrimanimdata.h"
#include "src/animData/projectanimdata.h"
#include "src/filetypes/characterfile.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStackedLayout>
#include <QStringList>
#include <QSignalMapper>
#include <QTableWidget>
#include <QHeaderView>
#include <QCoreApplication>
#include <QFileDialog>

QStringList AnimationsUI::headerLabels = {
    "Name",
    "Value"
};

AnimationsUI::AnimationsUI(const QString &title)
    : dataUI(nullptr),
      verLyt(new QVBoxLayout),
      animData(nullptr),
      loadedData(nullptr),
      table(new TableWidget),
      buttonLyt(new QHBoxLayout),
      addObjectPB(new QPushButton("Add Animation")),
      removeObjectPB(new QPushButton("Remove Animation")),
      stackLyt(new QStackedLayout),
      animationUI(new SkyrimAnimationDataUI())
{
    setTitle(title);
    stackLyt->addWidget(table);
    stackLyt->addWidget(animationUI);
    stackLyt->setCurrentIndex(TABLE_WIDGET);
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addSpacing(2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(headerLabels);
    verLyt->addLayout(buttonLyt);
    verLyt->addLayout(stackLyt);
    setLayout(verLyt);
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeAnimation()), Qt::UniqueConnection);
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addAnimation()), Qt::UniqueConnection);
    //connect(animationName, SIGNAL(editingFinished()), this, SLOT(renameSelectedAnimation()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewAnimation(int,int)), Qt::UniqueConnection);
    connect(animationUI, SIGNAL(returnToParent()), this, SLOT(returnToTable()), Qt::UniqueConnection);
}

void AnimationsUI::viewAnimation(int row, int column){
    if (column == 1 && animData && loadedData && loadedData->animationNames.size() > row && row >= 0){
        animationUI->loadData(animData->findMotionData(row));
        stackLyt->setCurrentIndex(ANIMATION_WIDGET);
    }
}

void AnimationsUI::returnToTable(){
    stackLyt->setCurrentIndex(TABLE_WIDGET);
}

void AnimationsUI::renameSelectedAnimation(){
    /*QString newName = animationName->text();
    table->item(table->currentRow(), 0)->setText(newName);
    loadedData->animationNames[table->currentRow()] = newName;
    loadedData->getParentFile()->toggleChanged(true);
    emit animationNameChanged(newName, table->currentRow());*/
}

void AnimationsUI::loadData(HkxObject *data, ProjectAnimData *animdata){
    if (data && data->getSignature() == HKB_CHARACTER_STRING_DATA){
        loadedData = static_cast<hkbCharacterStringData *>(data);
        animData = animdata;
        int row;
        for (int i = 0; i < loadedData->animationNames.size(); i++){
            row = table->rowCount();
            if (table->rowCount() > i){
                table->setRowHidden(i, false);
                if (table->item(row, 0)){
                    table->item(row, 0)->setText(loadedData->animationNames.at(i));
                }else{
                    table->setItem(row, 0, new QTableWidgetItem(loadedData->animationNames.at(i)));
                }
            }else{
                table->setRowCount(row + 1);
                table->setItem(row, 0, new QTableWidgetItem(loadedData->animationNames.at(i)));
                table->setItem(row, 1, new QTableWidgetItem("Edit"));
            }
        }
        for (int j = loadedData->animationNames.size(); j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
    }
}

void AnimationsUI::clear(){
    for (int i = table->rowCount() - 1; i >= 0; i--){
        table->removeRow(i);
    }
}

void AnimationsUI::addAnimation(){
    if (loadedData){
        QString filename = QFileDialog::getOpenFileName(this, tr("Open hkx animation file..."), loadedData->getParentFile()->fileName(), tr("hkx Files (*.hkx)"));
        if (filename != "" && (filename.contains(loadedData->getParentFile()->fileName().section("/", 0, -3)+"/"+"animations", Qt::CaseInsensitive) || filename.contains("SharedKillMoves", Qt::CaseInsensitive)) && loadedData->getAnimationIndex(filename) == -1){
            loadedData->addAnimation(filename.section("/", -2, -1).replace("/", "\\"));
            animData->appendAnimation(new SkyrimAnimationMotionData(animData, loadedData->animationNames.size() - 1));  //Need to set duration later...
            int row = table->rowCount();
            table->setRowCount(row + 1);
            table->setItem(row, 0, new QTableWidgetItem(loadedData->animationNames.last()));
            table->setItem(row, 1, new QTableWidgetItem("Edit"));
            if (stackLyt->currentIndex() == ANIMATION_WIDGET){
                stackLyt->setCurrentIndex(TABLE_WIDGET);
            }
            table->setCurrentCell(row, 0);
            emit openAnimationFile(filename);
            emit animationAdded(loadedData->animationNames.last());
        }
    }
}

void AnimationsUI::removeAnimation(){
    if (loadedData && !static_cast<CharacterFile *>(loadedData->getParentFile())->isAnimationUsed(table->item(table->currentRow(), table->currentColumn())->text())){
        int index = table->currentRow();
        if (!animData->removeAnimation(index)){
            CRITICAL_ERROR_MESSAGE("AnimationsUI::removeAnimation(): Failed!");
        }
        loadedData->animationNames.removeAt(index);
        if (index < table->rowCount()){
            table->removeRow(index);
        }
        if (stackLyt->currentIndex() == ANIMATION_WIDGET){
            stackLyt->setCurrentIndex(TABLE_WIDGET);
        }
        loadedData->getParentFile()->setIsChanged(true);
        emit animationRemoved(index);
        table->setFocus();
    }else{
        WARNING_MESSAGE(QString("Animation is in use! Check the text log for information on where the animation is used..."))
    }
}

void AnimationsUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
