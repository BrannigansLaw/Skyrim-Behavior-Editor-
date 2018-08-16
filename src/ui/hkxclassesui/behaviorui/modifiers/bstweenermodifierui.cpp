#include "bstweenermodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/BSTweenerModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define ENABLE_ROW 1
#define TWEEN_POSITION_ROW 2
#define TWEEN_ROTATION_ROW 3
#define USE_TWEEN_DURATION_ROW 4
#define TWEEN_DURATION_ROW 5
#define TARGET_POSITION_ROW 6
#define TARGET_ROTATION_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSTweenerModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSTweenerModifierUI::BSTweenerModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      tweenPosition(new CheckBox),
      tweenRotation(new CheckBox),
      useTweenDuration(new CheckBox),
      tweenDuration(new DoubleSpinBox),
      targetPosition(new QuadVariableWidget),
      targetRotation(new QuadVariableWidget)
{
    setTitle("BSTweenerModifier");
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
    table->setItem(TWEEN_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("tweenPosition"));
    table->setItem(TWEEN_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(TWEEN_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TWEEN_POSITION_ROW, VALUE_COLUMN, tweenPosition);
    table->setItem(TWEEN_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("tweenRotation"));
    table->setItem(TWEEN_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(TWEEN_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TWEEN_ROTATION_ROW, VALUE_COLUMN, tweenRotation);
    table->setItem(USE_TWEEN_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("useTweenDuration"));
    table->setItem(USE_TWEEN_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(USE_TWEEN_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(USE_TWEEN_DURATION_ROW, VALUE_COLUMN, useTweenDuration);
    table->setItem(TWEEN_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("tweenDuration"));
    table->setItem(TWEEN_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TWEEN_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TWEEN_DURATION_ROW, VALUE_COLUMN, tweenDuration);
    table->setItem(TARGET_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("targetPosition"));
    table->setItem(TARGET_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_POSITION_ROW, VALUE_COLUMN, targetPosition);
    table->setItem(TARGET_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("targetRotation"));
    table->setItem(TARGET_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(TARGET_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_ROTATION_ROW, VALUE_COLUMN, targetRotation);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSTweenerModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(tweenPosition, SIGNAL(released()), this, SLOT(setTweenPosition()), Qt::UniqueConnection);
    connect(tweenRotation, SIGNAL(released()), this, SLOT(setTweenRotation()), Qt::UniqueConnection);
    connect(useTweenDuration, SIGNAL(released()), this, SLOT(setUseTweenDuration()), Qt::UniqueConnection);
    connect(tweenDuration, SIGNAL(editingFinished()), this, SLOT(setTweenDuration()), Qt::UniqueConnection);
    connect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()), Qt::UniqueConnection);
    connect(targetRotation, SIGNAL(editingFinished()), this, SLOT(setTargetRotation()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSTweenerModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(tweenPosition, SIGNAL(released()), this, SLOT(setTweenPosition()));
    disconnect(tweenRotation, SIGNAL(released()), this, SLOT(setTweenRotation()));
    disconnect(useTweenDuration, SIGNAL(released()), this, SLOT(setUseTweenDuration()));
    disconnect(tweenDuration, SIGNAL(editingFinished()), this, SLOT(setTweenDuration()));
    disconnect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()));
    disconnect(targetRotation, SIGNAL(editingFinished()), this, SLOT(setTargetRotation()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSTweenerModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSTweenerModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_TWEENER_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<BSTweenerModifier *>(data);
            name->setText(bsData->getName());
            enable->setChecked(bsData->enable);
            tweenPosition->setChecked(bsData->tweenPosition);
            tweenRotation->setChecked(bsData->tweenRotation);
            useTweenDuration->setChecked(bsData->useTweenDuration);
            tweenDuration->setValue(bsData->tweenDuration);
            targetPosition->setValue(bsData->targetPosition);
            targetRotation->setValue(bsData->targetRotation);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(TWEEN_POSITION_ROW, BINDING_COLUMN, varBind, "tweenPosition");
                loadBinding(TWEEN_ROTATION_ROW, BINDING_COLUMN, varBind, "tweenRotation");
                loadBinding(USE_TWEEN_DURATION_ROW, BINDING_COLUMN, varBind, "useTweenDuration");
                loadBinding(TWEEN_DURATION_ROW, BINDING_COLUMN, varBind, "tweenDuration");
                loadBinding(TARGET_POSITION_ROW, BINDING_COLUMN, varBind, "targetPosition");
                loadBinding(TARGET_ROTATION_ROW, BINDING_COLUMN, varBind, "targetRotation");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TWEEN_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TWEEN_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(USE_TWEEN_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TWEEN_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void BSTweenerModifierUI::setName(){
    if (bsData){
        if (bsData->getName() != name->text()){
            bsData->getName() = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setName(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setEnable(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setTweenPosition(){
    if (bsData){
        bsData->tweenPosition = tweenPosition->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::settweenPosition(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setTweenRotation(){
    if (bsData){
        bsData->tweenRotation = tweenRotation->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::settweenRotation(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setUseTweenDuration(){
    if (bsData){
        bsData->useTweenDuration = useTweenDuration->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setuseTweenDuration(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setTweenDuration(){
    if (bsData){
        if (bsData->tweenDuration != tweenDuration->value()){
            bsData->tweenDuration = tweenDuration->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::settweenDuration(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setTargetPosition(){
    if (bsData){
        if (bsData->targetPosition != targetPosition->value()){
            bsData->targetPosition = targetPosition->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::settargetPosition(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::setTargetRotation(){
    if (bsData){
        if (bsData->targetRotation != targetRotation->value()){
            bsData->targetRotation = targetRotation->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::settargetRotation(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::viewSelected(int row, int column){
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
            case TWEEN_POSITION_ROW:
                if (table->item(TWEEN_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "tweenPosition");
                break;
            case TWEEN_ROTATION_ROW:
                if (table->item(TWEEN_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "tweenRotation");
                break;
            case USE_TWEEN_DURATION_ROW:
                if (table->item(USE_TWEEN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "useTweenDuration");
                break;
            case TWEEN_DURATION_ROW:
                if (table->item(TWEEN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "tweenDuration");
                break;
            case TARGET_POSITION_ROW:
                if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetPosition");
                break;
            case TARGET_ROTATION_ROW:
                if (table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetRotation");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BSTweenerModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("tweenPosition");
            if (bindIndex == index){
                table->item(TWEEN_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("tweenRotation");
            if (bindIndex == index){
                table->item(TWEEN_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("useTweenDuration");
            if (bindIndex == index){
                table->item(USE_TWEEN_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("tweenDuration");
            if (bindIndex == index){
                table->item(TWEEN_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetPosition");
            if (bindIndex == index){
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetRotation");
            if (bindIndex == index){
                table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool BSTweenerModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void BSTweenerModifierUI::setBindingVariable(int index, const QString &name){
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
        case TWEEN_POSITION_ROW:
            if (table->item(TWEEN_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "tweenPosition", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case TWEEN_ROTATION_ROW:
            if (table->item(TWEEN_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "tweenRotation", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case USE_TWEEN_DURATION_ROW:
            if (table->item(USE_TWEEN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "useTweenDuration", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case TWEEN_DURATION_ROW:
            if (table->item(TWEEN_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "tweenDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case TARGET_POSITION_ROW:
            if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TARGET_ROTATION_ROW:
            if (table->item(TARGET_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void BSTweenerModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSTweenerModifierUI::loadBinding(): The data is nullptr!!");
    }
}
