#include "boneweightarrayui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/filetypes/characterfile.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/ui/genericdatawidgets.h"

#include <QSignalMapper>

BoneWeightArrayUI::BoneWeightArrayUI()
    : bsData(NULL),
      lyt(new QVBoxLayout(this)),
      backPB(new QPushButton("Return")),
      bones(new TableWidget),
      mapper(new QSignalMapper)
{
    setTitle("hkbBoneWeightArray");
    QStringList list = {"Name", "Value"};
    bones->setColumnCount(2);
    bones->setHorizontalHeaderLabels(list);
    lyt->addWidget(backPB, 2);
    lyt->addWidget(bones, 8);
    setLayout(lyt);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setBoneWeight(int)));
    connect(backPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

BoneWeightArrayUI::~BoneWeightArrayUI(){
    //
}

void BoneWeightArrayUI::loadData(HkxObject *data, bool isRagdoll){
    if (data && data->getSignature() == HKB_BONE_WEIGHT_ARRAY){
        bsData = static_cast<hkbBoneWeightArray *>(data);
        /*HkxFile *file = dynamic_cast<BehaviorFile *>(bsData->getParentFile());
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
                //error
            }
        }
        for (int i = 0; i < bsData->boneWeights.size(), i < boneNames.size(); i++){
            rowCount = bones->rowCount();
            if (rowCount > i){
                bones->setRowHidden(i, false);
                if (bones->item(i, 0)){
                    bones->item(i, 0)->setText(boneNames.at(i));
                }else{
                    bones->setItem(i, 0, new QTableWidgetItem(boneNames.at(i)));
                }
                if (bones->cellWidget(i, 1)){
                    static_cast<DoubleSpinBox *>(bones->cellWidget(i, 1))->setValue(bsData->boneWeights.at(i));
                }else{
                    bones->setCellWidget(i, 1, new DoubleSpinBox(NULL, bsData->boneWeights.at(i), 6, 0.1, 1, 0));
                    mapper->setMapping(bones->cellWidget(i, 1), i);
                    connect(static_cast<DoubleSpinBox *>(bones->cellWidget(i, 1)), SIGNAL(editingFinished()), mapper, SLOT(map()));
                }
            }else{
                bones->setRowCount(rowCount + 1);
                bones->setItem(rowCount, 0, new QTableWidgetItem(boneNames.at(i)));
                bones->setCellWidget(rowCount, 1, new DoubleSpinBox(NULL, bsData->boneWeights.at(i), 6, 0.1, 1, 0));
                mapper->setMapping(bones->cellWidget(rowCount, 1), i);
                connect(static_cast<DoubleSpinBox *>(bones->cellWidget(i, 1)), SIGNAL(editingFinished()), mapper, SLOT(map()));
            }
        }
        for (int j = bsData->boneWeights.size(); j < bones->rowCount(); j++){
            bones->setRowHidden(j, true);
        }*/
    }
}

void BoneWeightArrayUI::setBoneWeight(int row){
    if (bsData){
        if (bsData->boneWeights.size() > row && row >= 0){
            if (bones->cellWidget(row, 1)){
                bsData->boneWeights[row] = static_cast<DoubleSpinBox *>(bones->cellWidget(row, 1))->value();
            }
        }
    }
}
