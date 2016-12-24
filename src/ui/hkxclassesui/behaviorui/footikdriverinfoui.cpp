#include "footikdriverinfoui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbfootikdriverinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

/*
 * FootIkDriverInfoUI
 */

QStringList FootIkDriverInfoUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

FootIkDriverInfoUI::FootIkDriverInfoUI()
    : bsData(NULL),
      lyt(new QVBoxLayout),
      stackLyt(new QStackedLayout),
      table(new TableWidget),
      raycastDistanceUp(new DoubleSpinBox),
      raycastDistanceDown(new DoubleSpinBox),
      originalGroundHeightMS(new DoubleSpinBox),
      verticalOffset(new DoubleSpinBox),
      collisionFilterInfo(new SpinBox),
      forwardAlignFraction(new DoubleSpinBox),
      sidewaysAlignFraction(new DoubleSpinBox),
      sidewaysSampleWidth(new DoubleSpinBox),
      lockFeetWhenPlanted(new QCheckBox),
      useCharacterUpVector(new QCheckBox),
      isQuadrupedNarrow(new QCheckBox)
{
    setTitle("hkbFootIkDriverInfo");
    table->setRowCount(11);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setItem(0, 0, new QTableWidgetItem("raycastDistanceUp"));
    table->setItem(0, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(0, 2, raycastDistanceUp);
    table->setItem(1, 0, new QTableWidgetItem("raycastDistanceDown"));
    table->setItem(1, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(1, 2, raycastDistanceDown);
    table->setItem(2, 0, new QTableWidgetItem("originalGroundHeightMS"));
    table->setItem(2, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(2, 2, originalGroundHeightMS);
    table->setItem(3, 0, new QTableWidgetItem("verticalOffset"));
    table->setItem(3, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(3, 2, verticalOffset);
    table->setItem(4, 0, new QTableWidgetItem("collisionFilterInfo"));
    table->setItem(4, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(4, 2, collisionFilterInfo);
    table->setItem(5, 0, new QTableWidgetItem("forwardAlignFraction"));
    table->setItem(5, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(5, 2, forwardAlignFraction);
    table->setItem(6, 0, new QTableWidgetItem("sidewaysAlignFraction"));
    table->setItem(6, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(6, 2, sidewaysAlignFraction);
    table->setItem(7, 0, new QTableWidgetItem("sidewaysSampleWidth"));
    table->setItem(7, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(7, 2, sidewaysSampleWidth);
    table->setItem(8, 0, new QTableWidgetItem("lockFeetWhenPlanted"));
    table->setItem(8, 1, new QTableWidgetItem("Bool"));
    table->setCellWidget(8, 2, lockFeetWhenPlanted);
    table->setItem(9, 0, new QTableWidgetItem("useCharacterUpVector"));
    table->setItem(9, 1, new QTableWidgetItem("Bool"));
    table->setCellWidget(9, 2, useCharacterUpVector);
    table->setItem(10, 0, new QTableWidgetItem("isQuadrupedNarrow"));
    table->setItem(10, 1, new QTableWidgetItem("Bool"));
    table->setCellWidget(10, 2, isQuadrupedNarrow);
    lyt->addWidget(table);
    setLayout(lyt);
    connect(raycastDistanceUp, SIGNAL(editingFinished()), this, SLOT(setRaycastDistanceUp()));
    connect(raycastDistanceDown, SIGNAL(editingFinished()), this, SLOT(setRaycastDistanceDown()));
    connect(originalGroundHeightMS, SIGNAL(editingFinished()), this, SLOT(setOriginalGroundHeightMS()));
    connect(verticalOffset, SIGNAL(editingFinished()), this, SLOT(setVerticalOffset()));
    connect(collisionFilterInfo, SIGNAL(editingFinished()), this, SLOT(setCollisionFilterInfo()));
    connect(forwardAlignFraction, SIGNAL(editingFinished()), this, SLOT(setForwardAlignFraction()));
    connect(sidewaysAlignFraction, SIGNAL(editingFinished()), this, SLOT(setSidewaysAlignFraction()));
    connect(sidewaysSampleWidth, SIGNAL(editingFinished()), this, SLOT(setSidewaysSampleWidth()));
    connect(lockFeetWhenPlanted, SIGNAL(released()), this, SLOT(setLockFeetWhenPlanted()));
    connect(useCharacterUpVector, SIGNAL(released()), this, SLOT(setUseCharacterUpVector()));
    connect(isQuadrupedNarrow, SIGNAL(released()), this, SLOT(setIsQuadrupedNarrow()));
}

void FootIkDriverInfoUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_FOOT_IK_DRIVER_INFO){
        bsData = static_cast<hkbFootIkDriverInfo *>(data);
        raycastDistanceUp->setValue(bsData->raycastDistanceUp);
        raycastDistanceDown->setValue(bsData->raycastDistanceDown);
        originalGroundHeightMS->setValue(bsData->originalGroundHeightMS);
        verticalOffset->setValue(bsData->verticalOffset);
        collisionFilterInfo->setValue(bsData->collisionFilterInfo);
        forwardAlignFraction->setValue(bsData->forwardAlignFraction);
        sidewaysAlignFraction->setValue(bsData->sidewaysAlignFraction);
        sidewaysSampleWidth->setValue(bsData->sidewaysSampleWidth);
        lockFeetWhenPlanted->setChecked(bsData->lockFeetWhenPlanted);
        useCharacterUpVector->setChecked(bsData->useCharacterUpVector);
        isQuadrupedNarrow->setChecked(bsData->isQuadrupedNarrow);
    }
}


void FootIkDriverInfoUI::setRaycastDistanceUp(){
    if (bsData){
        bsData->raycastDistanceUp = raycastDistanceUp->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setRaycastDistanceDown(){
    if (bsData){
        bsData->raycastDistanceDown = raycastDistanceDown->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setOriginalGroundHeightMS(){
    if (bsData){
        bsData->originalGroundHeightMS = originalGroundHeightMS->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setVerticalOffset(){
    if (bsData){
        bsData->verticalOffset = verticalOffset->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setCollisionFilterInfo(){
    if (bsData){
        bsData->collisionFilterInfo = collisionFilterInfo->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setForwardAlignFraction(){
    if (bsData){
        bsData->forwardAlignFraction = forwardAlignFraction->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setSidewaysAlignFraction(){
    if (bsData){
        bsData->sidewaysAlignFraction = sidewaysAlignFraction->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setSidewaysSampleWidth(){
    if (bsData){
        bsData->sidewaysSampleWidth = sidewaysSampleWidth->value();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setLockFeetWhenPlanted(){
    if (bsData){
        bsData->lockFeetWhenPlanted = lockFeetWhenPlanted->isChecked();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setUseCharacterUpVector(){
    if (bsData){
        bsData->useCharacterUpVector = useCharacterUpVector->isChecked();
        //behaviorView->toggleChanged(true);
    }
}

void FootIkDriverInfoUI::setIsQuadrupedNarrow(){
    if (bsData){
        bsData->isQuadrupedNarrow = isQuadrupedNarrow->isChecked();
        //behaviorView->toggleChanged(true);
    }
}
