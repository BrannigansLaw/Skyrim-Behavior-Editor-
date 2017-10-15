#include "rotatecharactermodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbrotatecharactermodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 5

#define NAME_ROW 0
#define ENABLE_ROW 1
#define DEGREES_PER_SECOND_ROW 2
#define SPEED_MULTIPLIER_ROW 3
#define AXIS_OF_ROTATION_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList RotateCharacterModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

RotateCharacterModifierUI::RotateCharacterModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      degreesPerSecond(new DoubleSpinBox),
      speedMultiplier(new DoubleSpinBox),
      axisOfRotation(new QuadVariableWidget)
{
    setTitle("hkbRotateCharacterModifier");
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
    table->setItem(DEGREES_PER_SECOND_ROW, NAME_COLUMN, new TableWidgetItem("degreesPerSecond"));
    table->setItem(DEGREES_PER_SECOND_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DEGREES_PER_SECOND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DEGREES_PER_SECOND_ROW, VALUE_COLUMN, degreesPerSecond);
    table->setItem(SPEED_MULTIPLIER_ROW, NAME_COLUMN, new TableWidgetItem("speedMultiplier"));
    table->setItem(SPEED_MULTIPLIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SPEED_MULTIPLIER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SPEED_MULTIPLIER_ROW, VALUE_COLUMN, speedMultiplier);
    table->setItem(AXIS_OF_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("axisOfRotation"));
    table->setItem(AXIS_OF_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(AXIS_OF_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(AXIS_OF_ROTATION_ROW, VALUE_COLUMN, axisOfRotation);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void RotateCharacterModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(degreesPerSecond, SIGNAL(editingFinished()), this, SLOT(setDegreesPerSecond()), Qt::UniqueConnection);
    connect(speedMultiplier, SIGNAL(editingFinished()), this, SLOT(setSpeedMultiplier()), Qt::UniqueConnection);
    connect(axisOfRotation, SIGNAL(editingFinished()), this, SLOT(setAxisOfRotation()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void RotateCharacterModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(degreesPerSecond, SIGNAL(editingFinished()), this, SLOT(setDegreesPerSecond()));
    disconnect(speedMultiplier, SIGNAL(editingFinished()), this, SLOT(setSpeedMultiplier()));
    disconnect(axisOfRotation, SIGNAL(editingFinished()), this, SLOT(setAxisOfRotation()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void RotateCharacterModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("RotateCharacterModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void RotateCharacterModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_ROTATE_CHARACTER_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbRotateCharacterModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            degreesPerSecond->setValue(bsData->degreesPerSecond);
            speedMultiplier->setValue(bsData->speedMultiplier);
            axisOfRotation->setValue(bsData->axisOfRotation);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(DEGREES_PER_SECOND_ROW, BINDING_COLUMN, varBind, "degreesPerSecond");
                loadBinding(SPEED_MULTIPLIER_ROW, BINDING_COLUMN, varBind, "speedMultiplier");
                loadBinding(AXIS_OF_ROTATION_ROW, BINDING_COLUMN, varBind, "axisOfRotation");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SPEED_MULTIPLIER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal("RotateCharacterModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        (qFatal("RotateCharacterModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void RotateCharacterModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        (qFatal("RotateCharacterModifierUI::setName(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("RotateCharacterModifierUI::setEnable(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::setDegreesPerSecond(){
    if (bsData){
        if (bsData->degreesPerSecond != degreesPerSecond->value()){
            bsData->degreesPerSecond = degreesPerSecond->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("RotateCharacterModifierUI::setDegreesPerSecond(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::setSpeedMultiplier(){
    if (bsData){
        if (bsData->speedMultiplier != speedMultiplier->value()){
            bsData->speedMultiplier = speedMultiplier->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("RotateCharacterModifierUI::setSpeedMultiplier(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::setAxisOfRotation(){
    if (bsData){
        if (bsData->axisOfRotation != axisOfRotation->value()){
            bsData->axisOfRotation = axisOfRotation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("RotateCharacterModifierUI::setAxisOfRotation(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::viewSelected(int row, int column){
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
                selectTableToView(isProperty, "degreesPerSecond");
                break;
            case SPEED_MULTIPLIER_ROW:
                if (table->item(SPEED_MULTIPLIER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "speedMultiplier");
                break;
            case AXIS_OF_ROTATION_ROW:
                if (table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "axisOfRotation");
                break;
            default:
                return;
            }
        }
    }else{
        (qFatal("RotateCharacterModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void RotateCharacterModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        (qFatal("RotateCharacterModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("degreesPerSecond");
            if (bindIndex == index){
                table->item(DEGREES_PER_SECOND_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("speedMultiplier");
            if (bindIndex == index){
                table->item(SPEED_MULTIPLIER_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("axisOfRotation");
            if (bindIndex == index){
                table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("RotateCharacterModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

bool RotateCharacterModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    (qFatal("RotateCharacterModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("RotateCharacterModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("RotateCharacterModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void RotateCharacterModifierUI::setBindingVariable(int index, const QString &name){
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
            setBinding(index, row, name, "degreesPerSecond", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SPEED_MULTIPLIER_ROW:
            if (table->item(SPEED_MULTIPLIER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "speedMultiplier", VARIABLE_TYPE_REAL, isProperty);
            break;
        case AXIS_OF_ROTATION_ROW:
            if (table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "axisOfRotation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("RotateCharacterModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void RotateCharacterModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("RotateCharacterModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("RotateCharacterModifierUI::loadBinding(): The data is NULL!!"));
    }
}
