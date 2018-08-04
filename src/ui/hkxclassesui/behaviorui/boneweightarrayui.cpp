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
    : bsData(nullptr),
      topLyt(new QGridLayout(this)),
      returnPB(new QPushButton("Return")),
      bones(new TableWidget),
      selectedBone(new DoubleSpinBox),
      label(new QLabel("Selected Bone Weight:")),
      setAllCB(new QCheckBox("Set All")),
      invertAllPB(new QPushButton("Invert All"))
{
    setTitle("hkbBoneWeightArray");
    bones->setSelectionBehavior(QAbstractItemView::SelectRows);
    bones->setColumnCount(2);
    bones->setHorizontalHeaderLabels(headerLabels);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(bones, 1, 0, 8, 4);
    topLyt->addWidget(setAllCB, 10, 0, 2, 1);
    topLyt->addWidget(invertAllPB, 10, 1, 2, 1);
    topLyt->addWidget(label, 10, 2, 2, 1);
    topLyt->addWidget(selectedBone, 10, 3, 2, 1);
    setLayout(topLyt);
    //label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //selectedBone->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    selectedBone->setMinimum(0);
    selectedBone->setMaximum(1);
    selectedBone->setSingleStep(0.05);
    connect(selectedBone, SIGNAL(editingFinished()), this, SLOT(setBoneWeight()), Qt::UniqueConnection);
    connect(bones, SIGNAL(cellClicked(int,int)), this, SLOT(loadBoneWeight(int,int)), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(invertAllPB, SIGNAL(released()), this, SLOT(invert()), Qt::UniqueConnection);
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
                CRITICAL_ERROR_MESSAGE("BoneWeightArrayUI::loadData(): Parent file type is unrecognized!!!");
            }
        }
        for (auto i = 0; i < bsData->boneWeights.size() && i < boneNames.size(); i++){
            rowCount = bones->rowCount();
            if (rowCount > i){
                bones->setRowHidden(i, false);
                if (bones->item(i, NAME_COLUMN)){
                    bones->item(i, NAME_COLUMN)->setText(boneNames.at(i));
                }else{
                    bones->setItem(i, NAME_COLUMN, new TableWidgetItem(boneNames.at(i), Qt::AlignCenter, QColor(Qt::white)));
                }
                if (bones->item(i, VALUE_COLUMN)){
                    bones->item(i, VALUE_COLUMN)->setText(QString::number(bsData->boneWeights.at(i), char('f'), 6));
                }else{
                    bones->setItem(i, VALUE_COLUMN, new TableWidgetItem(QString::number(bsData->boneWeights.at(i), char('f'), 6), Qt::AlignCenter, QColor(Qt::white)));
                }
            }else{
                bones->setRowCount(rowCount + 1);
                bones->setItem(rowCount, NAME_COLUMN, new TableWidgetItem(boneNames.at(i), Qt::AlignCenter, QColor(Qt::white)));
                bones->setItem(i, VALUE_COLUMN, new TableWidgetItem(QString::number(bsData->boneWeights.at(i), char('f'), 6), Qt::AlignCenter, QColor(Qt::white)));
            }
        }
        for (auto j = bsData->boneWeights.size(); j < bones->rowCount(); j++){
            bones->setRowHidden(j, true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneWeightArrayUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    blockSignals(false);
}

void BoneWeightArrayUI::setBoneWeight(){
    int row = bones->currentRow();
    if (bsData){
        if (bsData->boneWeights.size() > row && row >= 0){
            if (setAllCB->isChecked()){
                for (auto i = 0; i < bsData->boneWeights.size(); i++){
                    bsData->boneWeights[i] = selectedBone->value();
                    if (bones->item(i, VALUE_COLUMN)){
                        bones->item(i, VALUE_COLUMN)->setText(QString::number(selectedBone->value(), char('f'), 6));
                    }else{
                        bones->setItem(i, VALUE_COLUMN, new TableWidgetItem(QString::number(selectedBone->value(), char('f'), 6), Qt::AlignCenter, QColor(Qt::white)));
                    }
                }
            }else{
                bsData->boneWeights[row] = selectedBone->value();
                if (bones->item(row, VALUE_COLUMN)){
                    bones->item(row, VALUE_COLUMN)->setText(QString::number(selectedBone->value(), char('f'), 6));
                }else{
                    bones->setItem(row, VALUE_COLUMN, new TableWidgetItem(QString::number(selectedBone->value(), char('f'), 6), Qt::AlignCenter, QColor(Qt::white)));
                }
            }
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneWeightArrayUI::setBoneWeight(): The 'bsData' pointer is nullptr!!");
    }
}

void BoneWeightArrayUI::loadBoneWeight(int row, int){
    if (bsData){
        if (bsData->boneWeights.size() > row && row >= 0){
            selectedBone->setValue(bsData->boneWeights.at(row));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneWeightArrayUI::setBoneWeight(): The 'bsData' pointer is nullptr!!");
    }
}

void BoneWeightArrayUI::invert(){
    if (bsData){
        for (auto i = 0; i < bsData->boneWeights.size(); i++){
            bsData->boneWeights[i] = 1 - bsData->boneWeights.at(i);
        }
        for (auto i = 0; i < bones->rowCount() && i < bsData->boneWeights.size(); i++){
            if (bones->item(i, VALUE_COLUMN)){
                bones->item(i, VALUE_COLUMN)->setText(QString::number(bsData->boneWeights.at(i), char('f'), 6));
            }else{
                bones->setItem(i, VALUE_COLUMN, new TableWidgetItem(QString::number(bsData->boneWeights.at(i), char('f'), 6), Qt::AlignCenter, QColor(Qt::white)));
            }
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BoneWeightArrayUI::invert(): The 'bsData' pointer is nullptr!!");
    }
}
