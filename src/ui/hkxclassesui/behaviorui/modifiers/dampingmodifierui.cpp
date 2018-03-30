#include "dampingmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbDampingModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 15

#define NAME_ROW 0
#define ENABLE_ROW 1
#define KP_ROW 2
#define KI_ROW 3
#define KD_ROW 4
#define ENABLE_SCALAR_DAMPING_ROW 5
#define ENABLE_VECTOR_DAMPING_ROW 6
#define RAW_VALUE_ROW 7
#define DAMPED_VALUE_ROW 8
#define RAW_VECTOR_ROW 9
#define DAMPED_VECTOR_ROW 10
#define VEC_ERROR_SUM_ROW 11
#define VEC_PREVIOUS_ERROR_ROW 12
#define ERROR_SUM_ROW 13
#define PREVIOUS_ERROR_ROW 14

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList DampingModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

DampingModifierUI::DampingModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      kP(new DoubleSpinBox),
      kI(new DoubleSpinBox),
      kD(new DoubleSpinBox),
      enableScalarDamping(new CheckBox),
      enableVectorDamping(new CheckBox),
      rawValue(new DoubleSpinBox),
      dampedValue(new DoubleSpinBox),
      rawVector(new QuadVariableWidget),
      dampedVector(new QuadVariableWidget),
      vecErrorSum(new QuadVariableWidget),
      vecPreviousError(new QuadVariableWidget),
      errorSum(new DoubleSpinBox),
      previousError(new DoubleSpinBox)
{
    setTitle("hkbDampingModifier");
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
    table->setItem(KP_ROW, NAME_COLUMN, new TableWidgetItem("kP"));
    table->setItem(KP_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(KP_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(KP_ROW, VALUE_COLUMN, kP);
    table->setItem(KI_ROW, NAME_COLUMN, new TableWidgetItem("kI"));
    table->setItem(KI_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(KI_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(KI_ROW, VALUE_COLUMN, kI);
    table->setItem(KD_ROW, NAME_COLUMN, new TableWidgetItem("kD"));
    table->setItem(KD_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(KD_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(KD_ROW, VALUE_COLUMN, kD);
    table->setItem(ENABLE_SCALAR_DAMPING_ROW, NAME_COLUMN, new TableWidgetItem("enableScalarDamping"));
    table->setItem(ENABLE_SCALAR_DAMPING_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_SCALAR_DAMPING_ROW, VALUE_COLUMN, enableScalarDamping);
    table->setItem(ENABLE_VECTOR_DAMPING_ROW, NAME_COLUMN, new TableWidgetItem("enableVectorDamping"));
    table->setItem(ENABLE_VECTOR_DAMPING_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_VECTOR_DAMPING_ROW, VALUE_COLUMN, enableVectorDamping);
    table->setItem(RAW_VALUE_ROW, NAME_COLUMN, new TableWidgetItem("rawValue"));
    table->setItem(RAW_VALUE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(RAW_VALUE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RAW_VALUE_ROW, VALUE_COLUMN, rawValue);
    table->setItem(DAMPED_VALUE_ROW, NAME_COLUMN, new TableWidgetItem("dampedValue"));
    table->setItem(DAMPED_VALUE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DAMPED_VALUE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DAMPED_VALUE_ROW, VALUE_COLUMN, dampedValue);
    table->setItem(RAW_VECTOR_ROW, NAME_COLUMN, new TableWidgetItem("rawVector"));
    table->setItem(RAW_VECTOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(RAW_VECTOR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RAW_VECTOR_ROW, VALUE_COLUMN, rawVector);
    table->setItem(DAMPED_VECTOR_ROW, NAME_COLUMN, new TableWidgetItem("dampedVector"));
    table->setItem(DAMPED_VECTOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(DAMPED_VECTOR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DAMPED_VECTOR_ROW, VALUE_COLUMN, dampedVector);
    table->setItem(VEC_ERROR_SUM_ROW, NAME_COLUMN, new TableWidgetItem("vecErrorSum"));
    table->setItem(VEC_ERROR_SUM_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(VEC_ERROR_SUM_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VEC_ERROR_SUM_ROW, VALUE_COLUMN, vecErrorSum);
    table->setItem(VEC_PREVIOUS_ERROR_ROW, NAME_COLUMN, new TableWidgetItem("vecPreviousError"));
    table->setItem(VEC_PREVIOUS_ERROR_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VEC_PREVIOUS_ERROR_ROW, VALUE_COLUMN, vecPreviousError);
    table->setItem(ERROR_SUM_ROW, NAME_COLUMN, new TableWidgetItem("errorSum"));
    table->setItem(ERROR_SUM_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ERROR_SUM_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ERROR_SUM_ROW, VALUE_COLUMN, errorSum);
    table->setItem(PREVIOUS_ERROR_ROW, NAME_COLUMN, new TableWidgetItem("previousError"));
    table->setItem(PREVIOUS_ERROR_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(PREVIOUS_ERROR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(PREVIOUS_ERROR_ROW, VALUE_COLUMN, previousError);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void DampingModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(kP, SIGNAL(editingFinished()), this, SLOT(setKP()), Qt::UniqueConnection);
    connect(kI, SIGNAL(editingFinished()), this, SLOT(setKI()), Qt::UniqueConnection);
    connect(kD, SIGNAL(editingFinished()), this, SLOT(setKD()), Qt::UniqueConnection);
    connect(enableScalarDamping, SIGNAL(released()), this, SLOT(setEnableScalarDamping()), Qt::UniqueConnection);
    connect(enableVectorDamping, SIGNAL(released()), this, SLOT(setEnableVectorDamping()), Qt::UniqueConnection);
    connect(rawValue, SIGNAL(editingFinished()), this, SLOT(setRawValue()), Qt::UniqueConnection);
    connect(dampedValue, SIGNAL(editingFinished()), this, SLOT(setDampedValue()), Qt::UniqueConnection);
    connect(rawVector, SIGNAL(editingFinished()), this, SLOT(setRawVector()), Qt::UniqueConnection);
    connect(dampedVector, SIGNAL(editingFinished()), this, SLOT(setDampedVector()), Qt::UniqueConnection);
    connect(vecErrorSum, SIGNAL(editingFinished()), this, SLOT(setVecErrorSum()), Qt::UniqueConnection);
    connect(vecPreviousError, SIGNAL(editingFinished()), this, SLOT(setVecPreviousError()), Qt::UniqueConnection);
    connect(errorSum, SIGNAL(editingFinished()), this, SLOT(setErrorSum()), Qt::UniqueConnection);
    connect(previousError, SIGNAL(editingFinished()), this, SLOT(setPreviousError()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void DampingModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(kP, SIGNAL(editingFinished()), this, SLOT(setKP()));
    disconnect(kI, SIGNAL(editingFinished()), this, SLOT(setKI()));
    disconnect(kD, SIGNAL(editingFinished()), this, SLOT(setKD()));
    disconnect(enableScalarDamping, SIGNAL(released()), this, SLOT(setEnableScalarDamping()));
    disconnect(enableVectorDamping, SIGNAL(released()), this, SLOT(setEnableVectorDamping()));
    disconnect(rawValue, SIGNAL(editingFinished()), this, SLOT(setRawValue()));
    disconnect(dampedValue, SIGNAL(editingFinished()), this, SLOT(setDampedValue()));
    disconnect(rawVector, SIGNAL(editingFinished()), this, SLOT(setRawVector()));
    disconnect(dampedVector, SIGNAL(editingFinished()), this, SLOT(setDampedVector()));
    disconnect(vecErrorSum, SIGNAL(editingFinished()), this, SLOT(setVecErrorSum()));
    disconnect(vecPreviousError, SIGNAL(editingFinished()), this, SLOT(setVecPreviousError()));
    disconnect(errorSum, SIGNAL(editingFinished()), this, SLOT(setErrorSum()));
    disconnect(previousError, SIGNAL(editingFinished()), this, SLOT(setPreviousError()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void DampingModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void DampingModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_DAMPING_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbDampingModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            kP->setValue(bsData->kP);
            kI->setValue(bsData->kI);
            kD->setValue(bsData->kD);
            enableScalarDamping->setChecked(bsData->enableScalarDamping);
            enableVectorDamping->setChecked(bsData->enableVectorDamping);
            rawValue->setValue(bsData->rawValue);
            dampedValue->setValue(bsData->dampedValue);
            rawVector->setValue(bsData->rawVector);
            dampedVector->setValue(bsData->dampedVector);
            vecErrorSum->setValue(bsData->vecErrorSum);
            vecPreviousError->setValue(bsData->vecPreviousError);
            errorSum->setValue(bsData->errorSum);
            previousError->setValue(bsData->previousError);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(KP_ROW, BINDING_COLUMN, varBind, "kP");
                loadBinding(KI_ROW, BINDING_COLUMN, varBind, "kI");
                loadBinding(KD_ROW, BINDING_COLUMN, varBind, "kD");
                loadBinding(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN, varBind, "enableScalarDamping");
                loadBinding(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN, varBind, "enableVectorDamping");
                loadBinding(RAW_VALUE_ROW, BINDING_COLUMN, varBind, "rawValue");
                loadBinding(DAMPED_VALUE_ROW, BINDING_COLUMN, varBind, "dampedValue");
                loadBinding(RAW_VECTOR_ROW, BINDING_COLUMN, varBind, "rawVector");
                loadBinding(DAMPED_VECTOR_ROW, BINDING_COLUMN, varBind, "dampedVector");
                loadBinding(VEC_ERROR_SUM_ROW, BINDING_COLUMN, varBind, "vecErrorSum");
                loadBinding(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN, varBind, "vecPreviousError");
                loadBinding(ERROR_SUM_ROW, BINDING_COLUMN, varBind, "errorSum");
                loadBinding(PREVIOUS_ERROR_ROW, BINDING_COLUMN, varBind, "previousError");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(KP_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(KI_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(KD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RAW_VALUE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DAMPED_VALUE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RAW_VECTOR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DAMPED_VECTOR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VEC_ERROR_SUM_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ERROR_SUM_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(PREVIOUS_ERROR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            FATAL_RUNTIME_ERROR("DampingModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void DampingModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setName(): The data is nullptr!!");
    }
}

void DampingModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setEnable(): The data is nullptr!!");
    }
}

void DampingModifierUI::setKP(){
    if (bsData){
        if (bsData->kP != kP->value()){
            bsData->kP = kP->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setkP(): The data is nullptr!!");
    }
}

void DampingModifierUI::setKI(){
    if (bsData){
        if (bsData->kI != kI->value()){
            bsData->kI = kI->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setkI(): The data is nullptr!!");
    }
}

void DampingModifierUI::setKD(){
    if (bsData){
        if (bsData->kD != kD->value()){
            bsData->kD = kD->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setkD(): The data is nullptr!!");
    }
}

void DampingModifierUI::setEnableScalarDamping(){
    if (bsData){
        bsData->enableScalarDamping = enableScalarDamping->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setenableScalarDamping(): The data is nullptr!!");
    }
}

void DampingModifierUI::setEnableVectorDamping(){
    if (bsData){
        bsData->enableVectorDamping = enableVectorDamping->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setenableVectorDamping(): The data is nullptr!!");
    }
}

void DampingModifierUI::setRawValue(){
    if (bsData){
        if (bsData->rawValue != rawValue->value()){
            bsData->rawValue = rawValue->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setrawValue(): The data is nullptr!!");
    }
}

void DampingModifierUI::setDampedValue(){
    if (bsData){
        if (bsData->dampedValue != dampedValue->value()){
            bsData->dampedValue = dampedValue->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setdampedValue(): The data is nullptr!!");
    }
}

void DampingModifierUI::setRawVector(){
    if (bsData){
        if (bsData->rawVector != rawVector->value()){
            bsData->rawVector = rawVector->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setrawVector(): The data is nullptr!!");
    }
}

void DampingModifierUI::setDampedVector(){
    if (bsData){
        if (bsData->dampedVector != dampedVector->value()){
            bsData->dampedVector = dampedVector->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setdampedVector(): The data is nullptr!!");
    }
}

void DampingModifierUI::setVecErrorSum(){
    if (bsData){
        if (bsData->vecErrorSum != vecErrorSum->value()){
            bsData->vecErrorSum = vecErrorSum->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setvecErrorSum(): The data is nullptr!!");
    }
}

void DampingModifierUI::setVecPreviousError(){
    if (bsData){
        if (bsData->vecPreviousError != vecPreviousError->value()){
            bsData->vecPreviousError = vecPreviousError->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setvecPreviousError(): The data is nullptr!!");
    }
}

void DampingModifierUI::setErrorSum(){
    if (bsData){
        if (bsData->errorSum != errorSum->value()){
            bsData->errorSum = errorSum->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::seterrorSum(): The data is nullptr!!");
    }
}

void DampingModifierUI::setPreviousError(){
    if (bsData){
        if (bsData->previousError != previousError->value()){
            bsData->previousError = previousError->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setpreviousError(): The data is nullptr!!");
    }
}

void DampingModifierUI::viewSelected(int row, int column){
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
            case KP_ROW:
                if (table->item(KP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "kP");
                break;
            case KI_ROW:
                if (table->item(KI_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "kI");
                break;
            case KD_ROW:
                if (table->item(KD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "kD");
                break;
            case ENABLE_SCALAR_DAMPING_ROW:
                if (table->item(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enableScalarDamping");
                break;
            case ENABLE_VECTOR_DAMPING_ROW:
                if (table->item(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enableVectorDamping");
                break;
            case RAW_VALUE_ROW:
                if (table->item(RAW_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rawValue");
                break;
            case DAMPED_VALUE_ROW:
                if (table->item(DAMPED_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "dampedValue");
                break;
            case RAW_VECTOR_ROW:
                if (table->item(RAW_VECTOR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rawVector");
                break;
            case DAMPED_VECTOR_ROW:
                if (table->item(DAMPED_VECTOR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "dampedVector");
                break;
            case VEC_ERROR_SUM_ROW:
                if (table->item(VEC_ERROR_SUM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "vecErrorSum");
                break;
            case VEC_PREVIOUS_ERROR_ROW:
                if (table->item(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "vecPreviousError");
                break;
            case ERROR_SUM_ROW:
                if (table->item(ERROR_SUM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "errorSum");
                break;
            case PREVIOUS_ERROR_ROW:
                if (table->item(PREVIOUS_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "previousError");
                break;
            default:
                return;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void DampingModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        FATAL_RUNTIME_ERROR("DampingModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void DampingModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("kP");
            if (bindIndex == index){
                table->item(KP_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("kI");
            if (bindIndex == index){
                table->item(KI_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("kD");
            if (bindIndex == index){
                table->item(KD_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("enableScalarDamping");
            if (bindIndex == index){
                table->item(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("enableVectorDamping");
            if (bindIndex == index){
                table->item(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rawValue");
            if (bindIndex == index){
                table->item(RAW_VALUE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("dampedValue");
            if (bindIndex == index){
                table->item(DAMPED_VALUE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rawVector");
            if (bindIndex == index){
                table->item(RAW_VECTOR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("dampedVector");
            if (bindIndex == index){
                table->item(DAMPED_VECTOR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("vecErrorSum");
            if (bindIndex == index){
                table->item(VEC_ERROR_SUM_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("vecPreviousError");
            if (bindIndex == index){
                table->item(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("errorSum");
            if (bindIndex == index){
                table->item(ERROR_SUM_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("previousError");
            if (bindIndex == index){
                table->item(PREVIOUS_ERROR_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool DampingModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    FATAL_RUNTIME_ERROR("DampingModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("DampingModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void DampingModifierUI::setBindingVariable(int index, const QString &name){
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
        case KP_ROW:
            if (table->item(KP_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "kP", VARIABLE_TYPE_REAL, isProperty);
            break;
        case KI_ROW:
            if (table->item(KI_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "kI", VARIABLE_TYPE_REAL, isProperty);
            break;
        case KD_ROW:
            if (table->item(KD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "kD", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ENABLE_SCALAR_DAMPING_ROW:
            if (table->item(ENABLE_SCALAR_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enableScalarDamping", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case ENABLE_VECTOR_DAMPING_ROW:
            if (table->item(ENABLE_VECTOR_DAMPING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enableVectorDamping", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case RAW_VALUE_ROW:
            if (table->item(RAW_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rawValue", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DAMPED_VALUE_ROW:
            if (table->item(DAMPED_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "dampedValue", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case RAW_VECTOR_ROW:
            if (table->item(RAW_VECTOR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rawVector", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case DAMPED_VECTOR_ROW:
            if (table->item(DAMPED_VECTOR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "dampedVector", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case VEC_ERROR_SUM_ROW:
            if (table->item(VEC_ERROR_SUM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "vecErrorSum", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case VEC_PREVIOUS_ERROR_ROW:
            if (table->item(VEC_PREVIOUS_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "vecPreviousError", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ERROR_SUM_ROW:
            if (table->item(ERROR_SUM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "errorSum", VARIABLE_TYPE_REAL, isProperty);
            break;
        case PREVIOUS_ERROR_ROW:
            if (table->item(PREVIOUS_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "previousError", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void DampingModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("DampingModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("DampingModifierUI::loadBinding(): The data is nullptr!!");
    }
}
