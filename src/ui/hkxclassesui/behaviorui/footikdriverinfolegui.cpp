#include "footikdriverinfolegui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbfootikdriverinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

/*
 * FootIkDriverInfoLegUI
 */

QStringList FootIkDriverInfoLegUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

FootIkDriverInfoLegUI::FootIkDriverInfoLegUI()
    : bsData(NULL),
      lyt(new QVBoxLayout),
      stackLyt(new QStackedLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      kneeAxisLS(new QuadVariableWidget),
      footEndLS(new QuadVariableWidget),
      footPlantedAnkleHeightMS(new DoubleSpinBox),
      footRaisedAnkleHeightMS(new DoubleSpinBox),
      maxAnkleHeightMS(new DoubleSpinBox),
      minAnkleHeightMS(new DoubleSpinBox),
      maxKneeAngleDegrees(new DoubleSpinBox),
      minKneeAngleDegrees(new DoubleSpinBox),
      maxAnkleAngleDegrees(new DoubleSpinBox),
      hipIndex(new ComboBox),
      kneeIndex(new ComboBox),
      ankleIndex(new ComboBox)
{
    setTitle("hkbFootIkDriverInfoLeg");
    table->setRowCount(13);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 1, returnPB);
    table->setItem(1, 0, new TableWidgetItem("kneeAxisLS"));
    table->setItem(2, 1, new TableWidgetItem("hkVector4"));
    table->setCellWidget(1, 2, kneeAxisLS);
    table->setItem(2, 0, new TableWidgetItem("footEndLS"));
    table->setItem(2, 1, new TableWidgetItem("hkVector4"));
    table->setCellWidget(2, 2, footEndLS);
    table->setItem(3, 0, new TableWidgetItem("footPlantedAnkleHeightMS"));
    table->setItem(3, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(3, 2, footPlantedAnkleHeightMS);
    table->setItem(4, 0, new TableWidgetItem("footRaisedAnkleHeightMS"));
    table->setItem(4, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(4, 2, footRaisedAnkleHeightMS);
    table->setItem(5, 0, new TableWidgetItem("maxAnkleHeightMS"));
    table->setItem(5, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(5, 2, maxAnkleHeightMS);
    table->setItem(6, 0, new TableWidgetItem("minAnkleHeightMS"));
    table->setItem(6, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(6, 2, minAnkleHeightMS);
    table->setItem(7, 0, new TableWidgetItem("maxKneeAngleDegrees"));
    table->setItem(7, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(7, 2, maxKneeAngleDegrees);
    table->setItem(8, 0, new TableWidgetItem("minKneeAngleDegrees"));
    table->setItem(8, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(8, 2, minKneeAngleDegrees);
    table->setItem(9, 0, new TableWidgetItem("maxAnkleAngleDegrees"));
    table->setItem(9, 1, new TableWidgetItem("hkReal"));
    table->setCellWidget(9, 2, maxAnkleAngleDegrees);
    table->setItem(10, 0, new TableWidgetItem("hipIndex"));
    table->setItem(10, 1, new TableWidgetItem("hkInt32"));
    table->setCellWidget(10, 2, hipIndex);
    table->setItem(11, 0, new TableWidgetItem("kneeIndex"));
    table->setItem(11, 1, new TableWidgetItem("hkInt32"));
    table->setCellWidget(11, 2, kneeIndex);
    table->setItem(12, 0, new TableWidgetItem("ankleIndex"));
    table->setItem(12, 1, new TableWidgetItem("hkInt32"));
    table->setCellWidget(12, 2, ankleIndex);
    lyt->addWidget(table);
    setLayout(lyt);
    connect(kneeAxisLS, SIGNAL(editingFinished()), this, SLOT(setKneeAxisLS()));
    connect(footEndLS, SIGNAL(editingFinished()), this, SLOT(setFootEndLS()));
    connect(footPlantedAnkleHeightMS, SIGNAL(editingFinished()), this, SLOT(setFootPlantedAnkleHeightMS()));
    connect(footRaisedAnkleHeightMS, SIGNAL(editingFinished()), this, SLOT(setFootRaisedAnkleHeightMS()));
    connect(maxAnkleHeightMS, SIGNAL(editingFinished()), this, SLOT(setMaxAnkleHeightMS()));
    connect(minAnkleHeightMS, SIGNAL(editingFinished()), this, SLOT(setMinAnkleHeightMS()));
    connect(maxKneeAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setMaxKneeAngleDegrees()));
    connect(minKneeAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setMinKneeAngleDegrees()));
    connect(maxAnkleAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setMaxAnkleAngleDegrees()));
    connect(hipIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setHipIndex(int)));
    connect(kneeIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setKneeIndex(int)));
    connect(ankleIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnkleIndex(int)));
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

void FootIkDriverInfoLegUI::loadData(hkbFootIkDriverInfoLeg *data){
    if (data){
        bsData = (hkbFootIkDriverInfo::hkbFootIkDriverInfoLeg *)data;
        kneeAxisLS->setValue(bsData->kneeAxisLS);
        footEndLS->setValue(bsData->footEndLS);
        footPlantedAnkleHeightMS->setValue(bsData->footPlantedAnkleHeightMS);
        footRaisedAnkleHeightMS->setValue(bsData->footRaisedAnkleHeightMS);
        maxAnkleHeightMS->setValue(bsData->maxAnkleHeightMS);
        minAnkleHeightMS->setValue(bsData->minAnkleHeightMS);
        maxKneeAngleDegrees->setValue(bsData->maxKneeAngleDegrees);
        minKneeAngleDegrees->setValue(bsData->minKneeAngleDegrees);
        maxAnkleAngleDegrees->setValue(bsData->maxAnkleAngleDegrees);
        hipIndex->setCurrentIndex(bsData->hipIndex + 1);
        kneeIndex->setCurrentIndex(bsData->kneeIndex + 1);
        ankleIndex->setCurrentIndex(bsData->ankleIndex + 1);
    }
}

void FootIkDriverInfoLegUI::setKneeAxisLS(){
    if (bsData){
        bsData->kneeAxisLS = kneeAxisLS->getValue();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setFootEndLS(){
    if (bsData){
        bsData->footEndLS = footEndLS->getValue();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setFootPlantedAnkleHeightMS(){
    if (bsData){
        bsData->footPlantedAnkleHeightMS = footPlantedAnkleHeightMS->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setFootRaisedAnkleHeightMS(){
    if (bsData){
        bsData->footRaisedAnkleHeightMS = footRaisedAnkleHeightMS->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setMaxAnkleHeightMS(){
    if (bsData){
        bsData->maxAnkleHeightMS = maxAnkleHeightMS->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setMinAnkleHeightMS(){
    if (bsData){
        bsData->minAnkleHeightMS = minAnkleHeightMS->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setMaxKneeAngleDegrees(){
    if (bsData){
        bsData->maxKneeAngleDegrees = maxKneeAngleDegrees->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setMinKneeAngleDegrees(){
    if (bsData){
        bsData->minKneeAngleDegrees = minKneeAngleDegrees->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setMaxAnkleAngleDegrees(){
    if (bsData){
        bsData->maxAnkleAngleDegrees = maxAnkleAngleDegrees->value();
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setHipIndex(int index){
    if (bsData){
        bsData->hipIndex = index - 1;
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setKneeIndex(int index){
    if (bsData){
        bsData->kneeIndex = index - 1;
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::setAnkleIndex(int index){
    if (bsData){
        bsData->ankleIndex = index - 1;
        //bsData->getParentFile()->toggleChanged(true);
    }
}

void FootIkDriverInfoLegUI::loadBoneList(QStringList & bones){
    bones.prepend("None");
    hipIndex->clear();
    hipIndex->addItems(bones);
    kneeIndex->clear();
    kneeIndex->addItems(bones);
    ankleIndex->clear();
    ankleIndex->addItems(bones);
}
