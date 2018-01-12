#include "footikdriverinfoui.h"
#include "footikdriverinfolegui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbfootikdriverinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

#define BASE_NUMBER_OF_ROWS 11
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
      footDriverGB(new QGroupBox("hkbFootIkDriverInfo")),
      legUI(new FootIkDriverInfoLegUI),
      footDriverLyt(new QGridLayout),
      addLegPB(new QPushButton("Add leg")),
      removeLegPB(new QPushButton("Remove selected leg")),
      table(new TableWidget),
      raycastDistanceUp(new DoubleSpinBox),
      raycastDistanceDown(new DoubleSpinBox),
      originalGroundHeightMS(new DoubleSpinBox),
      verticalOffset(new DoubleSpinBox),
      collisionFilterInfo(new SpinBox),
      forwardAlignFraction(new DoubleSpinBox),
      sidewaysAlignFraction(new DoubleSpinBox),
      sidewaysSampleWidth(new DoubleSpinBox),
      lockFeetWhenPlanted(new CheckBox),
      useCharacterUpVector(new CheckBox),
      isQuadrupedNarrow(new CheckBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setItem(0, 0, new QTableWidgetItem("raycastDistanceUp"));
    table->setItem(0, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(0, 2, raycastDistanceUp);
    table->setItem(1, 0, new QTableWidgetItem("raycastDistanceDown"));
    table->setItem(1, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(1, 2, raycastDistanceDown);
    table->setItem(2, 0, new QTableWidgetItem("originalGroundHeightMS"));
    table->setItem(2, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(2, 2, originalGroundHeightMS);
    table->setItem(3, 0, new QTableWidgetItem("verticalOffset"));
    table->setItem(3, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(3, 2, verticalOffset);
    table->setItem(4, 0, new QTableWidgetItem("collisionFilterInfo"));
    table->setItem(4, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(4, 2, collisionFilterInfo);
    table->setItem(5, 0, new QTableWidgetItem("forwardAlignFraction"));
    table->setItem(5, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(5, 2, forwardAlignFraction);
    table->setItem(6, 0, new QTableWidgetItem("sidewaysAlignFraction"));
    table->setItem(6, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(6, 2, sidewaysAlignFraction);
    table->setItem(7, 0, new QTableWidgetItem("sidewaysSampleWidth"));
    table->setItem(7, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(7, 2, sidewaysSampleWidth);
    table->setItem(8, 0, new QTableWidgetItem("lockFeetWhenPlanted"));
    table->setItem(8, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(8, 2, lockFeetWhenPlanted);
    table->setItem(9, 0, new QTableWidgetItem("useCharacterUpVector"));
    table->setItem(9, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(9, 2, useCharacterUpVector);
    table->setItem(10, 0, new QTableWidgetItem("isQuadrupedNarrow"));
    table->setItem(10, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(10, 2, isQuadrupedNarrow);
    footDriverLyt->addWidget(addLegPB, 0, 0, 1, 1);
    footDriverLyt->addWidget(removeLegPB, 0, 2, 1, 1);
    footDriverLyt->addWidget(table, 1, 0, 8, 3);
    footDriverGB->setLayout(footDriverLyt);
    addWidget(footDriverGB);
    addWidget(legUI);
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
    connect(addLegPB, SIGNAL(released()), this, SLOT(addLeg()));
    connect(removeLegPB, SIGNAL(released()), this, SLOT(removeSelectedLeg()));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedLeg(int,int)));
    connect(legUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
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
        for (int i = 0, k; i < bsData->legs.size(); i++){
            k = i + BASE_NUMBER_OF_ROWS;
            if (k >= table->rowCount()){
                table->setRowCount(table->rowCount() + 1);
                table->setItem(k, 0, new QTableWidgetItem("Leg "+QString::number(i)));
                table->setItem(k, 1, new QTableWidgetItem("hkbFootIkDriverInfoLeg"));
                table->setItem(k, 2, new QTableWidgetItem("Click to Edit"));
            }else{
                table->setRowHidden(k, false);
                table->item(k, 0)->setText("Leg "+QString::number(i));
                table->item(k, 1)->setText("hkbFootIkDriverInfoLeg");
            }
        }
        for (int j = bsData->legs.size() + BASE_NUMBER_OF_ROWS; j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
    }
}

void FootIkDriverInfoUI::setRaycastDistanceUp(){
    if (bsData){
        bsData->raycastDistanceUp = raycastDistanceUp->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setRaycastDistanceDown(){
    if (bsData){
        bsData->raycastDistanceDown = raycastDistanceDown->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setOriginalGroundHeightMS(){
    if (bsData){
        bsData->originalGroundHeightMS = originalGroundHeightMS->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setVerticalOffset(){
    if (bsData){
        bsData->verticalOffset = verticalOffset->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setCollisionFilterInfo(){
    if (bsData){
        bsData->collisionFilterInfo = collisionFilterInfo->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setForwardAlignFraction(){
    if (bsData){
        bsData->forwardAlignFraction = forwardAlignFraction->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setSidewaysAlignFraction(){
    if (bsData){
        bsData->sidewaysAlignFraction = sidewaysAlignFraction->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setSidewaysSampleWidth(){
    if (bsData){
        bsData->sidewaysSampleWidth = sidewaysSampleWidth->value();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setLockFeetWhenPlanted(){
    if (bsData){
        bsData->lockFeetWhenPlanted = lockFeetWhenPlanted->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setUseCharacterUpVector(){
    if (bsData){
        bsData->useCharacterUpVector = useCharacterUpVector->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::setIsQuadrupedNarrow(){
    if (bsData){
        bsData->isQuadrupedNarrow = isQuadrupedNarrow->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::addLeg(){
    if (bsData){
        bsData->addLeg();
        int result = BASE_NUMBER_OF_ROWS + bsData->legs.size();
        if (result >= table->rowCount()){
            result--;
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, 0, new QTableWidgetItem("Leg "+QString::number(bsData->legs.size() - 1)));
            table->setItem(result, 1, new QTableWidgetItem("hkbFootIkDriverInfoLeg"));
            table->setItem(result, 2, new QTableWidgetItem("Click to Edit"));
        }else{
            result--;
            table->setRowHidden(result, false);
            table->item(result, 0)->setText("Leg "+QString::number(bsData->legs.size() - 1));
            table->item(result, 1)->setText("hkbFootIkDriverInfoLeg");
        }
        bsData->getParentFile()->setIsChanged(true);
    }
}

void FootIkDriverInfoUI::removeSelectedLeg(){
    int result = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData && result < bsData->legs.size()){
        bsData->removeLegAt(result);
        delete table->takeItem(table->currentRow(), 0);
        delete table->takeItem(table->currentRow(), 1);
        delete table->takeItem(table->currentRow(), 2);
        table->removeRow(table->currentRow());
    }
}

void FootIkDriverInfoUI::viewSelectedLeg(int row, int column){
    int result = row - BASE_NUMBER_OF_ROWS;
    if (bsData && bsData->legs.size() > result && column == 2){
        legUI->loadData((hkbFootIkDriverInfoLeg *)bsData->getLegAt(result));
        setCurrentIndex(FOOT_IK_DRIVER_INFO_LEG);
    }
}

void FootIkDriverInfoUI::returnToWidget(){
    setCurrentIndex(FOOT_IK_DRIVER_INFO);
}

void FootIkDriverInfoUI::loadBoneList(QStringList &bones){
    legUI->loadBoneList(bones);
}
