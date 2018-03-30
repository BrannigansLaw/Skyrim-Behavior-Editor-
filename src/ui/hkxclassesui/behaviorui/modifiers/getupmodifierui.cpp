#include "getupmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbgetupmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define ENABLE_ROW 1
#define GROUND_NORMAL_ROW 2
#define DURATION_ROW 3
#define ALIGN_WITH_GROUND_DURATION_ROW 4
#define ROOT_BONE_INDEX_ROW 5
#define OTHER_BONE_INDEX_ROW 6
#define ANOTHER_BONE_INDEX_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList GetUpModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

GetUpModifierUI::GetUpModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      groundNormal(new QuadVariableWidget),
      duration(new DoubleSpinBox),
      alignWithGroundDuration(new DoubleSpinBox),
      rootBoneIndex(new ComboBox),
      otherBoneIndex(new ComboBox),
      anotherBoneIndex(new ComboBox)
{
    setTitle("hkbGetUpModifier");
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
    table->setItem(GROUND_NORMAL_ROW, NAME_COLUMN, new TableWidgetItem("groundNormal"));
    table->setItem(GROUND_NORMAL_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(GROUND_NORMAL_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GROUND_NORMAL_ROW, VALUE_COLUMN, groundNormal);
    table->setItem(DURATION_ROW, NAME_COLUMN, new TableWidgetItem("duration"));
    table->setItem(DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DURATION_ROW, VALUE_COLUMN, duration);
    table->setItem(ALIGN_WITH_GROUND_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("alignWithGroundDuration"));
    table->setItem(ALIGN_WITH_GROUND_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ALIGN_WITH_GROUND_DURATION_ROW, VALUE_COLUMN, alignWithGroundDuration);
    table->setItem(ROOT_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("rootBoneIndex"));
    table->setItem(ROOT_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ROOT_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROOT_BONE_INDEX_ROW, VALUE_COLUMN, rootBoneIndex);
    table->setItem(OTHER_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("otherBoneIndex"));
    table->setItem(OTHER_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(OTHER_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OTHER_BONE_INDEX_ROW, VALUE_COLUMN, otherBoneIndex);
    table->setItem(ANOTHER_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("anotherBoneIndex"));
    table->setItem(ANOTHER_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANOTHER_BONE_INDEX_ROW, VALUE_COLUMN, anotherBoneIndex);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void GetUpModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(groundNormal, SIGNAL(editingFinished()), this, SLOT(setGroundNormal()), Qt::UniqueConnection);
    connect(duration, SIGNAL(currentIndexChanged(int)), this, SLOT(setDuration(int)), Qt::UniqueConnection);
    connect(alignWithGroundDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlignWithGroundDuration(int)), Qt::UniqueConnection);
    connect(rootBoneIndex, SIGNAL(editingFinished()), this, SLOT(setRootBoneIndex()), Qt::UniqueConnection);
    connect(otherBoneIndex, SIGNAL(editingFinished()), this, SLOT(setOtherBoneIndex()), Qt::UniqueConnection);
    connect(anotherBoneIndex, SIGNAL(editingFinished()), this, SLOT(setAnotherBoneIndex()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void GetUpModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(groundNormal, SIGNAL(editingFinished()), this, SLOT(setGroundNormal()));
    disconnect(duration, SIGNAL(currentIndexChanged(int)), this, SLOT(setDuration(int)));
    disconnect(alignWithGroundDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(setAlignWithGroundDuration(int)));
    disconnect(rootBoneIndex, SIGNAL(editingFinished()), this, SLOT(setRootBoneIndex()));
    disconnect(otherBoneIndex, SIGNAL(editingFinished()), this, SLOT(setOtherBoneIndex()));
    disconnect(anotherBoneIndex, SIGNAL(editingFinished()), this, SLOT(setAnotherBoneIndex()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void GetUpModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void GetUpModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_GET_UP_MODIFIER){
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbGetUpModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            groundNormal->setValue(bsData->groundNormal);
            duration->setValue(bsData->duration);
            alignWithGroundDuration->setValue(bsData->alignWithGroundDuration);
            if (rootBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                rootBoneIndex->insertItems(0, boneNames);
            }
            rootBoneIndex->setCurrentIndex(bsData->rootBoneIndex + 1);
            if (otherBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                otherBoneIndex->insertItems(0, boneNames);
            }
            otherBoneIndex->setCurrentIndex(bsData->otherBoneIndex + 1);
            if (anotherBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRagdollBoneNames();
                anotherBoneIndex->insertItems(0, boneNames);
            }
            anotherBoneIndex->setCurrentIndex(bsData->anotherBoneIndex + 1);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(GROUND_NORMAL_ROW, BINDING_COLUMN, varBind, "groundNormal");
                loadBinding(DURATION_ROW, BINDING_COLUMN, varBind, "duration");
                loadBinding(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN, varBind, "alignWithGroundDuration");
                loadBinding(ROOT_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "rootBoneIndex");
                loadBinding(OTHER_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "otherBoneIndex");
                loadBinding(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "anotherBoneIndex");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GROUND_NORMAL_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            FATAL_RUNTIME_ERROR("GetUpModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void GetUpModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setName(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setEnable(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setGroundNormal(){
    if (bsData){
        if (bsData->groundNormal != groundNormal->value()){
            bsData->groundNormal = groundNormal->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setgroundNormal(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setDuration(){
    if (bsData){
        if (bsData->duration != duration->value()){
            bsData->duration = duration->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setduration(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setAlignWithGroundDuration(){
    if (bsData){
        if (bsData->alignWithGroundDuration != alignWithGroundDuration->value()){
            bsData->alignWithGroundDuration = alignWithGroundDuration->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setalignWithGroundDuration(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setRootBoneIndex(int index){
    if (bsData){
        bsData->rootBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setrootBoneIndex(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setOtherBoneIndex(int index){
    if (bsData){
        bsData->otherBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setotherBoneIndex(): The data is nullptr!!");
    }
}

void GetUpModifierUI::setAnotherBoneIndex(int index){
    if (bsData){
        bsData->anotherBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setanotherBoneIndex(): The data is nullptr!!");
    }
}

void GetUpModifierUI::viewSelected(int row, int column){
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
            case GROUND_NORMAL_ROW:
                if (table->item(GROUND_NORMAL_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "groundNormal");
                break;
            case DURATION_ROW:
                if (table->item(DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "duration");
                break;
            case ALIGN_WITH_GROUND_DURATION_ROW:
                if (table->item(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "alignWithGroundDuration");
                break;
            case ROOT_BONE_INDEX_ROW:
                if (table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rootBoneIndex");
                break;
            case OTHER_BONE_INDEX_ROW:
                if (table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "otherBoneIndex");
                break;
            case ANOTHER_BONE_INDEX_ROW:
                if (table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "anotherBoneIndex");
                break;
            default:
                return;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void GetUpModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void GetUpModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("groundNormal");
            if (bindIndex == index){
                table->item(GROUND_NORMAL_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("duration");
            if (bindIndex == index){
                table->item(DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("alignWithGroundDuration");
            if (bindIndex == index){
                table->item(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rootBoneIndex");
            if (bindIndex == index){
                table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("otherBoneIndex");
            if (bindIndex == index){
                table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("anotherBoneIndex");
            if (bindIndex == index){
                table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool GetUpModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("GetUpModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("GetUpModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void GetUpModifierUI::setBindingVariable(int index, const QString &name){
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
        case GROUND_NORMAL_ROW:
            if (table->item(GROUND_NORMAL_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "groundNormal", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case DURATION_ROW:
            if (table->item(DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "duration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ALIGN_WITH_GROUND_DURATION_ROW:
            if (table->item(ALIGN_WITH_GROUND_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "alignWithGroundDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ROOT_BONE_INDEX_ROW:
            if (table->item(ROOT_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rootBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case OTHER_BONE_INDEX_ROW:
            if (table->item(OTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "otherBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case ANOTHER_BONE_INDEX_ROW:
            if (table->item(ANOTHER_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "anotherBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void GetUpModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("GetUpModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("GetUpModifierUI::loadBinding(): The data is nullptr!!");
    }
}
