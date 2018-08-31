#include "keyframebonesmodifierui.h"
#include "src/ui/hkxclassesui/behaviorui/keyframeinfoui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbKeyframeBonesModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/hkbboneindexarray.h"
#include "src/ui/hkxclassesui/behaviorui/boneindexarrayui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define ENABLE_ROW 1
#define KEYFRAMED_BONES_LIST_ROW 2
#define ADD_KEYFRAME_INFO_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

const QStringList KeyframeBonesModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

KeyframeBonesModifierUI::KeyframeBonesModifierUI()
    : bsData(nullptr),
      keyframeInfoUI(new KeyframeInfoUI),
      groupBox(new QGroupBox),
      boneIndexUI(new BoneIndexArrayUI),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      keyframedBonesList(new CheckButtonCombo("Edit"))
{
    groupBox->setTitle("hkbKeyframeBonesModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(KEYFRAMED_BONES_LIST_ROW, NAME_COLUMN, new TableWidgetItem("keyframedBonesList"));
    table->setItem(KEYFRAMED_BONES_LIST_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneIndexArray", Qt::AlignCenter));
    table->setItem(KEYFRAMED_BONES_LIST_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(KEYFRAMED_BONES_LIST_ROW, VALUE_COLUMN, keyframedBonesList);
    table->setItem(ADD_KEYFRAME_INFO_ROW, NAME_COLUMN, new TableWidgetItem("Add KeyframeInfo", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new KeyframeInfo"));
    table->setItem(ADD_KEYFRAME_INFO_ROW, TYPE_COLUMN, new TableWidgetItem("hkKeyframeInfo", Qt::AlignCenter));
    table->setItem(ADD_KEYFRAME_INFO_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected KeyframeInfo", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected KeyframeInfo"));
    table->setItem(ADD_KEYFRAME_INFO_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected KeyframeInfo", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected KeyframeInfo"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(keyframeInfoUI);
    addWidget(boneIndexUI);
    toggleSignals(true);
}

void KeyframeBonesModifierUI::toggleSignals(bool toggleconnections){
    if (toggleconnections){
        connect(name, SIGNAL(textEdited(QString)), this, SLOT(setName(QString)), Qt::UniqueConnection);
        connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
        connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
        connect(keyframedBonesList, SIGNAL(pressed()), this, SLOT(viewKeyframedBonesList()), Qt::UniqueConnection);
        connect(keyframedBonesList, SIGNAL(enabled(bool)), this, SLOT(toggleKeyframedBonesList(bool)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
        connect(keyframeInfoUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)), Qt::UniqueConnection);
        connect(keyframeInfoUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    }else{
        disconnect(name, SIGNAL(textEdited(QString)), this, SLOT(setName(QString)));
        disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
        disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
        disconnect(keyframedBonesList, SIGNAL(pressed()), this, SLOT(viewKeyframedBonesList()));
        disconnect(keyframedBonesList, SIGNAL(enabled(bool)), this, SLOT(toggleKeyframedBonesList(bool)));
        disconnect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
        disconnect(keyframeInfoUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)));
        disconnect(keyframeInfoUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    }
}

void KeyframeBonesModifierUI::addKeyframeInfo(){
    (bsData) ? bsData->addKeyframeInfo(), loadDynamicTableRows() : LogFile::writeToLog("KeyframeBonesModifierUI::addKeyframeInfo(): The data is nullptr!!");
}

void KeyframeBonesModifierUI::removeKeyframeInfo(int index){
    (bsData) ? bsData->removeKeyframeInfo(index), loadDynamicTableRows() : LogFile::writeToLog("KeyframeBonesModifierUI::removeKeyframeInfo(): The data is nullptr!!");
}

void KeyframeBonesModifierUI::loadData(HkxObject *data){
    toggleSignals(false);
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == HKB_KEY_FRAME_BONES_MODIFIER){
            bsData = static_cast<hkbKeyframeBonesModifier *>(data);
            name->setText(bsData->getName());
            enable->setChecked(bsData->getEnable());
            if (bsData->getKeyframedBonesList()){
                keyframedBonesList->setChecked(true);
                keyframedBonesList->setText("Edit");
            }else{
                keyframedBonesList->setChecked(false);
                keyframedBonesList->setText("nullptr");
            }
            auto varBind = bsData->getVariableBindingSetData();
            UIHelper::loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable", table, bsData);
            loadDynamicTableRows();
        }else{
            LogFile::writeToLog(QString("KeyframeBonesModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::loadData(): Attempting to load a null pointer!!");
    }
    toggleSignals(true);
}

void KeyframeBonesModifierUI::loadDynamicTableRows(){
    if (bsData){
        auto temp = ADD_KEYFRAME_INFO_ROW + bsData->getNumberOfKeyframeInfos() + 1;
        (table->rowCount() != temp) ? table->setRowCount(temp) : NULL;
        for (auto i = ADD_KEYFRAME_INFO_ROW + 1, j = 0; j < bsData->getNumberOfKeyframeInfos(); i++, j++){
            UIHelper::setRowItems(i, "KeyframeInfo "+QString::number(j), "hkKeyframeInfo", "Remove", "Edit", "Double click to remove this KeyframeInfo", "Double click to edit this KeyframeInfo", table);
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::loadDynamicTableRows(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        auto isProperty = false;
        auto row = table->currentRow();
        if (row == ENABLE_ROW){
            (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked) ? isProperty = true : NULL;
            UIHelper::setBinding(index, row, BINDING_COLUMN, name, "enable", VARIABLE_TYPE_BOOL, isProperty, table, bsData);
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void KeyframeBonesModifierUI::setName(const QString &newname){
    if (bsData){
        bsData->setName(newname);
        bsData->updateIconNames();
        emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::setName(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::setEnable(){
    (bsData) ? bsData->setEnable(enable->isChecked()) : LogFile::writeToLog("KeyframeBonesModifierUI::setEnable(): The 'bsData' pointer is nullptr!!");
}

void KeyframeBonesModifierUI::toggleKeyframedBonesList(bool enable){
    if (bsData){
        if (!enable){
            bsData->setKeyframedBonesList(nullptr);
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (!bsData->getKeyframedBonesList()){
            bsData->setKeyframedBonesList(new hkbBoneIndexArray(bsData->getParentFile()));
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::toggleKeyframedBonesList(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::viewKeyframedBonesList(){
    if (bsData){
        boneIndexUI->loadData(bsData->getKeyframedBonesList());
        setCurrentIndex(BONE_INDEX_WIDGET);
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::viewKeyframedBonesList(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::viewSelectedChild(int row, int column){
    if (bsData){
        if (row < ADD_KEYFRAME_INFO_ROW && row >= 0){
            auto properties = false;
            if (column == BINDING_COLUMN){
                if (row == ENABLE_ROW){
                    (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked) ? properties = true : NULL;
                    selectTableToView(properties, "enable");
                }
            }
        }else if (row == ADD_KEYFRAME_INFO_ROW && column == NAME_COLUMN){
            addKeyframeInfo();
        }else if (row > ADD_KEYFRAME_INFO_ROW && row < ADD_KEYFRAME_INFO_ROW + bsData->getNumberOfKeyframeInfos() + 1){
            auto result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfKeyframeInfos() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    keyframeInfoUI->loadData(((BehaviorFile *)(bsData->getParentFile())), &bsData->keyframeInfo[result], bsData, result);
                    setCurrentIndex(KEYFRAME_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the KeyFrameInfo \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeKeyframeInfo(result);
                    }
                }
            }else{
                LogFile::writeToLog("KeyframeBonesModifierUI::viewSelectedChild(): Invalid index of range to view!!");
            }
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name); break;
    case KEYFRAME_WIDGET:
        keyframeInfoUI->setBindingVariable(index, name); break;
    default:
        WARNING_MESSAGE("KeyframeBonesModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void KeyframeBonesModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *ragdollBones){
    if (variables && properties && ragdollBones){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(ragdollBones, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(ragdollBones, SIGNAL(elementSelected(int,QString)), boneIndexUI, SLOT(setRagdollBone(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void KeyframeBonesModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->getVariableBindingSetData()){
                emit viewProperties(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->getVariableBindingSetData()){
                emit viewVariables(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            auto bind = bsData->getVariableBindingSetData();
            if (bind){
                auto bindIndex = bind->getVariableIndexOfBinding("enable");
                (bindIndex == index) ? table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name) : NULL;
            }
        }else{
            keyframeInfoUI->variableRenamed(name, index);
        }
    }else{
        LogFile::writeToLog("KeyframeBonesModifierUI::variableRenamed(): The data is nullptr!!");
    }
}

