#include "evaluatehandlemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbevaluatehandlemodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 9

#define NAME_ROW 0
#define ENABLE_ROW 1
#define HANDLE_ROW 2
#define HANDLE_POSITION_OUT_ROW 3
#define HANDLE_ROTATION_OUT_ROW 4
#define IS_VALID_OUT_ROW 5
#define EXTRAPOLATION_TIME_STEP_ROW 6
#define HANDLE_CHANGE_SPEED_ROW 7
#define HANDLE_CHANGE_MODE_ROW 8

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList EvaluateHandleModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

EvaluateHandleModifierUI::EvaluateHandleModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      handlePositionOut(new QuadVariableWidget),
      handleRotationOut(new QuadVariableWidget),
      isValidOut(new CheckBox),
      extrapolationTimeStep(new DoubleSpinBox),
      handleChangeSpeed(new DoubleSpinBox),
      handleChangeMode(new ComboBox)
{
    setTitle("hkbEvaluateHandleModifier");
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
    table->setItem(HANDLE_ROW, NAME_COLUMN, new TableWidgetItem("handle"));
    table->setItem(HANDLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkHandle", Qt::AlignCenter));
    table->setItem(HANDLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(HANDLE_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(HANDLE_POSITION_OUT_ROW, NAME_COLUMN, new TableWidgetItem("handlePositionOut"));
    table->setItem(HANDLE_POSITION_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HANDLE_POSITION_OUT_ROW, VALUE_COLUMN, handlePositionOut);
    table->setItem(HANDLE_ROTATION_OUT_ROW, NAME_COLUMN, new TableWidgetItem("handleRotationOut"));
    table->setItem(HANDLE_ROTATION_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HANDLE_ROTATION_OUT_ROW, VALUE_COLUMN, handleRotationOut);
    table->setItem(IS_VALID_OUT_ROW, NAME_COLUMN, new TableWidgetItem("isValidOut"));
    table->setItem(IS_VALID_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_VALID_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_VALID_OUT_ROW, VALUE_COLUMN, isValidOut);
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, NAME_COLUMN, new TableWidgetItem("extrapolationTimeStep"));
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(EXTRAPOLATION_TIME_STEP_ROW, VALUE_COLUMN, extrapolationTimeStep);
    table->setItem(HANDLE_CHANGE_SPEED_ROW, NAME_COLUMN, new TableWidgetItem("handleChangeSpeed"));
    table->setItem(HANDLE_CHANGE_SPEED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HANDLE_CHANGE_SPEED_ROW, VALUE_COLUMN, handleChangeSpeed);
    table->setItem(HANDLE_CHANGE_MODE_ROW, NAME_COLUMN, new TableWidgetItem("handleChangeMode"));
    table->setItem(HANDLE_CHANGE_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("HandleChangeMode", Qt::AlignCenter));
    table->setItem(HANDLE_CHANGE_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(HANDLE_CHANGE_MODE_ROW, VALUE_COLUMN, handleChangeMode);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void EvaluateHandleModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(handlePositionOut, SIGNAL(editingFinished()), this, SLOT(setHandlePositionOut()), Qt::UniqueConnection);
    connect(handleRotationOut, SIGNAL(editingFinished()), this, SLOT(setHandleRotationOut()), Qt::UniqueConnection);
    connect(isValidOut, SIGNAL(released()), this, SLOT(setIsValidOut()), Qt::UniqueConnection);
    connect(extrapolationTimeStep, SIGNAL(editingFinished()), this, SLOT(setExtrapolationTimeStep()), Qt::UniqueConnection);
    connect(handleChangeSpeed, SIGNAL(editingFinished()), this, SLOT(setHandleChangeSpeed()), Qt::UniqueConnection);
    connect(handleChangeMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setHandleChangeMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void EvaluateHandleModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(handlePositionOut, SIGNAL(editingFinished()), this, SLOT(setHandlePositionOut()));
    disconnect(handleRotationOut, SIGNAL(editingFinished()), this, SLOT(setHandleRotationOut()));
    disconnect(isValidOut, SIGNAL(released()), this, SLOT(setIsValidOut()));
    disconnect(extrapolationTimeStep, SIGNAL(editingFinished()), this, SLOT(setExtrapolationTimeStep()));
    disconnect(handleChangeSpeed, SIGNAL(editingFinished()), this, SLOT(setHandleChangeSpeed()));
    disconnect(handleChangeMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setHandleChangeMode(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void EvaluateHandleModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void EvaluateHandleModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_EVALUATE_HANDLE_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbEvaluateHandleModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            handlePositionOut->setValue(bsData->handlePositionOut);
            handleRotationOut->setValue(bsData->handleRotationOut);
            isValidOut->setChecked(bsData->isValidOut);
            extrapolationTimeStep->setValue(bsData->extrapolationTimeStep);
            handleChangeSpeed->setValue(bsData->handleChangeSpeed);
            if (handleChangeMode->count() == 0){
                handleChangeMode->insertItems(0, bsData->HandleChangeMode);
            }
            handleChangeMode->setCurrentIndex(bsData->HandleChangeMode.indexOf(bsData->handleChangeMode));
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(HANDLE_ROW, BINDING_COLUMN, varBind, "handle");
                loadBinding(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN, varBind, "handlePositionOut");
                loadBinding(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN, varBind, "handleRotationOut");
                loadBinding(IS_VALID_OUT_ROW, BINDING_COLUMN, varBind, "isValidOut");
                loadBinding(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN, varBind, "extrapolationTimeStep");
                loadBinding(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN, varBind, "handleChangeSpeed");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_VALID_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void EvaluateHandleModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setName(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setEnable(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setHandlePositionOut(){
    if (bsData){
        if (bsData->handlePositionOut != handlePositionOut->value()){
            bsData->handlePositionOut = handlePositionOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setHandlePositionOut(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setHandleRotationOut(){
    if (bsData){
        if (bsData->handleRotationOut != handleRotationOut->value()){
            bsData->handleRotationOut = handleRotationOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setHandleRotationOut(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setIsValidOut(){
    if (bsData){
        if (bsData->isValidOut != isValidOut->isChecked()){
            bsData->isValidOut = isValidOut->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setIsValidOut(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setExtrapolationTimeStep(){
    if (bsData){
        if (bsData->extrapolationTimeStep != extrapolationTimeStep->value()){
            bsData->extrapolationTimeStep = extrapolationTimeStep->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setExtrapolationTimeStep(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setHandleChangeSpeed(){
    if (bsData){
        if (bsData->handleChangeSpeed != handleChangeSpeed->value()){
            bsData->handleChangeSpeed = handleChangeSpeed->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setHandleChangeSpeed(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::setHandleChangeMode(int index){
    if (bsData){
        bsData->handleChangeMode = bsData->HandleChangeMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setHandleChangeMode(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::viewSelected(int row, int column){
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
            case HANDLE_ROW:
                if (table->item(HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "handle");
                break;
            case HANDLE_POSITION_OUT_ROW:
                if (table->item(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "handlePositionOut");
                break;
            case HANDLE_ROTATION_OUT_ROW:
                if (table->item(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "handleRotationOut");
                break;
            case IS_VALID_OUT_ROW:
                if (table->item(IS_VALID_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "isValidOut");
                break;
            case EXTRAPOLATION_TIME_STEP_ROW:
                if (table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "extrapolationTimeStep");
                break;
            case HANDLE_CHANGE_SPEED_ROW:
                if (table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "handleChangeSpeed");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void EvaluateHandleModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("handle");
            if (bindIndex == index){
                table->item(HANDLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("handlePositionOut");
            if (bindIndex == index){
                table->item(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("handleRotationOut");
            if (bindIndex == index){
                table->item(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("isValidOut");
            if (bindIndex == index){
                table->item(IS_VALID_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("extrapolationTimeStep");
            if (bindIndex == index){
                table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("handleChangeSpeed");
            if (bindIndex == index){
                table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool EvaluateHandleModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void EvaluateHandleModifierUI::setBindingVariable(int index, const QString &name){
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
        case HANDLE_ROW:
            if (table->item(HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handle", VARIABLE_TYPE_POINTER, isProperty);
            break;
        case HANDLE_POSITION_OUT_ROW:
            if (table->item(HANDLE_POSITION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handlePositionOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case HANDLE_ROTATION_OUT_ROW:
            if (table->item(HANDLE_ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handleRotationOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case IS_VALID_OUT_ROW:
            if (table->item(IS_VALID_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "isValidOut", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case EXTRAPOLATION_TIME_STEP_ROW:
            if (table->item(EXTRAPOLATION_TIME_STEP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "extrapolationTimeStep", VARIABLE_TYPE_REAL, isProperty);
            break;
        case HANDLE_CHANGE_SPEED_ROW:
            if (table->item(HANDLE_CHANGE_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "handleChangeSpeed", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void EvaluateHandleModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EvaluateHandleModifierUI::loadBinding(): The data is NULL!!"));
    }
}

