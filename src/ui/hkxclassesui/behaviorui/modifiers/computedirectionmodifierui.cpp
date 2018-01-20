#include "computedirectionmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbcomputedirectionmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 13

#define NAME_ROW 0
#define ENABLE_ROW 1
#define POINT_IN_ROW 2
#define POINT_OUT_ROW 3
#define GROUND_ANGLE_OUT_ROW 4
#define UP_ANGLE_OUT_ROW 5
#define VERTICAL_OFFSET_ROW 6
#define REVERSE_GROUND_ANGLE_ROW 7
#define REVERSE_UP_ANGLE_ROW 8
#define PROJECT_POINT_ROW 9
#define NORMALIZE_POINT_ROW 10
#define COMPUTE_ONLY_ONCE_ROW 11
#define COMPUTED_OUTPUT_ROW 12

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ComputeDirectionModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ComputeDirectionModifierUI::ComputeDirectionModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      pointIn(new QuadVariableWidget),
      pointOut(new QuadVariableWidget),
      groundAngleOut(new DoubleSpinBox),
      upAngleOut(new DoubleSpinBox),
      verticalOffset(new DoubleSpinBox),
      reverseGroundAngle(new CheckBox),
      reverseUpAngle(new CheckBox),
      projectPoint(new CheckBox),
      normalizePoint(new CheckBox),
      computeOnlyOnce(new CheckBox),
      computedOutput(new CheckBox)
{
    setTitle("hkbComputeDirectionModifier");
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
    table->setItem(POINT_IN_ROW, NAME_COLUMN, new TableWidgetItem("pointIn"));
    table->setItem(POINT_IN_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(POINT_IN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POINT_IN_ROW, VALUE_COLUMN, pointIn);
    table->setItem(POINT_OUT_ROW, NAME_COLUMN, new TableWidgetItem("pointOut"));
    table->setItem(POINT_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(POINT_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(POINT_OUT_ROW, VALUE_COLUMN, pointOut);
    table->setItem(GROUND_ANGLE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("groundAngleOut"));
    table->setItem(GROUND_ANGLE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GROUND_ANGLE_OUT_ROW, VALUE_COLUMN, groundAngleOut);
    table->setItem(UP_ANGLE_OUT_ROW, NAME_COLUMN, new TableWidgetItem("upAngleOut"));
    table->setItem(UP_ANGLE_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(UP_ANGLE_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(UP_ANGLE_OUT_ROW, VALUE_COLUMN, upAngleOut);
    table->setItem(VERTICAL_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("verticalOffset"));
    table->setItem(VERTICAL_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(VERTICAL_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VERTICAL_OFFSET_ROW, VALUE_COLUMN, verticalOffset);
    table->setItem(REVERSE_GROUND_ANGLE_ROW, NAME_COLUMN, new TableWidgetItem("reverseGroundAngle"));
    table->setItem(REVERSE_GROUND_ANGLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(REVERSE_GROUND_ANGLE_ROW, VALUE_COLUMN, reverseGroundAngle);
    table->setItem(REVERSE_UP_ANGLE_ROW, NAME_COLUMN, new TableWidgetItem("reverseUpAngle"));
    table->setItem(REVERSE_UP_ANGLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(REVERSE_UP_ANGLE_ROW, VALUE_COLUMN, reverseUpAngle);
    table->setItem(PROJECT_POINT_ROW, NAME_COLUMN, new TableWidgetItem("projectPoint"));
    table->setItem(PROJECT_POINT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(PROJECT_POINT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(PROJECT_POINT_ROW, VALUE_COLUMN, projectPoint);
    table->setItem(NORMALIZE_POINT_ROW, NAME_COLUMN, new TableWidgetItem("normalizePoint"));
    table->setItem(NORMALIZE_POINT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(NORMALIZE_POINT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NORMALIZE_POINT_ROW, VALUE_COLUMN, normalizePoint);
    table->setItem(COMPUTE_ONLY_ONCE_ROW, NAME_COLUMN, new TableWidgetItem("computeOnlyOnce"));
    table->setItem(COMPUTE_ONLY_ONCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COMPUTE_ONLY_ONCE_ROW, VALUE_COLUMN, computeOnlyOnce);
    table->setItem(COMPUTED_OUTPUT_ROW, NAME_COLUMN, new TableWidgetItem("computedOutput"));
    table->setItem(COMPUTED_OUTPUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(COMPUTED_OUTPUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COMPUTED_OUTPUT_ROW, VALUE_COLUMN, computedOutput);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void ComputeDirectionModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(pointIn, SIGNAL(editingFinished()), this, SLOT(setPointIn()), Qt::UniqueConnection);
    connect(pointOut, SIGNAL(editingFinished()), this, SLOT(setPointOut()), Qt::UniqueConnection);
    connect(groundAngleOut, SIGNAL(editingFinished()), this, SLOT(setGroundAngleOut()), Qt::UniqueConnection);
    connect(upAngleOut, SIGNAL(editingFinished()), this, SLOT(setUpAngleOut()), Qt::UniqueConnection);
    connect(verticalOffset, SIGNAL(editingFinished()), this, SLOT(setVerticalOffset()), Qt::UniqueConnection);
    connect(reverseGroundAngle, SIGNAL(released()), this, SLOT(setReverseGroundAngle()), Qt::UniqueConnection);
    connect(reverseUpAngle, SIGNAL(released()), this, SLOT(setReverseUpAngle()), Qt::UniqueConnection);
    connect(projectPoint, SIGNAL(released()), this, SLOT(setProjectPoint()), Qt::UniqueConnection);
    connect(normalizePoint, SIGNAL(released()), this, SLOT(setNormalizePoint()), Qt::UniqueConnection);
    connect(computeOnlyOnce, SIGNAL(released()), this, SLOT(setComputeOnlyOnce()), Qt::UniqueConnection);
    connect(computedOutput, SIGNAL(released()), this, SLOT(setComputedOutput()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void ComputeDirectionModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(pointIn, SIGNAL(editingFinished()), this, SLOT(setPointIn()));
    disconnect(pointOut, SIGNAL(editingFinished()), this, SLOT(setPointOut()));
    disconnect(groundAngleOut, SIGNAL(editingFinished()), this, SLOT(setGroundAngleOut()));
    disconnect(upAngleOut, SIGNAL(editingFinished()), this, SLOT(setUpAngleOut()));
    disconnect(verticalOffset, SIGNAL(editingFinished()), this, SLOT(setVerticalOffset()));
    disconnect(reverseGroundAngle, SIGNAL(released()), this, SLOT(setReverseGroundAngle()));
    disconnect(reverseUpAngle, SIGNAL(released()), this, SLOT(setReverseUpAngle()));
    disconnect(projectPoint, SIGNAL(released()), this, SLOT(setProjectPoint()));
    disconnect(normalizePoint, SIGNAL(released()), this, SLOT(setNormalizePoint()));
    disconnect(computeOnlyOnce, SIGNAL(released()), this, SLOT(setComputeOnlyOnce()));
    disconnect(computedOutput, SIGNAL(released()), this, SLOT(setComputedOutput()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void ComputeDirectionModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void ComputeDirectionModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_COMPUTE_DIRECTION_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbComputeDirectionModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            pointIn->setValue(bsData->pointIn);
            pointOut->setValue(bsData->pointOut);
            groundAngleOut->setValue(bsData->groundAngleOut);
            upAngleOut->setValue(bsData->upAngleOut);
            verticalOffset->setValue(bsData->verticalOffset);
            reverseGroundAngle->setChecked(bsData->reverseGroundAngle);
            reverseUpAngle->setChecked(bsData->reverseUpAngle);
            projectPoint->setChecked(bsData->projectPoint);
            normalizePoint->setChecked(bsData->normalizePoint);
            computeOnlyOnce->setChecked(bsData->computeOnlyOnce);
            computedOutput->setChecked(bsData->computedOutput);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(POINT_IN_ROW, BINDING_COLUMN, varBind, "pointIn");
                loadBinding(POINT_OUT_ROW, BINDING_COLUMN, varBind, "pointOut");
                loadBinding(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN, varBind, "groundAngleOut");
                loadBinding(UP_ANGLE_OUT_ROW, BINDING_COLUMN, varBind, "upAngleOut");
                loadBinding(VERTICAL_OFFSET_ROW, BINDING_COLUMN, varBind, "verticalOffset");
                loadBinding(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN, varBind, "reverseGroundAngle");
                loadBinding(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN, varBind, "reverseUpAngle");
                loadBinding(PROJECT_POINT_ROW, BINDING_COLUMN, varBind, "projectPoint");
                loadBinding(NORMALIZE_POINT_ROW, BINDING_COLUMN, varBind, "normalizePoint");
                loadBinding(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN, varBind, "computeOnlyOnce");
                loadBinding(COMPUTED_OUTPUT_ROW, BINDING_COLUMN, varBind, "computedOutput");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POINT_IN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(POINT_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(UP_ANGLE_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VERTICAL_OFFSET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(PROJECT_POINT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NORMALIZE_POINT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COMPUTED_OUTPUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void ComputeDirectionModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setName(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setEnable(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setPointIn(){
    if (bsData){
        if (bsData->pointIn != pointIn->value()){
            bsData->pointIn = pointIn->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setPointIn(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setPointOut(){
    if (bsData){
        if (bsData->pointOut != pointOut->value()){
            bsData->pointOut = pointOut->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setPointOut(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setGroundAngleOut(){
    if (bsData){
        if (bsData->groundAngleOut != groundAngleOut->value()){
            bsData->groundAngleOut = groundAngleOut->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setGroundAngleOut(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setUpAngleOut(){
    if (bsData){
        if (bsData->upAngleOut != upAngleOut->value()){
            bsData->upAngleOut = upAngleOut->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setUpAngleOut(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setVerticalOffset(){
    if (bsData){
        if (bsData->verticalOffset != verticalOffset->value()){
            bsData->verticalOffset = verticalOffset->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setVerticalOffset(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setReverseGroundAngle(){
    if (bsData){
        bsData->reverseGroundAngle = reverseGroundAngle->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setReverseGroundAngle(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setReverseUpAngle(){
    if (bsData){
        bsData->reverseUpAngle = reverseUpAngle->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setReverseUpAngle(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setProjectPoint(){
    if (bsData){
        bsData->projectPoint = projectPoint->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setProjectPoint(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setNormalizePoint(){
    if (bsData){
        bsData->normalizePoint = normalizePoint->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setNormalizePoint(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setComputeOnlyOnce(){
    if (bsData){
        bsData->computeOnlyOnce = computeOnlyOnce->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setComputeOnlyOnce(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::setComputedOutput(){
    if (bsData){
        bsData->computedOutput = computedOutput->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setComputedOutput(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::viewSelected(int row, int column){
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
            case POINT_IN_ROW:
                if (table->item(POINT_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "pointIn");
                break;
            case POINT_OUT_ROW:
                if (table->item(POINT_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "pointOut");
                break;
            case GROUND_ANGLE_OUT_ROW:
                if (table->item(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "groundAngleOut");
                break;
            case UP_ANGLE_OUT_ROW:
                if (table->item(UP_ANGLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "upAngleOut");
                break;
            case VERTICAL_OFFSET_ROW:
                if (table->item(VERTICAL_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "verticalOffset");
                break;
            case REVERSE_GROUND_ANGLE_ROW:
                if (table->item(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "reverseGroundAngle");
                break;
            case REVERSE_UP_ANGLE_ROW:
                if (table->item(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "reverseUpAngle");
                break;
            case PROJECT_POINT_ROW:
                if (table->item(PROJECT_POINT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "projectPoint");
                break;
            case NORMALIZE_POINT_ROW:
                if (table->item(NORMALIZE_POINT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "normalizePoint");
                break;
            case COMPUTE_ONLY_ONCE_ROW:
                if (table->item(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "computeOnlyOnce");
                break;
            case COMPUTED_OUTPUT_ROW:
                if (table->item(COMPUTED_OUTPUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "computedOutput");
                break;
            default:
                return;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void ComputeDirectionModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("pointIn");
            if (bindIndex == index){
                table->item(POINT_IN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("pointOut");
            if (bindIndex == index){
                table->item(POINT_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("groundAngleOut");
            if (bindIndex == index){
                table->item(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("upAngleOut");
            if (bindIndex == index){
                table->item(UP_ANGLE_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("verticalOffset");
            if (bindIndex == index){
                table->item(VERTICAL_OFFSET_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("reverseGroundAngle");
            if (bindIndex == index){
                table->item(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("reverseUpAngle");
            if (bindIndex == index){
                table->item(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("projectPoint");
            if (bindIndex == index){
                table->item(PROJECT_POINT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("normalizePoint");
            if (bindIndex == index){
                table->item(NORMALIZE_POINT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("computeOnlyOnce");
            if (bindIndex == index){
                table->item(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("computedOutput");
            if (bindIndex == index){
                table->item(COMPUTED_OUTPUT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool ComputeDirectionModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void ComputeDirectionModifierUI::setBindingVariable(int index, const QString &name){
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
        case POINT_IN_ROW:
            if (table->item(POINT_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "pointIn", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case POINT_OUT_ROW:
            if (table->item(POINT_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "pointOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case GROUND_ANGLE_OUT_ROW:
            if (table->item(GROUND_ANGLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "groundAngleOut", VARIABLE_TYPE_REAL, isProperty);
            break;
        case UP_ANGLE_OUT_ROW:
            if (table->item(UP_ANGLE_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "upAngleOut", VARIABLE_TYPE_REAL, isProperty);
            break;
        case VERTICAL_OFFSET_ROW:
            if (table->item(VERTICAL_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "verticalOffset", VARIABLE_TYPE_REAL, isProperty);
            break;
        case REVERSE_GROUND_ANGLE_ROW:
            if (table->item(REVERSE_GROUND_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "reverseGroundAngle", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case REVERSE_UP_ANGLE_ROW:
            if (table->item(REVERSE_UP_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "reverseUpAngle", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case PROJECT_POINT_ROW:
            if (table->item(PROJECT_POINT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "projectPoint", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case NORMALIZE_POINT_ROW:
            if (table->item(NORMALIZE_POINT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "normalizePoint", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case COMPUTE_ONLY_ONCE_ROW:
            if (table->item(COMPUTE_ONLY_ONCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "computeOnlyOnce", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case COMPUTED_OUTPUT_ROW:
            if (table->item(COMPUTED_OUTPUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "computedOutput", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void ComputeDirectionModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("ComputeDirectionModifierUI::loadBinding(): The data is nullptr!!");
    }
}
