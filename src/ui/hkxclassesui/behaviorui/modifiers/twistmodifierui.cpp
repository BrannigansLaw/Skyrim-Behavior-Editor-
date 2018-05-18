#include "twistmodifierui.h"

#include "src/hkxclasses/behavior/modifiers/hkbTwistModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 9

#define NAME_ROW 0
#define ENABLE_ROW 1
#define AXIS_OF_ROTATION_ROW 2
#define TWIST_ANGLE_ROW 3
#define START_BONE_INDEX_ROW 4
#define END_BONE_INDEX_ROW 5
#define SET_ANGLE_METHOD_ROW 6
#define ROTATION_AXIS_COORDINATE_ROW 7
#define IS_ADDITIVE_ROW 8

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList TwistModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

TwistModifierUI::TwistModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      axisOfRotation(new QuadVariableWidget),
      twistAngle(new DoubleSpinBox),
      startBoneIndex(new ComboBox),
      endBoneIndex(new ComboBox),
      setAngleMethod(new ComboBox),
      rotationAxisCoordinates(new ComboBox),
      isAdditive(new CheckBox)
{
    setTitle("hkbTwistModifier");
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
    table->setItem(AXIS_OF_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("axisOfRotation"));
    table->setItem(AXIS_OF_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(AXIS_OF_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(AXIS_OF_ROTATION_ROW, VALUE_COLUMN, axisOfRotation);
    table->setItem(TWIST_ANGLE_ROW, NAME_COLUMN, new TableWidgetItem("twistAngle"));
    table->setItem(TWIST_ANGLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TWIST_ANGLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TWIST_ANGLE_ROW, VALUE_COLUMN, twistAngle);
    table->setItem(START_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("startBoneIndex"));
    table->setItem(START_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(START_BONE_INDEX_ROW, VALUE_COLUMN, startBoneIndex);
    table->setItem(END_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("endBoneIndex"));
    table->setItem(END_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(END_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(END_BONE_INDEX_ROW, VALUE_COLUMN, endBoneIndex);
    table->setItem(SET_ANGLE_METHOD_ROW, NAME_COLUMN, new TableWidgetItem("setAngleMethod"));
    table->setItem(SET_ANGLE_METHOD_ROW, TYPE_COLUMN, new TableWidgetItem("SetAngleMethod", Qt::AlignCenter));
    table->setItem(SET_ANGLE_METHOD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SET_ANGLE_METHOD_ROW, VALUE_COLUMN, setAngleMethod);
    table->setItem(ROTATION_AXIS_COORDINATE_ROW, NAME_COLUMN, new TableWidgetItem("rotationAxisCoordinates"));
    table->setItem(ROTATION_AXIS_COORDINATE_ROW, TYPE_COLUMN, new TableWidgetItem("RotationAxisCoordinates", Qt::AlignCenter));
    table->setItem(ROTATION_AXIS_COORDINATE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(ROTATION_AXIS_COORDINATE_ROW, VALUE_COLUMN, rotationAxisCoordinates);
    table->setItem(IS_ADDITIVE_ROW, NAME_COLUMN, new TableWidgetItem("isAdditive"));
    table->setItem(IS_ADDITIVE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ADDITIVE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ADDITIVE_ROW, VALUE_COLUMN, isAdditive);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void TwistModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(axisOfRotation, SIGNAL(editingFinished()), this, SLOT(setAxisOfRotation()), Qt::UniqueConnection);
    connect(twistAngle, SIGNAL(editingFinished()), this, SLOT(setTwistAngle()), Qt::UniqueConnection);
    connect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)), Qt::UniqueConnection);
    connect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)), Qt::UniqueConnection);
    connect(setAngleMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(setSetAngleMethod(int)), Qt::UniqueConnection);
    connect(rotationAxisCoordinates, SIGNAL(currentIndexChanged(int)), this, SLOT(setRotationAxisCoordinates(int)), Qt::UniqueConnection);
    connect(isAdditive, SIGNAL(released()), this, SLOT(setIsAdditive()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void TwistModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(axisOfRotation, SIGNAL(editingFinished()), this, SLOT(setAxisOfRotation()));
    disconnect(twistAngle, SIGNAL(editingFinished()), this, SLOT(setTwistAngle()));
    disconnect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)));
    disconnect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)));
    disconnect(setAngleMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(setSetAngleMethod(int)));
    disconnect(rotationAxisCoordinates, SIGNAL(currentIndexChanged(int)), this, SLOT(setRotationAxisCoordinates(int)));
    disconnect(isAdditive, SIGNAL(released()), this, SLOT(setIsAdditive()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void TwistModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void TwistModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_TWIST_MODIFIER){
            QStringList boneNames("None");
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbTwistModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            axisOfRotation->setValue(bsData->axisOfRotation);
            twistAngle->setValue(bsData->twistAngle);
            if (startBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                startBoneIndex->insertItems(0, boneNames);
            }
            startBoneIndex->setCurrentIndex(bsData->startBoneIndex + 1);
            if (endBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                endBoneIndex->insertItems(0, boneNames);
            }
            endBoneIndex->setCurrentIndex(bsData->endBoneIndex + 1);
            if (setAngleMethod->count() == 0){
                setAngleMethod->insertItems(0, bsData->SetAngleMethod);
            }
            setAngleMethod->setCurrentIndex(bsData->SetAngleMethod.indexOf(bsData->setAngleMethod));

            if (rotationAxisCoordinates->count() == 0){
                rotationAxisCoordinates->insertItems(0, bsData->RotationAxisCoordinates);
            }
            rotationAxisCoordinates->setCurrentIndex(bsData->RotationAxisCoordinates.indexOf(bsData->rotationAxisCoordinates));
            isAdditive->setChecked(bsData->isAdditive);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(AXIS_OF_ROTATION_ROW, BINDING_COLUMN, varBind, "axisOfRotation");
                loadBinding(TWIST_ANGLE_ROW, BINDING_COLUMN, varBind, "twistAngle");
                loadBinding(START_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "startBoneIndex");
                loadBinding(END_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "endBoneIndex");
                loadBinding(IS_ADDITIVE_ROW, BINDING_COLUMN, varBind, "isAdditive");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TWIST_ANGLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ADDITIVE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("TwistModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void TwistModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setName(): The data is nullptr!!");
    }
}

void TwistModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setEnable(): The data is nullptr!!");
    }
}

void TwistModifierUI::setAxisOfRotation(){
    if (bsData){
        if (bsData->axisOfRotation != axisOfRotation->value()){
            bsData->axisOfRotation = axisOfRotation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setaxisOfRotation(): The data is nullptr!!");
    }
}

void TwistModifierUI::setStartBoneIndex(int index){
    if (bsData){
        bsData->startBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setStartBoneIndex(): The data is nullptr!!");
    }
}

void TwistModifierUI::setEndBoneIndex(int index){
    if (bsData){
        bsData->endBoneIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setEndBoneIndex(): The data is nullptr!!");
    }
}

void TwistModifierUI::setTwistAngle(){
    if (bsData){
        if (bsData->twistAngle != twistAngle->value()){
            bsData->twistAngle = twistAngle->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::settwistAngle(): The data is nullptr!!");
    }
}

void TwistModifierUI::setSetAngleMethod(int index){
    if (bsData){
        bsData->setAngleMethod = bsData->SetAngleMethod.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setSetAngleMethod(): The data is nullptr!!");
    }
}

void TwistModifierUI::setRotationAxisCoordinates(int index){
    if (bsData){
        bsData->rotationAxisCoordinates = bsData->RotationAxisCoordinates.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setRotationAxisCoordinates(): The data is nullptr!!");
    }
}

void TwistModifierUI::setIsAdditive(){
    if (bsData){
        bsData->isAdditive = isAdditive->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setIsAdditive(): The data is nullptr!!");
    }
}

void TwistModifierUI::viewSelected(int row, int column){
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
            case AXIS_OF_ROTATION_ROW:
                if (table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "axisOfRotation");
                break;
            case TWIST_ANGLE_ROW:
                if (table->item(TWIST_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "twistAngle");
                break;
            case START_BONE_INDEX_ROW:
                if (table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "startBoneIndex");
                break;
            case END_BONE_INDEX_ROW:
                if (table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "endBoneIndex");
                break;
            case IS_ADDITIVE_ROW:
                if (table->item(IS_ADDITIVE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "isAdditive");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void TwistModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void TwistModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("axisOfRotation");
            if (bindIndex == index){
                table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("twistAngle");
            if (bindIndex == index){
                table->item(TWIST_ANGLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("startBoneIndex");
            if (bindIndex == index){
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("endBoneIndex");
            if (bindIndex == index){
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("isAdditive");
            if (bindIndex == index){
                table->item(IS_ADDITIVE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool TwistModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("TwistModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("TwistModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void TwistModifierUI::setBindingVariable(int index, const QString &name){
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
        case AXIS_OF_ROTATION_ROW:
            if (table->item(AXIS_OF_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "axisOfRotation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case TWIST_ANGLE_ROW:
            if (table->item(TWIST_ANGLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "twistAngle", VARIABLE_TYPE_REAL, isProperty);
            break;
        case START_BONE_INDEX_ROW:
            if (table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "startBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case END_BONE_INDEX_ROW:
            if (table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "endBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case IS_ADDITIVE_ROW:
            if (table->item(IS_ADDITIVE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "isAdditive", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void TwistModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("TwistModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TwistModifierUI::loadBinding(): The data is nullptr!!");
    }
}
