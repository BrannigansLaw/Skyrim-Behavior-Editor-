#include "gethandleonbonemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbgethandleonbonemodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define ENABLE_ROW 1
#define HANDLE_OUT_ROW 2
#define LOCAL_FRAME_NAME_ROW 3
#define RAGDOLL_BONE_INDEX_ROW 4
#define ANIMATION_BONE_INDEX_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList GetHandleOnBoneModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

GetHandleOnBoneModifierUI::GetHandleOnBoneModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      localFrameName(new ComboBox),
      ragdollBoneIndex(new ComboBox),
      animationBoneIndex(new ComboBox)
{
    setTitle("hkbGetHandleOnBoneModifier");
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
    table->setItem(HANDLE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("handleOut"));
    table->setItem(HANDLE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkHandle", Qt::AlignCenter));
    table->setItem(HANDLE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(HANDLE_OUT_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(LOCAL_FRAME_NAME_ROW, NAME_COLUMN, new TableWidgetItem("localFrameName"));
    table->setItem(LOCAL_FRAME_NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(LOCAL_FRAME_NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(LOCAL_FRAME_NAME_ROW, VALUE_COLUMN, localFrameName);
    table->setItem(RAGDOLL_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("ragdollBoneIndex"));
    table->setItem(RAGDOLL_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RAGDOLL_BONE_INDEX_ROW, VALUE_COLUMN, ragdollBoneIndex);
    table->setItem(ANIMATION_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("animationBoneIndex"));
    table->setItem(ANIMATION_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANIMATION_BONE_INDEX_ROW, VALUE_COLUMN, animationBoneIndex);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void GetHandleOnBoneModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(localFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setLocalFrameName(QString)), Qt::UniqueConnection);
    connect(ragdollBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setRagdollBoneIndex(int)), Qt::UniqueConnection);
    connect(animationBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnimationBoneIndex(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void GetHandleOnBoneModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(localFrameName, SIGNAL(currentTextChanged(QString)), this, SLOT(setLocalFrameName(QString)));
    disconnect(ragdollBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setRagdollBoneIndex(int)));
    disconnect(animationBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnimationBoneIndex(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void GetHandleOnBoneModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_GET_HANDLE_ON_BONE_MODIFIER){
            int index = -1;
            QStringList localFrames;
            localFrames.append("None");
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbGetHandleOnBoneModifier *>(data);
            name->setText(bsData->getName());
            enable->setChecked(bsData->enable);
            if (localFrameName->count() == 0){
                localFrames = localFrames + static_cast<BehaviorFile *>(bsData->getParentFile())->getLocalFrameNames();
                localFrameName->insertItems(0, localFrames);
            }
            index = localFrameName->findText(bsData->localFrameName);
            if (index < 0 || index >= localFrameName->count()){
                CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::loadData(): The local frame name was not loaded correctly!!!");
            }
            localFrameName->setCurrentIndex(index);
            if (ragdollBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                ragdollBoneIndex->insertItems(0, boneNames);
            }
            ragdollBoneIndex->setCurrentIndex(bsData->ragdollBoneIndex + 1);
            boneNames.clear();
            boneNames.append("None");
            if (animationBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                animationBoneIndex->insertItems(0, boneNames);
            }
            animationBoneIndex->setCurrentIndex(bsData->animationBoneIndex + 1);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(HANDLE_OUT_ROW, BINDING_COLUMN, varBind, "handleOut");
                loadBinding(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "ragdollBoneIndex");
                loadBinding(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "animationBoneIndex");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void GetHandleOnBoneModifierUI::setName(){
    if (bsData){
        if (bsData->getName() != name->text()){
            bsData->getName() = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setName(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setEnable(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::setLocalFrameName(const QString & name){
    if (bsData){
        bsData->localFrameName = name;
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setLocalFrameName(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::setRagdollBoneIndex(int index){
    if (bsData){
        bsData->ragdollBoneIndex = index - 1;
        if (bsData->ragdollBoneIndex > -1){   //ragdollBoneIndex and animationBoneIndex cannot simultaneously have nonnegative values!!!
            setAnimationBoneIndex(-1);
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setRagdollBoneIndex(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::setAnimationBoneIndex(int index){
    if (bsData){
        bsData->animationBoneIndex = index - 1;
        if (bsData->animationBoneIndex > -1){   //ragdollBoneIndex and animationBoneIndex cannot simultaneously have nonnegative values!!!
            setRagdollBoneIndex(-1);
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setAnimationBoneIndex(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::viewSelected(int row, int column){
    if (bsData){
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            case HANDLE_OUT_ROW:
                if (table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "handleOut");
                break;
            case RAGDOLL_BONE_INDEX_ROW:
                if (table->item(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "ragdollBoneIndex");
                break;
            case ANIMATION_BONE_INDEX_ROW:
                if (table->item(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "animationBoneIndex");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("handleOut");
            if (bindIndex == index){
                table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("ragdollBoneIndex");
            if (bindIndex == index){
                table->item(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("animationBoneIndex");
            if (bindIndex == index){
                table->item(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool GetHandleOnBoneModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = bsData->getVariableBindingSetData();
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void GetHandleOnBoneModifierUI::setBindingVariable(int index, const QString &name){
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
        case HANDLE_OUT_ROW:
            if (table->item(HANDLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handleOut", VARIABLE_TYPE_POINTER, isProperty);
            break;
        case RAGDOLL_BONE_INDEX_ROW:
            if (table->item(RAGDOLL_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "ragdollBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case ANIMATION_BONE_INDEX_ROW:
            if (table->item(ANIMATION_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "animationBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void GetHandleOnBoneModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, column)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, column)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("GetHandleOnBoneModifierUI::loadBinding(): The data is nullptr!!");
    }
}
