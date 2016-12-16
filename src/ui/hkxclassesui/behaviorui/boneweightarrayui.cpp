#include "boneweightarrayui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/filetypes/characterfile.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/ui/genericdatawidgets.h"

#include <QCoreApplication>

BoneWeightArrayUI::BoneWeightArrayUI()
    : bsData(NULL),
      lyt(new QVBoxLayout(this)),
      backPB(new QPushButton("Return")),
      bones(new TableWidget),
      //spinBox(new DoubleSpinBox),
      previousRow(-1)
{
    QStringList list = {"Name", "Value"};
    bones->setColumnCount(2);
    bones->setHorizontalHeaderLabels(list);
    /*spinBox->setDecimals(6);
    spinBox->setMinimum(0);
    spinBox->setMaximum(1);
    spinBox->setSingleStep(0.1);*/
    bones->setEditTriggers(QAbstractItemView::DoubleClicked);
    lyt->addWidget(backPB);
    lyt->addWidget(bones);
    setLayout(lyt);
    //connect(bones, SIGNAL(cellClicked(int,int)), this, SLOT(selectionChanged(int,int)));
    connect(spinBox, SIGNAL(editingFinished()), this, SLOT(setBoneWeight()));
    connect(backPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

BoneWeightArrayUI::~BoneWeightArrayUI(){
    //
}

void BoneWeightArrayUI::loadData(HkxObject *data, bool isRagdoll){
    if (data && data->getSignature() == HKB_BONE_WEIGHT_ARRAY){
        bsData = static_cast<hkbBoneWeightArray *>(data);
        HkxFile *file = dynamic_cast<BehaviorFile *>(bsData->getParentFile());
        int row = 0;
        QStringList boneNames;
        if (file){
            if (isRagdoll){
                boneNames = static_cast<BehaviorFile *>(file)->getRagdollBoneNames();
            }else{
                boneNames = static_cast<BehaviorFile *>(file)->getRigBoneNames();
            }
        }else{
            file = dynamic_cast<CharacterFile *>(bsData->getParentFile());
            if (file){
                if (isRagdoll){
                    boneNames = static_cast<CharacterFile *>(file)->getRagdollBoneNames();
                }else{
                    boneNames = static_cast<CharacterFile *>(file)->getRigBoneNames();
                }
            }else{
                //error
            }
        }
        for (int i = 0; i < bsData->boneWeights.size(), i < boneNames.size(); i++){
            row = bones->rowCount();
            if (bones->rowCount() > i){
                bones->setRowHidden(i, false);
                if (bones->item(row, 0)){
                    bones->item(row, 0)->setText(boneNames.at(i));
                }else{
                    bones->setItem(row, 0, new QTableWidgetItem(boneNames.at(i)));
                }
                if (bones->item(row, 1)){
                    bones->item(row, 1)->setText(QString::number(bsData->boneWeights.at(i), 'g', 6));
                }else{
                    bones->setItem(row, 1, new QTableWidgetItem(QString::number(bsData->boneWeights.at(i), 'g', 6)));
                }
            }else{
                bones->setRowCount(row + 1);
                bones->setItem(row, 0, new QTableWidgetItem(boneNames.at(i)));
                bones->setItem(row, 1, new QTableWidgetItem(QString::number(bsData->boneWeights.at(i), 'g', 6)));
            }
        }
        for (int j = bsData->boneWeights.size(); j < bones->rowCount(); j++){
            bones->setRowHidden(j, true);
        }
    }
}

/*void BoneWeightArrayUI::selectionChanged(int row, int column){
    if (column != 1){
        return;
    }
    bones->blockSignals(true);
    QWidget *wid = NULL;
    if (previousRow >= 0){
        wid = bones->cellWidget(previousRow, column);
    }
    if (wid){
        bones->removeCellWidget(row, column);
    }else{
        for (int i = 0; i < bones->rowCount(); i++){
            wid = bones->cellWidget(i, column);
            if (wid){
                bones->removeCellWidget(i, column);
                break;
            }
        }
    }
    bones->setCellWidget(row, column, spinBox);
    if (bsData->boneWeights.size() > row && row >= 0){
        spinBox->setValue(bsData->boneWeights.at(row));
    }
    previousRow = row;
    bones->blockSignals(false);
}*/

void BoneWeightArrayUI::setBoneWeight(){
    if (bsData){
        if (bsData->boneWeights.size() > bones->currentRow() && bones->currentRow() >= 0 && bones->cellWidget(bones->currentRow(), 1) == spinBox){
            bsData->boneWeights[bones->currentRow()] = spinBox->value();
            if (bones->item(bones->currentRow(), 1)){
                bones->item(bones->currentRow(), 1)->setText(QString::number(bsData->boneWeights.at(bones->currentRow())));
            }else{
                bones->setItem(bones->currentRow(), 1, new QTableWidgetItem(QString::number(bsData->boneWeights.at(bones->currentRow()), 'g', 6)));
            }
        }
    }
}
