#include "boneweightarrayui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/filetypes/characterfile.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/ui/genericdatawidgets.h"

#define NAME_COLUMN 0
#define VALUE_COLUMN 1

QStringList BoneWeightArrayUI::headerLabels = {
    "Bone Name",
    "Weight"
};

BoneWeightArrayUI::BoneWeightArrayUI()
    : bsData(NULL),
      topLyt(new QGridLayout(this)),
      returnPB(new QPushButton("Return")),
      bones(new TableWidget),
      selectedBone(new DoubleSpinBox),
      label(new QLabel("Selected Bone Weight:"))
{
    setTitle("hkbBoneWeightArray");
    bones->setColumnCount(2);
    bones->setHorizontalHeaderLabels(headerLabels);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(bones, 1, 0, 8, 3);
    topLyt->addWidget(label, 10, 1, 1, 1);
    topLyt->addWidget(selectedBone, 10, 2, 1, 1);
    setLayout(topLyt);
    connect(selectedBone, SIGNAL(editingFinished()), this, SLOT(setBoneWeight()));
    connect(bones, SIGNAL(cellClicked(int,int)), this, SLOT(loadBoneWeight(int,int)));
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

BoneWeightArrayUI::~BoneWeightArrayUI(){
    //
}

void BoneWeightArrayUI::loadData(HkxObject *data, bool isRagdoll){
    blockSignals(true);
    if (data && data->getSignature() == HKB_BONE_WEIGHT_ARRAY){
        bsData = static_cast<hkbBoneWeightArray *>(data);
        HkxFile *file = dynamic_cast<BehaviorFile *>(bsData->getParentFile());
        int rowCount = 0;
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
                CRITICAL_ERROR_MESSAGE(QString("BoneWeightArrayUI::loadData(): Parent file type is unrecognized!!!"))
            }
        }
        for (int i = 0; i < bsData->boneWeights.size(), i < boneNames.size(); i++){
            rowCount = bones->rowCount();
            if (rowCount > i){
                bones->setRowHidden(i, false);
                if (bones->item(i, NAME_COLUMN)){
                    bones->item(i, NAME_COLUMN)->setText(boneNames.at(i));
                }else{
                    bones->setItem(i, NAME_COLUMN, new QTableWidgetItem(boneNames.at(i)));
                }
                if (bones->item(i, VALUE_COLUMN)){
                    bones->item(i, VALUE_COLUMN)->setText(QString::number(bsData->boneWeights.at(i)));
                }else{
                    bones->setItem(i, VALUE_COLUMN, new QTableWidgetItem(QString::number(bsData->boneWeights.at(i), char('f'), 6)));
                }
            }else{
                bones->setRowCount(rowCount + 1);
                bones->setItem(rowCount, NAME_COLUMN, new QTableWidgetItem(boneNames.at(i)));
                bones->setItem(i, VALUE_COLUMN, new QTableWidgetItem(QString::number(bsData->boneWeights.at(i), char('f'), 6)));
            }
        }
        for (int j = bsData->boneWeights.size(); j < bones->rowCount(); j++){
            bones->setRowHidden(j, true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BoneWeightArrayUI::loadData(): The data passed to the UI is NULL!!!"))
    }
    blockSignals(false);
}

void BoneWeightArrayUI::setBoneWeight(){
    int row = bones->currentRow();
    if (bsData){
        if (bsData->boneWeights.size() > row && row >= 0){
            bsData->boneWeights[row] = selectedBone->value();
            if (bones->item(row, VALUE_COLUMN)){
                bones->item(row, VALUE_COLUMN)->setText(QString::number(selectedBone->value(), char('f'), 6));
            }else{
                bones->setItem(row, VALUE_COLUMN, new QTableWidgetItem(QString::number(selectedBone->value(), char('f'), 6)));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BoneWeightArrayUI::setBoneWeight(): The 'bsData' pointer is NULL!!"))
    }
}

void BoneWeightArrayUI::loadBoneWeight(int row, int){
    if (bsData){
        if (bsData->boneWeights.size() > row && row >= 0){
            selectedBone->setValue(bsData->boneWeights.at(row));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("loadBoneWeight::setBoneWeight(): The 'bsData' pointer is NULL!!"))
    }
}
