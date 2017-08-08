#include "bsspeedsamplermodifierui.h"


#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/BSSpeedSamplerModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define ENABLE_ROW 1
#define STATE_ROW 2
#define DIRECTION_ROW 3
#define GOAL_SPEED_ROW 4
#define SPEED_OUT_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSSpeedSamplerModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSSpeedSamplerModifierUI::BSSpeedSamplerModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      state(new DoubleSpinBox),
      direction(new DoubleSpinBox),
      goalSpeed(new DoubleSpinBox),
      speedOut(new DoubleSpinBox)
{
    setTitle("BSSpeedSamplerModifier");
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
    table->setItem(STATE_ROW, NAME_COLUMN, new TableWidgetItem("state"));
    table->setItem(STATE_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(STATE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(STATE_ROW, VALUE_COLUMN, state);
    table->setItem(DIRECTION_ROW, NAME_COLUMN, new TableWidgetItem("direction"));
    table->setItem(DIRECTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DIRECTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DIRECTION_ROW, VALUE_COLUMN, direction);
    table->setItem(GOAL_SPEED_ROW, NAME_COLUMN, new TableWidgetItem("goalSpeed"));
    table->setItem(GOAL_SPEED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GOAL_SPEED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GOAL_SPEED_ROW, VALUE_COLUMN, goalSpeed);
    table->setItem(SPEED_OUT_ROW, NAME_COLUMN, new TableWidgetItem("speedOut"));
    table->setItem(SPEED_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SPEED_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SPEED_OUT_ROW, VALUE_COLUMN, speedOut);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSSpeedSamplerModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(state, SIGNAL(editingFinished()), this, SLOT(setState()), Qt::UniqueConnection);
    connect(direction, SIGNAL(editingFinished()), this, SLOT(setDirection()), Qt::UniqueConnection);
    connect(goalSpeed, SIGNAL(editingFinished()), this, SLOT(setGoalSpeed()), Qt::UniqueConnection);
    connect(speedOut, SIGNAL(editingFinished()), this, SLOT(setSpeedOut()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSSpeedSamplerModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(state, SIGNAL(editingFinished()), this, SLOT(setState()));
    disconnect(direction, SIGNAL(editingFinished()), this, SLOT(setDirection()));
    disconnect(goalSpeed, SIGNAL(editingFinished()), this, SLOT(setGoalSpeed()));
    disconnect(speedOut, SIGNAL(editingFinished()), this, SLOT(setSpeedOut()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSSpeedSamplerModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSSpeedSamplerModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_SPEED_SAMPLER_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<BSSpeedSamplerModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            state->setValue(bsData->state);
            direction->setValue(bsData->direction);
            goalSpeed->setValue(bsData->goalSpeed);
            speedOut->setValue(bsData->speedOut);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(STATE_ROW, BINDING_COLUMN, varBind, "state");
                loadBinding(DIRECTION_ROW, BINDING_COLUMN, varBind, "direction");
                loadBinding(GOAL_SPEED_ROW, BINDING_COLUMN, varBind, "goalSpeed");
                loadBinding(SPEED_OUT_ROW, BINDING_COLUMN, varBind, "speedOut");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(STATE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DIRECTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GOAL_SPEED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SPEED_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSSpeedSamplerModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setName(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::setState(){
    if (bsData){
        if (bsData->state != state->value()){
            bsData->state = state->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setstate(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::setDirection(){
    if (bsData){
        if (bsData->direction != direction->value()){
            bsData->direction = direction->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setdirection(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::setGoalSpeed(){
    if (bsData){
        if (bsData->goalSpeed != goalSpeed->value()){
            bsData->goalSpeed = goalSpeed->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setgoalSpeed(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::setSpeedOut(){
    if (bsData){
        if (bsData->speedOut != speedOut->value()){
            bsData->speedOut = speedOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setspeedOut(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::viewSelected(int row, int column){
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
            case STATE_ROW:
                if (table->item(STATE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "state");
                break;
            case DIRECTION_ROW:
                if (table->item(DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "direction");
                break;
            case GOAL_SPEED_ROW:
                if (table->item(GOAL_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "goalSpeed");
                break;
            case SPEED_OUT_ROW:
                if (table->item(SPEED_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "speedOut");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSSpeedSamplerModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("state");
            if (bindIndex == index){
                table->item(STATE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("direction");
            if (bindIndex == index){
                table->item(DIRECTION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("goalSpeed");
            if (bindIndex == index){
                table->item(GOAL_SPEED_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("speedOut");
            if (bindIndex == index){
                table->item(SPEED_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSSpeedSamplerModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSSpeedSamplerModifierUI::setBindingVariable(int index, const QString &name){
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
        case STATE_ROW:
            if (table->item(STATE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "state", VARIABLE_TYPE_INT32, isProperty);
            break;
        case DIRECTION_ROW:
            if (table->item(DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "direction", VARIABLE_TYPE_REAL, isProperty);
            break;
        case GOAL_SPEED_ROW:
            if (table->item(GOAL_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "goalSpeed", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SPEED_OUT_ROW:
            if (table->item(SPEED_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "speedOut", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSSpeedSamplerModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSSpeedSamplerModifierUI::loadBinding(): The data is NULL!!"));
    }
}
