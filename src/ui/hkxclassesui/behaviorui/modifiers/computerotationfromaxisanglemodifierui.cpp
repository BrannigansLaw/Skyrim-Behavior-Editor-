#include "computerotationfromaxisanglemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbComputeRotationFromAxisAngleModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 5

#define NAME_ROW 0
#define ENABLE_ROW 1
#define DEGREES_PER_SECOND_ROW 2
#define AXIS_ROW 3
#define ANGLE_DEGREES_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ComputeRotationFromAxisAngleModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ComputeRotationFromAxisAngleModifierUI::ComputeRotationFromAxisAngleModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      rotationOut(new QuadVariableWidget),
      axis(new QuadVariableWidget),
      angleDegrees(new DoubleSpinBox)
{
    setTitle("hkbComputeRotationFromAxisAngleModifier");
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
    table->setItem(DEGREES_PER_SECOND_ROW, NAME_COLUMN, new TableWidgetItem("rotationOut"));
    table->setItem(DEGREES_PER_SECOND_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(DEGREES_PER_SECOND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DEGREES_PER_SECOND_ROW, VALUE_COLUMN, rotationOut);
    table->setItem(AXIS_ROW, NAME_COLUMN, new TableWidgetItem("axis"));
    table->setItem(AXIS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(AXIS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(AXIS_ROW, VALUE_COLUMN, axis);
    table->setItem(ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("angleDegrees"));
    table->setItem(ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANGLE_DEGREES_ROW, VALUE_COLUMN, angleDegrees);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void ComputeRotationFromAxisAngleModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setAxis()), Qt::UniqueConnection);
    connect(axis, SIGNAL(editingFinished()), this, SLOT(setAxis()), Qt::UniqueConnection);
    connect(angleDegrees, SIGNAL(editingFinished()), this, SLOT(setAngleDegrees()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void ComputeRotationFromAxisAngleModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setAxis()));
    disconnect(axis, SIGNAL(editingFinished()), this, SLOT(setAxis()));
    disconnect(angleDegrees, SIGNAL(editingFinished()), this, SLOT(setAngleDegrees()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void ComputeRotationFromAxisAngleModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_COMPUTE_ROTATION_FROM_AXIS_ANGLE_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbComputeRotationFromAxisAngleModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            rotationOut->setValue(bsData->rotationOut);
            axis->setValue(bsData->axis);
            angleDegrees->setValue(bsData->angleDegrees);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(DEGREES_PER_SECOND_ROW, BINDING_COLUMN, varBind, "rotationOut");
                loadBinding(AXIS_ROW, BINDING_COLUMN, varBind, "axis");
                loadBinding(ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, "angleDegrees");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(AXIS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal("ComputeRotationFromAxisAngleModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void ComputeRotationFromAxisAngleModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setName(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setEnable(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::setRotationOut(){
    if (bsData){
        if (bsData->rotationOut != rotationOut->value()){
            bsData->rotationOut = rotationOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setRotationOut(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::setAxis(){
    if (bsData){
        if (bsData->axis != axis->value()){
            bsData->axis = axis->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setaxis(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::setAngleDegrees(){
    if (bsData){
        if (bsData->angleDegrees != angleDegrees->value()){
            bsData->angleDegrees = angleDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setangleDegrees(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::viewSelected(int row, int column){
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
            case DEGREES_PER_SECOND_ROW:
                if (table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotationOut");
                break;
            case AXIS_ROW:
                if (table->item(AXIS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "axis");
                break;
            case ANGLE_DEGREES_ROW:
                if (table->item(ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "angleDegrees");
                break;
            default:
                return;
            }
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        (qFatal("ComputeRotationFromAxisAngleModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::variableRenamed(const QString & name, int index){
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
                table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("axis");
            if (bindIndex == index){
                table->item(AXIS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("angleDegrees");
            if (bindIndex == index){
                table->item(ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

bool ComputeRotationFromAxisAngleModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    (qFatal("ComputeRotationFromAxisAngleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("ComputeRotationFromAxisAngleModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void ComputeRotationFromAxisAngleModifierUI::setBindingVariable(int index, const QString &name){
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
        case DEGREES_PER_SECOND_ROW:
            if (table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotationOut", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case AXIS_ROW:
            if (table->item(AXIS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "axis", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ANGLE_DEGREES_ROW:
            if (table->item(ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "angleDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void ComputeRotationFromAxisAngleModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("ComputeRotationFromAxisAngleModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("ComputeRotationFromAxisAngleModifierUI::loadBinding(): The data is NULL!!"));
    }
}
