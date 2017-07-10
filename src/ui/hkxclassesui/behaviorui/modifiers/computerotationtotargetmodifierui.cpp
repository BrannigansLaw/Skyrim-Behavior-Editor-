#include "computerotationtotargetmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbComputeRotationToTargetModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 9

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ROTATION_OUT_ROW 2
#define TARGET_POSITION_ROW 3
#define CURRENT_POSITION_ROW 4
#define CURRENT_ROTATION_ROW 5
#define LOCAL_AXIS_OF_ROTATION_ROW 6
#define LOCAL_FACING_DIRECTION_ROW 7
#define RESULT_IS_DELTA_ROW 8

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ComputeRotationToTargetModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ComputeRotationToTargetModifierUI::ComputeRotationToTargetModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      rotationOut(new QuadVariableWidget),
      targetPosition(new QuadVariableWidget),
      currentPosition(new QuadVariableWidget),
      currentRotation(new QuadVariableWidget),
      localAxisOfRotation(new QuadVariableWidget),
      localFacingDirection(new QuadVariableWidget),
      resultIsDelta(new CheckBox)
{
    setTitle("hkbComputeRotationToTargetModifier");
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
    table->setItem(ROTATION_OUT_ROW, NAME_COLUMN, new TableWidgetItem("rotationOut"));
    table->setItem(ROTATION_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(ROTATION_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROTATION_OUT_ROW, VALUE_COLUMN, rotationOut);
    table->setItem(TARGET_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("targetPosition"));
    table->setItem(TARGET_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_POSITION_ROW, VALUE_COLUMN, targetPosition);
    table->setItem(CURRENT_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("currentPosition"));
    table->setItem(CURRENT_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(CURRENT_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CURRENT_POSITION_ROW, VALUE_COLUMN, currentPosition);
    table->setItem(CURRENT_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("currentRotation"));
    table->setItem(CURRENT_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(CURRENT_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CURRENT_ROTATION_ROW, VALUE_COLUMN, currentRotation);
    table->setItem(LOCAL_AXIS_OF_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("localAxisOfRotation"));
    table->setItem(LOCAL_AXIS_OF_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOCAL_AXIS_OF_ROTATION_ROW, VALUE_COLUMN, localAxisOfRotation);
    table->setItem(LOCAL_FACING_DIRECTION_ROW, NAME_COLUMN, new TableWidgetItem("localFacingDirection"));
    table->setItem(LOCAL_FACING_DIRECTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOCAL_FACING_DIRECTION_ROW, VALUE_COLUMN, localFacingDirection);
    table->setItem(RESULT_IS_DELTA_ROW, NAME_COLUMN, new TableWidgetItem("resultIsDelta"));
    table->setItem(RESULT_IS_DELTA_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(RESULT_IS_DELTA_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RESULT_IS_DELTA_ROW, VALUE_COLUMN, resultIsDelta);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void ComputeRotationToTargetModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setRotationOut()), Qt::UniqueConnection);
    connect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()), Qt::UniqueConnection);
    connect(currentPosition, SIGNAL(editingFinished()), this, SLOT(setCurrentPosition()), Qt::UniqueConnection);
    connect(currentRotation, SIGNAL(editingFinished()), this, SLOT(setCurrentRotation()), Qt::UniqueConnection);
    connect(localAxisOfRotation, SIGNAL(editingFinished()), this, SLOT(setLocalAxisOfRotation()), Qt::UniqueConnection);
    connect(localFacingDirection, SIGNAL(editingFinished()), this, SLOT(setLocalFacingDirection()), Qt::UniqueConnection);
    connect(resultIsDelta, SIGNAL(released()), this, SLOT(setResultIsDelta()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void ComputeRotationToTargetModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setRotationOut()));
    disconnect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()));
    disconnect(currentPosition, SIGNAL(editingFinished()), this, SLOT(setCurrentPosition()));
    disconnect(currentRotation, SIGNAL(editingFinished()), this, SLOT(setCurrentRotation()));
    disconnect(localAxisOfRotation, SIGNAL(editingFinished()), this, SLOT(setLocalAxisOfRotation()));
    disconnect(localFacingDirection, SIGNAL(editingFinished()), this, SLOT(setLocalFacingDirection()));
    disconnect(resultIsDelta, SIGNAL(released()), this, SLOT(setResultIsDelta()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void ComputeRotationToTargetModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void ComputeRotationToTargetModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_COMPUTE_ROTATION_TO_TARGET_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbComputeRotationToTargetModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            rotationOut->setValue(bsData->rotationOut);
            targetPosition->setValue(bsData->targetPosition);
            currentPosition->setValue(bsData->currentPosition);
            currentRotation->setValue(bsData->currentRotation);
            localAxisOfRotation->setValue(bsData->localAxisOfRotation);
            localFacingDirection->setValue(bsData->localFacingDirection);
            resultIsDelta->setChecked(bsData->resultIsDelta);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(ROTATION_OUT_ROW, BINDING_COLUMN, varBind, "rotationOut");
                loadBinding(TARGET_POSITION_ROW, BINDING_COLUMN, varBind, "targetPosition");
                loadBinding(CURRENT_POSITION_ROW, BINDING_COLUMN, varBind, "currentPosition");
                loadBinding(CURRENT_ROTATION_ROW, BINDING_COLUMN, varBind, "currentRotation");
                loadBinding(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN, varBind, "localAxisOfRotation");
                loadBinding(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN, varBind, "localFacingDirection");
                loadBinding(RESULT_IS_DELTA_ROW, BINDING_COLUMN, varBind, "resultIsDelta");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CURRENT_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CURRENT_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RESULT_IS_DELTA_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void ComputeRotationToTargetModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setName(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setEnable(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setRotationOut(){
    if (bsData){
        if (bsData->rotationOut != rotationOut->value()){
            bsData->rotationOut = rotationOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setrotationOut(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setTargetPosition(){
    if (bsData){
        if (bsData->targetPosition != targetPosition->value()){
            bsData->targetPosition = targetPosition->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::settargetPosition(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setCurrentPosition(){
    if (bsData){
        if (bsData->currentPosition != currentPosition->value()){
            bsData->currentPosition = currentPosition->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setcurrentPosition(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setCurrentRotation(){
    if (bsData){
        if (bsData->currentRotation != currentRotation->value()){
            bsData->currentRotation = currentRotation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setcurrentRotation(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setLocalAxisOfRotation(){
    if (bsData){
        if (bsData->localAxisOfRotation != localAxisOfRotation->value()){
            bsData->localAxisOfRotation = localAxisOfRotation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setlocalAxisOfRotation(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setLocalFacingDirection(){
    if (bsData){
        if (bsData->localFacingDirection != localFacingDirection->value()){
            bsData->localFacingDirection = localFacingDirection->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setlocalFacingDirection(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::setResultIsDelta(){
    if (bsData){
        if (bsData->resultIsDelta != resultIsDelta->isChecked()){
            bsData->resultIsDelta = resultIsDelta->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setresultIsDelta(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::viewSelected(int row, int column){
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
            case ROTATION_OUT_ROW:
                if (table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotationOut");
                break;
            case TARGET_POSITION_ROW:
                if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetPosition");
                break;
            case CURRENT_POSITION_ROW:
                if (table->item(CURRENT_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "currentPosition");
                break;
            case CURRENT_ROTATION_ROW:
                if (table->item(CURRENT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "currentRotation");
                break;
            case LOCAL_AXIS_OF_ROTATION_ROW:
                if (table->item(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "localAxisOfRotation");
                break;
            case LOCAL_FACING_DIRECTION_ROW:
                if (table->item(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "localFacingDirection");
                break;
            case RESULT_IS_DELTA_ROW:
                if (table->item(RESULT_IS_DELTA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "resultIsDelta");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void ComputeRotationToTargetModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rotationOut");
            if (bindIndex == index){
                table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetPosition");
            if (bindIndex == index){
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("currentPosition");
            if (bindIndex == index){
                table->item(CURRENT_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("currentRotation");
            if (bindIndex == index){
                table->item(CURRENT_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("localAxisOfRotation");
            if (bindIndex == index){
                table->item(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("localFacingDirection");
            if (bindIndex == index){
                table->item(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("resultIsDelta");
            if (bindIndex == index){
                table->item(RESULT_IS_DELTA_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool ComputeRotationToTargetModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void ComputeRotationToTargetModifierUI::setBindingVariable(int index, const QString &name){
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
        case ROTATION_OUT_ROW:
            if (table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotationOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TARGET_POSITION_ROW:
            if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case CURRENT_POSITION_ROW:
            if (table->item(CURRENT_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "currentPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case CURRENT_ROTATION_ROW:
            if (table->item(CURRENT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "currentRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case LOCAL_AXIS_OF_ROTATION_ROW:
            if (table->item(LOCAL_AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "localAxisOfRotation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case LOCAL_FACING_DIRECTION_ROW:
            if (table->item(LOCAL_FACING_DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "localFacingDirection", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case RESULT_IS_DELTA_ROW:
            if (table->item(RESULT_IS_DELTA_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "resultIsDelta", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void ComputeRotationToTargetModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("ComputeRotationToTargetModifierUI::loadBinding(): The data is NULL!!"));
    }
}
