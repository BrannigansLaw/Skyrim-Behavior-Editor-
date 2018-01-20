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

QStringList KeyframeBonesModifierUI::headerLabels = {
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
      keyframedBonesList(new CheckButtonCombo("Click to edit"))
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
    connectSignals();
}

void KeyframeBonesModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(keyframedBonesList, SIGNAL(pressed()), this, SLOT(viewKeyframedBonesList()), Qt::UniqueConnection);
    connect(keyframedBonesList, SIGNAL(enabled(bool)), this, SLOT(toggleKeyframedBonesList(bool)), Qt::UniqueConnection);
    connect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(keyframeInfoUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(keyframeInfoUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void KeyframeBonesModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(keyframedBonesList, SIGNAL(pressed()), this, SLOT(viewKeyframedBonesList()));
    disconnect(keyframedBonesList, SIGNAL(enabled(bool)), this, SLOT(toggleKeyframedBonesList(bool)));
    disconnect(boneIndexUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(keyframeInfoUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(keyframeInfoUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void KeyframeBonesModifierUI::addKeyframeInfo(){
    if (bsData){
        bsData->keyframeInfo.append(hkbKeyframeBonesModifier::hkKeyframeInfo());
        bsData->getParentFile()->setIsChanged(true);
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::addKeyframeInfo(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::removeKeyframeInfo(int index){
    if (bsData){
        if (!bsData->keyframeInfo.isEmpty()){
            if (index < bsData->keyframeInfo.size() && index >= 0){
                bsData->keyframeInfo.removeAt(index);
            }else{
                WARNING_MESSAGE("KeyframeBonesModifierUI::removeKeyframeInfo(): Invalid row index selected!!");
                return;
            }
            bsData->getParentFile()->setIsChanged(true);
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE("KeyframeBonesModifierUI::removeKeyframeInfo(): Ranges is empty!!");
            return;
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::removeKeyframeInfo(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == HKB_KEY_FRAME_BONES_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbKeyframeBonesModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            if (bsData->keyframedBonesList.data()){
                keyframedBonesList->setChecked(true);
                keyframedBonesList->setText("Click to Edit");
            }else{
                keyframedBonesList->setChecked(false);
                keyframedBonesList->setText("nullptr");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            FATAL_RUNTIME_ERROR(QString("KeyframeBonesModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void KeyframeBonesModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_KEYFRAME_INFO_ROW + bsData->getNumberOfKeyframeInfos() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        for (int i = ADD_KEYFRAME_INFO_ROW + 1, j = 0; j < bsData->getNumberOfKeyframeInfos(); i++, j++){
            setRowItems(i, "KeyframeInfo "+QString::number(j), "hkKeyframeInfo", "Remove", "Edit", "Double click to remove this KeyframeInfo", "Double click to edit this KeyframeInfo");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void KeyframeBonesModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

bool KeyframeBonesModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void KeyframeBonesModifierUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void KeyframeBonesModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setName(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::setEnable(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::toggleKeyframedBonesList(bool enable){
    if (bsData){
        if (!enable){
            bsData->keyframedBonesList = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (!bsData->keyframedBonesList.data()){
            hkbBoneIndexArray *indices = new hkbBoneIndexArray(bsData->getParentFile());
            bsData->getParentFile()->addObjectToFile(indices, -1);
            bsData->keyframedBonesList = HkxSharedPtr(indices);
            //bones->setText(indices->getName());
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::toggleKeyframedBonesList(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::viewKeyframedBonesList(){
    if (bsData){
        boneIndexUI->loadData(bsData->keyframedBonesList.data());
        setCurrentIndex(BONE_INDEX_WIDGET);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::viewKeyframedBonesList(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_KEYFRAME_INFO_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                }
            }
        }else if (row == ADD_KEYFRAME_INFO_ROW && column == NAME_COLUMN){
            addKeyframeInfo();
        }else if (row > ADD_KEYFRAME_INFO_ROW && row < ADD_KEYFRAME_INFO_ROW + bsData->getNumberOfKeyframeInfos() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
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
                FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::viewSelectedChild(): Invalid index of range to view!!");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case KEYFRAME_WIDGET:
        keyframeInfoUI->setBindingVariable(index, name);
        break;
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
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(boneIndexUI, SIGNAL(viewRagdollBones(int)), ragdollBones, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void KeyframeBonesModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::loadBinding(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void KeyframeBonesModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("KeyframeBonesModifierUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                bindIndex = bind->getVariableIndexOfBinding("enable");
                if (bindIndex == index){
                    table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
                }
            }
        }else{
            keyframeInfoUI->variableRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeBonesModifierUI::variableRenamed(): The data is nullptr!!");
    }
}

