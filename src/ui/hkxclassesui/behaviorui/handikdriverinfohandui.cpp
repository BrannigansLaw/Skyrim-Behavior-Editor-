#include "handikdriverinfohandui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbhandikdriverinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QLineEdit>

/*
 * HandIkDriverInfoHandUI
 */

QStringList HandIkDriverInfoHandUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

HandIkDriverInfoHandUI::HandIkDriverInfoHandUI()
    : bsData(nullptr),
      lyt(new QVBoxLayout),
      stackLyt(new QStackedLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      elbowAxisLS(new QuadVariableWidget),
      backHandNormalLS(new QuadVariableWidget),
      handOffsetLS(new QuadVariableWidget),
      handOrienationOffsetLS(new QuadVariableWidget),
      maxElbowAngleDegrees(new DoubleSpinBox),
      minElbowAngleDegrees(new DoubleSpinBox),
      shoulderIndex(new ComboBox),
      shoulderSiblingIndex(new ComboBox),
      elbowIndex(new ComboBox),
      elbowSiblingIndex(new ComboBox),
      wristIndex(new ComboBox),
      enforceEndPosition(new CheckBox),
      enforceEndRotation(new CheckBox),
      localFrameName(new LineEdit)
{
    setTitle("hkbHandIkDriverInfoHand");
    table->setRowCount(15);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 1, returnPB);
    table->setItem(1, 0, new QTableWidgetItem("elbowAxisLS"));
    table->setItem(1, 1, new QTableWidgetItem("hkVector4"));
    table->setCellWidget(1, 2, elbowAxisLS);
    table->setItem(2, 0, new QTableWidgetItem("backHandNormalLS"));
    table->setItem(2, 1, new QTableWidgetItem("hkVector4"));
    table->setCellWidget(2, 2, backHandNormalLS);
    table->setItem(3, 0, new QTableWidgetItem("handOffsetLS"));
    table->setItem(3, 1, new QTableWidgetItem("hkVector4"));
    table->setCellWidget(3, 2, handOffsetLS);
    table->setItem(4, 0, new QTableWidgetItem("handOrienationOffsetLS"));
    table->setItem(4, 1, new QTableWidgetItem("hkQuaternion"));
    table->setCellWidget(4, 2, handOrienationOffsetLS);
    table->setItem(5, 0, new QTableWidgetItem("maxElbowAngleDegrees"));
    table->setItem(5, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(5, 2, maxElbowAngleDegrees);
    table->setItem(6, 0, new QTableWidgetItem("minElbowAngleDegrees"));
    table->setItem(6, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(6, 2, minElbowAngleDegrees);
    table->setItem(7, 0, new QTableWidgetItem("shoulderIndex"));
    table->setItem(7, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(7, 2, shoulderIndex);
    table->setItem(8, 0, new QTableWidgetItem("shoulderSiblingIndex"));
    table->setItem(8, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(8, 2, shoulderSiblingIndex);
    table->setItem(9, 0, new QTableWidgetItem("elbowIndex"));
    table->setItem(9, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(9, 2, elbowIndex);
    table->setItem(10, 0, new QTableWidgetItem("elbowSiblingIndex"));
    table->setItem(10, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(10, 2, elbowSiblingIndex);
    table->setItem(11, 0, new QTableWidgetItem("wristIndex"));
    table->setItem(11, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(11, 2, wristIndex);
    table->setItem(12, 0, new QTableWidgetItem("enforceEndPosition"));
    table->setItem(12, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(12, 2, enforceEndPosition);
    table->setItem(13, 0, new QTableWidgetItem("enforceEndRotation"));
    table->setItem(13, 1, new QTableWidgetItem("hkBool"));
    table->setCellWidget(13, 2, enforceEndRotation);
    table->setItem(14, 0, new QTableWidgetItem("localFrameName"));
    table->setItem(14, 1, new QTableWidgetItem("hkStringPtr"));
    table->setCellWidget(14, 2, localFrameName);
    lyt->addWidget(table);
    setLayout(lyt);
    connect(elbowAxisLS, SIGNAL(editingFinished()), this, SLOT(setElbowAxisLS()));
    connect(backHandNormalLS, SIGNAL(editingFinished()), this, SLOT(setBackHandNormalLS()));
    connect(handOffsetLS, SIGNAL(editingFinished()), this, SLOT(setHandOffsetLS()));
    connect(handOrienationOffsetLS, SIGNAL(editingFinished()), this, SLOT(setHandOrienationOffsetLS()));
    connect(maxElbowAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setMaxElbowAngleDegrees()));
    connect(minElbowAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setMinElbowAngleDegrees()));
    connect(shoulderIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setShoulderIndex(int)));
    connect(shoulderSiblingIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setShoulderSiblingIndex(int)));
    connect(elbowIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setElbowIndex(int)));
    connect(elbowSiblingIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setElbowSiblingIndex(int)));
    connect(wristIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setWristIndex(int)));
    connect(enforceEndPosition, SIGNAL(released()), this, SLOT(setEnforceEndPosition()));
    connect(enforceEndRotation, SIGNAL(released()), this, SLOT(setEnforceEndRotation()));
    connect(localFrameName, SIGNAL(editingFinished()), this, SLOT(setLocalFrameName()));
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

void HandIkDriverInfoHandUI::loadData(hkbHandIkDriverInfoHand *data, hkbHandIkDriverInfo *par){
    if (data && par){
        bsData = (hkbHandIkDriverInfo::hkbHandIkDriverInfoHand *)data;
        parent = par;
        elbowAxisLS->setValue(bsData->elbowAxisLS);
        backHandNormalLS->setValue(bsData->backHandNormalLS);
        handOffsetLS->setValue(bsData->handOffsetLS);
        handOrienationOffsetLS->setValue(bsData->handOrienationOffsetLS);
        maxElbowAngleDegrees->setValue(bsData->maxElbowAngleDegrees);
        minElbowAngleDegrees->setValue(bsData->minElbowAngleDegrees);
        shoulderIndex->setCurrentIndex(bsData->shoulderIndex + 1);
        shoulderSiblingIndex->setCurrentIndex(bsData->shoulderSiblingIndex + 1);
        elbowIndex->setCurrentIndex(bsData->elbowIndex + 1);
        elbowSiblingIndex->setCurrentIndex(bsData->elbowSiblingIndex + 1);
        wristIndex->setCurrentIndex(bsData->wristIndex + 1);
        enforceEndPosition->setChecked(bsData->enforceEndPosition);
        enforceEndRotation->setChecked(bsData->enforceEndRotation);
        localFrameName->setText(bsData->localFrameName);
    }else{
        CRITICAL_ERROR_MESSAGE("HandIkDriverInfoHandUI::loadData(): The data or parent object is nullptr!!");
    }
}

void HandIkDriverInfoHandUI::setElbowAxisLS(){
    if (bsData && parent){
        bsData->elbowAxisLS = elbowAxisLS->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setBackHandNormalLS(){
    if (bsData && parent){
        bsData->backHandNormalLS = backHandNormalLS->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setHandOffsetLS(){
    if (bsData && parent){
        bsData->handOffsetLS = handOffsetLS->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setHandOrienationOffsetLS(){
    if (bsData && parent){
        bsData->handOrienationOffsetLS = handOrienationOffsetLS->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setMaxElbowAngleDegrees(){
    if (bsData && parent){
        bsData->maxElbowAngleDegrees = maxElbowAngleDegrees->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setMinElbowAngleDegrees(){
    if (bsData && parent){
        bsData->minElbowAngleDegrees = minElbowAngleDegrees->value();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setShoulderIndex(int index){
    if (bsData && parent){
        bsData->shoulderIndex = index - 1;
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setShoulderSiblingIndex(int index){
    if (bsData && parent){
        bsData->shoulderSiblingIndex = index - 1;
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setElbowIndex(int index){
    if (bsData && parent){
        bsData->elbowIndex = index - 1;
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setElbowSiblingIndex(int index){
    if (bsData && parent){
        bsData->elbowSiblingIndex = index - 1;
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setWristIndex(int index){
    if (bsData && parent){
        bsData->wristIndex = index - 1;
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setEnforceEndPosition(){
    if (bsData && parent){
        bsData->enforceEndPosition = enforceEndPosition->isChecked();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setEnforceEndRotation(){
    if (bsData && parent){
        bsData->enforceEndRotation = enforceEndRotation->isChecked();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::setLocalFrameName(){
    if (bsData && parent){
        bsData->localFrameName = localFrameName->text();
        parent->getParentFile()->setIsChanged(true);
    }
}

void HandIkDriverInfoHandUI::loadBoneList(QStringList & bones){
    bones.prepend("None");
    shoulderIndex->clear();
    shoulderIndex->addItems(bones);
    shoulderSiblingIndex->clear();
    shoulderSiblingIndex->addItems(bones);
    elbowIndex->clear();
    elbowIndex->addItems(bones);
    elbowSiblingIndex->clear();
    elbowSiblingIndex->addItems(bones);
    wristIndex->clear();
    wristIndex->addItems(bones);
}
