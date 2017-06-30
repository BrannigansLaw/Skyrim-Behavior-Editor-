#include "bslimbikmodifierui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/bslimbikmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define ENABLE_ROW 1
#define LIMIT_ANGLE_DEGREES_ROW 2
#define START_BONE_INDEX_ROW 3
#define END_BONE_INDEX_ROW 4
#define GAIN_ROW 5
#define BONE_RADIUS_ROW 6
#define CAST_OFFSET_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSLimbIKModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSLimbIKModifierUI::BSLimbIKModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      limitAngleDegrees(new DoubleSpinBox),
      startBoneIndex(new ComboBox),
      endBoneIndex(new ComboBox),
      gain(new DoubleSpinBox),
      boneRadius(new DoubleSpinBox),
      castOffset(new DoubleSpinBox)
{
    setTitle("BSLimbIKModifier");
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
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDegrees"));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_DEGREES_ROW, VALUE_COLUMN, limitAngleDegrees);
    table->setItem(START_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("startBoneIndex"));
    table->setItem(START_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(START_BONE_INDEX_ROW, VALUE_COLUMN, startBoneIndex);
    table->setItem(END_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("endBoneIndex"));
    table->setItem(END_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(END_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(END_BONE_INDEX_ROW, VALUE_COLUMN, endBoneIndex);
    table->setItem(GAIN_ROW, NAME_COLUMN, new TableWidgetItem("gain"));
    table->setItem(GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GAIN_ROW, VALUE_COLUMN, gain);
    table->setItem(BONE_RADIUS_ROW, NAME_COLUMN, new TableWidgetItem("boneRadius"));
    table->setItem(BONE_RADIUS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(BONE_RADIUS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BONE_RADIUS_ROW, VALUE_COLUMN, boneRadius);
    table->setItem(CAST_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("castOffset"));
    table->setItem(CAST_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CAST_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CAST_OFFSET_ROW, VALUE_COLUMN, castOffset);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSLimbIKModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()), Qt::UniqueConnection);
    connect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)), Qt::UniqueConnection);
    connect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)), Qt::UniqueConnection);
    connect(gain, SIGNAL(editingFinished()), this, SLOT(setGain()), Qt::UniqueConnection);
    connect(boneRadius, SIGNAL(editingFinished()), this, SLOT(setBoneRadius()), Qt::UniqueConnection);
    connect(castOffset, SIGNAL(editingFinished()), this, SLOT(setCastOffset()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSLimbIKModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()));
    disconnect(startBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartBoneIndex(int)));
    disconnect(endBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndBoneIndex(int)));
    disconnect(gain, SIGNAL(editingFinished()), this, SLOT(setGain()));
    disconnect(boneRadius, SIGNAL(editingFinished()), this, SLOT(setBoneRadius()));
    disconnect(castOffset, SIGNAL(editingFinished()), this, SLOT(setCastOffset()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSLimbIKModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSLimbIKModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_LIMB_IK_MODIFIER){
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<BSLimbIKModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            limitAngleDegrees->setValue(bsData->limitAngleDegrees);
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
            gain->setValue(bsData->gain);
            boneRadius->setValue(bsData->boneRadius);
            castOffset->setValue(bsData->castOffset);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, "limitAngleDegrees");
                loadBinding(START_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "startBoneIndex");
                loadBinding(END_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "endBoneIndex");
                loadBinding(GAIN_ROW, BINDING_COLUMN, varBind, "gain");
                loadBinding(BONE_RADIUS_ROW, BINDING_COLUMN, varBind, "boneRadius");
                loadBinding(CAST_OFFSET_ROW, BINDING_COLUMN, varBind, "castOffset");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(BONE_RADIUS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CAST_OFFSET_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSLimbIKModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setName(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setLimitAngleDegrees(){
    if (bsData){
        if (bsData->limitAngleDegrees != limitAngleDegrees->value()){
            bsData->limitAngleDegrees = limitAngleDegrees->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setLimitAngleDegrees(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setStartBoneIndex(int index){
    if (bsData){
        bsData->startBoneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setStartBoneIndex(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setEndBoneIndex(int index){
    if (bsData){
        bsData->endBoneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setEndBoneIndex(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setGain(){
    if (bsData){
        if (bsData->gain != gain->value()){
            bsData->gain = gain->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setGain(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setBoneRadius(){
    if (bsData){
        if (bsData->boneRadius != boneRadius->value()){
            bsData->boneRadius = boneRadius->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setBoneRadius(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::setCastOffset(){
    if (bsData){
        if (bsData->castOffset != castOffset->value()){
            bsData->castOffset = castOffset->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setCastOffset(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::viewSelected(int row, int column){
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
            case LIMIT_ANGLE_DEGREES_ROW:
                if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "limitAngleDegrees");
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
            case GAIN_ROW:
                if (table->item(GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "gain");
                break;
            case BONE_RADIUS_ROW:
                if (table->item(BONE_RADIUS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "boneRadius");
                break;
            case CAST_OFFSET_ROW:
                if (table->item(CAST_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "castOffset");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSLimbIKModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("limitAngleDegrees");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("startBoneIndex");
            if (bindIndex == index){
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("endBoneIndex");
            if (bindIndex == index){
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("gain");
            if (bindIndex == index){
                table->item(GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("boneRadius");
            if (bindIndex == index){
                table->item(BONE_RADIUS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("castOffset");
            if (bindIndex == index){
                table->item(CAST_OFFSET_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSLimbIKModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSLimbIKModifierUI::setBindingVariable(int index, const QString &name){
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
        case LIMIT_ANGLE_DEGREES_ROW:
            if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "limitAngleDegrees", VARIABLE_TYPE_REAL, isProperty);
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
        case GAIN_ROW:
            if (table->item(GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "gain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case BONE_RADIUS_ROW:
            if (table->item(BONE_RADIUS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "boneRadius", VARIABLE_TYPE_REAL, isProperty);
            break;
        case CAST_OFFSET_ROW:
            if (table->item(CAST_OFFSET_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "castOffset", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSLimbIKModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadBinding(): The data is NULL!!"));
    }
}
