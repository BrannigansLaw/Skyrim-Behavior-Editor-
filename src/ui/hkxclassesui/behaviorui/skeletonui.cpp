#include "skeletonui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/hksimplelocalframe.h"
#include "src/filetypes/characterfile.h"
#include "src/hkxclasses/animation/hkaskeleton.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/skeletonfile.h"

#include <QSignalMapper>

SkeletonUI::SkeletonUI(const QString & title)
    : bsData(nullptr),
      lyt(new QVBoxLayout(this)),
      //backPB(new QPushButton("Return")),
      bones(new TableWidget),
      mapper(new QSignalMapper)
{
    setTitle(title);
    QStringList list = {"Name", "Local Frame Name"};
    bones->setColumnCount(2);
    bones->setHorizontalHeaderLabels(list);
    //lyt->addWidget(backPB, 2);
    lyt->addWidget(bones, 8);
    setLayout(lyt);
    //connect(mapper, SIGNAL(mapped(int)), this, SLOT(setLocalFrame(int)));
    //connect(backPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

SkeletonUI::~SkeletonUI(){
    //
}

void SkeletonUI::loadData(HkxObject *data, bool isRagdoll){
    if (data && data->getSignature() == HKA_SKELETON){
        bsData = static_cast<hkaSkeleton *>(data);
        setTitle(bsData->name);
        SkeletonFile *file = static_cast<SkeletonFile *>(bsData->getParentFile());
        int rowCount = 0;
        QStringList boneNames;
        if (file){
            if (isRagdoll){
                boneNames = static_cast<SkeletonFile *>(bsData->getParentFile())->getBonesFromSkeletonAt(1);
            }else{
                boneNames = static_cast<SkeletonFile *>(bsData->getParentFile())->getBonesFromSkeletonAt(0);
            }
        }
        for (int i = 0; i < bsData->bones.size(), i < boneNames.size(); i++){
            rowCount = bones->rowCount();
            if (rowCount > i){
                bones->setRowHidden(i, false);
                if (bones->item(i, 0)){
                    bones->item(i, 0)->setText(boneNames.at(i));
                }else{
                    bones->setItem(i, 0, new QTableWidgetItem(boneNames.at(i)));
                }
                if (bones->cellWidget(i, 1)){
                    //static_cast<LineEdit *>(bones->cellWidget(i, 1))->setText(bsData->bones.at(i).name);
                }else{
                    //bones->setCellWidget(rowCount, 1, new LineEdit(bsData->getLocalFrameName(i)));
                    //mapper->setMapping(bones->cellWidget(rowCount, 1), i);
                    //connect(static_cast<LineEdit *>(bones->cellWidget(i, 1)), SIGNAL(editingFinished()), mapper, SLOT(map()));
                }
            }else{
                bones->setRowCount(rowCount + 1);
                bones->setItem(rowCount, 0, new QTableWidgetItem(boneNames.at(i)));
                //bones->setCellWidget(rowCount, 1, new LineEdit(bsData->getLocalFrameName(i)));
                //mapper->setMapping(bones->cellWidget(rowCount, 1), i);
                //connect(static_cast<LineEdit *>(bones->cellWidget(i, 1)), SIGNAL(editingFinished()), mapper, SLOT(map()));
            }
        }
        for (int j = bsData->bones.size(); j < bones->rowCount(); j++){
            bones->setRowHidden(j, true);
        }
    }
}

void SkeletonUI::setLocalFrame(int row){
    if (bsData){
        if (bsData->bones.size() > row && row >= 0){
            if (bones->cellWidget(row, 1)){
                //bsData->setLocalFrameName(row, static_cast<LineEdit *>(bones->cellWidget(row, 1))->text());
            }
        }
    }
}
