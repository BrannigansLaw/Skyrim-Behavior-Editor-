#include "boneindexarrayui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/filetypes/characterfile.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/mainwindow.h"

#define BASE_NUMBER_OF_ROWS 1
#define ADD_RAGDOLL_BONE_ROW 0

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList BoneIndexArrayUI::headerLabels = {
    "Bone Name",
    "Type",
    "Bone Index"
};

BoneIndexArrayUI::BoneIndexArrayUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      returnPB(new QPushButton("Return"))
{
    setTitle("hkbBoneIndexArray");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(ADD_RAGDOLL_BONE_ROW, NAME_COLUMN, new TableWidgetItem("Add Ragdoll Bone", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new ragdoll bone"));
    table->setItem(ADD_RAGDOLL_BONE_ROW, TYPE_COLUMN, new TableWidgetItem("Remove Ragdoll Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected ragdoll bone"));
    table->setItem(ADD_RAGDOLL_BONE_ROW, VALUE_COLUMN, new TableWidgetItem("Change Ragdoll Bone", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to change the selected ragdoll bone"));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BoneIndexArrayUI::connectSignals(){
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
}

void BoneIndexArrayUI::disconnectSignals(){
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

void BoneIndexArrayUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_BONE_INDEX_ARRAY){
            bsData = static_cast<hkbBoneIndexArray *>(data);
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BoneIndexArrayUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    connectSignals();
}

void BoneIndexArrayUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_RAGDOLL_BONE_ROW + bsData->boneIndices.size() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        QStringList bones = static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
        for (auto i = ADD_RAGDOLL_BONE_ROW + 1, j = 0; j < bsData->boneIndices.size(); i++, j++){
            temp = bsData->boneIndices.at(j);
            if (temp < bones.size() && temp >= 0){
                setRowItems(i, bones.at(temp), "Remove", "Edit", "Double click to remove this ragdoll bone", VIEW_BONES_TABLE_TIP);
            }else{
                WARNING_MESSAGE("BoneIndexArrayUI::loadDynamicTableRows(): Invalid bone index found!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void BoneIndexArrayUI::setRowItems(int row, const QString & name, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name, Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::white), QBrush(Qt::black)));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(bind);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

void BoneIndexArrayUI::connectToTables(GenericTableWidget *ragdollBones){
    if (ragdollBones){
        disconnect(ragdollBones, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(ragdollBones, SIGNAL(elementSelected(int,QString)), this, SLOT(setRagdollBone(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BoneIndexArrayUI::viewSelectedChild(int row, int column){
    int result = -1;
    if (bsData){
        if (row == ADD_RAGDOLL_BONE_ROW && column == NAME_COLUMN){
            addRagdollBone();
        }else if (row > ADD_RAGDOLL_BONE_ROW && row < ADD_RAGDOLL_BONE_ROW + bsData->boneIndices.size() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->boneIndices.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    emit viewRagdollBones(bsData->boneIndices.at(result) + 1);
                }else if (column == TYPE_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the ragdoll bone \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeRagdollBone(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::viewSelectedChild(): Invalid index of bone to view!!");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BoneIndexArrayUI::setRagdollBone(int index, const QString &name){
    int row = table->currentRow() - BASE_NUMBER_OF_ROWS;
    index--;
    if (bsData){
        if (name != ""){
            if (row >= 0 && row < bsData->boneIndices.size()){
                if (index >= 0){
                    bsData->boneIndices[row] = index;
                    bsData->setIsFileChanged(true);
                    if (table->item(table->currentRow(), NAME_COLUMN)){
                        table->item(table->currentRow(), NAME_COLUMN)->setText(name);
                    }else{
                        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::setRagdollBone(): Table row does not exist!!");
                    }
                }else{
                    removeRagdollBone(row);
                }
            }else{
                CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::setRagdollBone(): Invalid bone index!!");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::setRagdollBone(): Bone name is an empty string!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::setRagdollBone(): The 'bsData' pointer is nullptr!!");
    }
}

void BoneIndexArrayUI::addRagdollBone(){
    if (bsData){
        bsData->boneIndices.append(0);
        bsData->setIsFileChanged(true);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::addRagdollBone(): The data is nullptr!!");
    }
}

void BoneIndexArrayUI::removeRagdollBone(int index){
    if (bsData){
        if (index < bsData->boneIndices.size() && index >= 0){
            bsData->boneIndices.removeAt(index);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("BoneIndexArrayUI::removeRagdollBone(): Invalid index!!");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE("BoneIndexArrayUI::removeRagdollBone(): The data is nullptr!!");
    }
}
