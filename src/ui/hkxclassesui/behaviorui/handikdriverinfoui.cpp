#include "handikdriverinfoui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbhandikdriverinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/handikdriverinfohandui.h"
#include "src/filetypes/behaviorfile.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 1
/*
 * HandIkDriverInfoUI
 */

QStringList HandIkDriverInfoUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

HandIkDriverInfoUI::HandIkDriverInfoUI()
    : bsData(nullptr),
      handDriverGB(new QGroupBox("hkbHandIkDriverInfo")),
      handUI(new HandIkDriverInfoHandUI),
      footDriverLyt(new QGridLayout),
      addHandPB(new QPushButton("Add hand")),
      removeHandPB(new QPushButton("Remove selected hand")),
      table(new TableWidget),
      fadeInOutCurve(new ComboBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setItem(0, 0, new QTableWidgetItem("fadeInOutCurve"));
    table->setItem(0, 1, new QTableWidgetItem("BlendCurve"));
    table->setCellWidget(0, 2, fadeInOutCurve);
    footDriverLyt->addWidget(addHandPB, 0, 0, 1, 1);
    footDriverLyt->addWidget(removeHandPB, 0, 2, 1, 1);
    footDriverLyt->addWidget(table, 1, 0, 8, 3);
    handDriverGB->setLayout(footDriverLyt);
    addWidget(handDriverGB);
    addWidget(handUI);
    connect(fadeInOutCurve, SIGNAL(currentIndexChanged(int)), this, SLOT(setFadeInOutCurve(int)));
    connect(addHandPB, SIGNAL(released()), this, SLOT(addHand()));
    connect(removeHandPB, SIGNAL(released()), this, SLOT(removeSelectedHand()));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedHand(int,int)));
    connect(handUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void HandIkDriverInfoUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_HAND_IK_DRIVER_INFO){
        bsData = static_cast<hkbHandIkDriverInfo *>(data);
        if (fadeInOutCurve->count() <= 0){
            fadeInOutCurve->addItems(bsData->BlendCurve);
        }
        fadeInOutCurve->setCurrentIndex(bsData->BlendCurve.indexOf(bsData->fadeInOutCurve));
        for (int i = 0, k; i < bsData->hands.size(); i++){
            k = i + BASE_NUMBER_OF_ROWS;
            if (k >= table->rowCount()){
                table->setRowCount(table->rowCount() + 1);
                table->setItem(k, 0, new QTableWidgetItem("Hand "+QString::number(i)));
                table->setItem(k, 1, new QTableWidgetItem("hkbHandIkDriverInfoHand"));
                table->setItem(k, 2, new QTableWidgetItem("Click to Edit"));
            }else{
                table->setRowHidden(k, false);
                table->item(k, 0)->setText("Hand "+QString::number(i));
                table->item(k, 1)->setText("hkbHandIkDriverInfoHand");
            }
        }
        for (int j = bsData->hands.size() + BASE_NUMBER_OF_ROWS; j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
    }
}

void HandIkDriverInfoUI::setFadeInOutCurve(int index){
    if (bsData){
        bsData->fadeInOutCurve = fadeInOutCurve->itemText(index);
        bsData->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoUI::addHand(){
    if (bsData){
        bsData->addHand();
        int result = BASE_NUMBER_OF_ROWS + bsData->hands.size();
        if (result >= table->rowCount()){
            result--;
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, 0, new QTableWidgetItem("Hand "+QString::number(bsData->hands.size() - 1)));
            table->setItem(result, 1, new QTableWidgetItem("hkbHandIkDriverInfoHand"));
            table->setItem(result, 2, new QTableWidgetItem("Click to Edit"));
        }else{
            result--;
            table->setRowHidden(result, false);
            table->item(result, 0)->setText("Hand "+QString::number(bsData->hands.size() - 1));
            table->item(result, 1)->setText("hkbHandIkDriverInfoHand");
        }
        bsData->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoUI::removeSelectedHand(){
    int result = table->currentRow() - BASE_NUMBER_OF_ROWS;
    if (bsData && result < bsData->hands.size()){
        bsData->removeHandAt(result);
        delete table->takeItem(table->currentRow(), 0);
        delete table->takeItem(table->currentRow(), 1);
        delete table->takeItem(table->currentRow(), 2);
        table->removeRow(table->currentRow());
    }
}

void HandIkDriverInfoUI::viewSelectedHand(int row, int column){
    int result = row - BASE_NUMBER_OF_ROWS;
    if (bsData && bsData->hands.size() > result && column == 2){
        handUI->loadData((hkbHandIkDriverInfoHand *)bsData->getHandAt(result), bsData);
        setCurrentIndex(HAND_IK_DRIVER_INFO_HAND);
    }
}

void HandIkDriverInfoUI::returnToWidget(){
    setCurrentIndex(HAND_IK_DRIVER_INFO);
}

void HandIkDriverInfoUI::loadBoneList(QStringList &bones){
    handUI->loadBoneList(bones);
}
