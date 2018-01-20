#include "keyframeinfoui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 4

#define KEYFRAMED_POSITION_ROW 0
#define KEYFRAMED_ROTATION_ROW 1
#define BONE_INDEX_ROW 2
#define IS_VALID_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList KeyframeInfoUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

KeyframeInfoUI::KeyframeInfoUI()
    : file(nullptr),
      keyframeIndex(-1),
      bsData(nullptr),
      parent(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      keyframedPosition(new QuadVariableWidget),
      keyframedRotation(new QuadVariableWidget),
      boneIndex(new ComboBox),
      isValid(new CheckBox)
{
    setTitle("hkKeyframeInfo");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(KEYFRAMED_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("keyframedPosition"));
    table->setItem(KEYFRAMED_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(KEYFRAMED_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(KEYFRAMED_POSITION_ROW, VALUE_COLUMN, keyframedPosition);
    table->setItem(KEYFRAMED_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("keyframedRotation"));
    table->setItem(KEYFRAMED_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(KEYFRAMED_ROTATION_ROW, VALUE_COLUMN, keyframedRotation);
    table->setItem(BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("boneIndex"));
    table->setItem(BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BONE_INDEX_ROW, VALUE_COLUMN, boneIndex);
    table->setItem(IS_VALID_ROW, NAME_COLUMN, new TableWidgetItem("isValid"));
    table->setItem(IS_VALID_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_VALID_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_VALID_ROW, VALUE_COLUMN, isValid);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void KeyframeInfoUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(keyframedPosition, SIGNAL(editingFinished()), this, SLOT(setKeyframedPosition()), Qt::UniqueConnection);
    connect(keyframedRotation, SIGNAL(editingFinished()), this, SLOT(setKeyframedRotation()), Qt::UniqueConnection);
    connect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)), Qt::UniqueConnection);
    connect(isValid, SIGNAL(released()), this, SLOT(setIsValid()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void KeyframeInfoUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(keyframedPosition, SIGNAL(editingFinished()), this, SLOT(setKeyframedPosition()));
    disconnect(keyframedRotation, SIGNAL(editingFinished()), this, SLOT(setKeyframedRotation()));
    disconnect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)));
    disconnect(isValid, SIGNAL(released()), this, SLOT(setIsValid()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void KeyframeInfoUI::loadData(BehaviorFile *parentFile, hkbKeyframeBonesModifier::hkKeyframeInfo *key, hkbKeyframeBonesModifier *par, int index){
    disconnectSignals();
    if (parentFile && key && par && index > -1){
        parent = par;
        keyframeIndex = index;
        file = parentFile;
        bsData = key;
        QStringList boneNames("None");
        keyframedPosition->setValue(bsData->keyframedPosition);
        keyframedRotation->setValue(bsData->keyframedRotation);
        if (boneIndex->count() == 0){
            boneNames = boneNames + file->getRagdollBoneNames();
            boneIndex->insertItems(0, boneNames);
        }
        boneIndex->setCurrentIndex(bsData->boneIndex + 1);
        isValid->setChecked(bsData->isValid);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(KEYFRAMED_POSITION_ROW, BINDING_COLUMN, varBind, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedPosition");
            loadBinding(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN, varBind, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedRotation");
            loadBinding(BONE_INDEX_ROW, BINDING_COLUMN, varBind, "keyframeInfo:"+QString::number(keyframeIndex)+"/boneIndex");
            loadBinding(IS_VALID_ROW, BINDING_COLUMN, varBind, "keyframeInfo:"+QString::number(keyframeIndex)+"/isValid");
        }else{
            table->item(KEYFRAMED_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(IS_VALID_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::loadData(): Behavior file, bind or event data is null!!!");
    }
    connectSignals();
}

void KeyframeInfoUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(file)->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(file)->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            FATAL_RUNTIME_ERROR("KeyframeInfoUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::loadBinding(): The data is nullptr!!");
    }
}

bool KeyframeInfoUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(file)->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(file)->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(file);
                parent->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            file->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void KeyframeInfoUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case KEYFRAMED_POSITION_ROW:
            if (table->item(KEYFRAMED_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case KEYFRAMED_ROTATION_ROW:
            if (table->item(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case BONE_INDEX_ROW:
            if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "keyframeInfo:"+QString::number(keyframeIndex)+"/boneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case IS_VALID_ROW:
            if (table->item(IS_VALID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "keyframeInfo:"+QString::number(keyframeIndex)+"/isValid", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setBindingVariable(): The data is nullptr!!");
    }
}

void KeyframeInfoUI::setKeyframedPosition(){
    if (bsData && file){
        if (bsData->keyframedPosition != keyframedPosition->value()){
            bsData->keyframedPosition = keyframedPosition->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setkeyframedPosition(): Behavior file or event data is null!!!");
    }
}

void KeyframeInfoUI::setKeyframedRotation(){
    if (bsData && file){
        if (bsData->keyframedRotation != keyframedRotation->value()){
            bsData->keyframedRotation = keyframedRotation->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setkeyframedRotation(): Behavior file or event data is null!!!");
    }
}

void KeyframeInfoUI::setBoneIndex(int index){
    if (bsData && file){
        bsData->boneIndex = index - 1;
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setboneIndex(): Behavior file or event data is null!!!");
    }
}

void KeyframeInfoUI::setIsValid(){
    if (bsData && file){
        if (bsData->isValid != isValid->isChecked()){
            bsData->isValid = isValid->isChecked();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::setIsAnnotation(): Behavior file or event data is null!!!");
    }
}

void KeyframeInfoUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case KEYFRAMED_POSITION_ROW:
                if (table->item(KEYFRAMED_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedPosition");
                break;
            case KEYFRAMED_ROTATION_ROW:
                if (table->item(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedRotation");
                break;
            case BONE_INDEX_ROW:
                if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "keyframeInfo:"+QString::number(keyframeIndex)+"/boneIndex");
                break;
            case IS_VALID_ROW:
                if (table->item(IS_VALID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "keyframeInfo:"+QString::number(keyframeIndex)+"/isValid");
                break;
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void KeyframeInfoUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (parent->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (parent->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::selectTableToView(): The data is nullptr!!");
    }
}

void KeyframeInfoUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("KeyframeInfoUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedPosition");
            if (bindIndex == index){
                table->item(KEYFRAMED_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("keyframeInfo:"+QString::number(keyframeIndex)+"/keyframedRotation");
            if (bindIndex == index){
                table->item(KEYFRAMED_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("keyframeInfo:"+QString::number(keyframeIndex)+"/boneIndex");
            if (bindIndex == index){
                table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("keyframeInfo:"+QString::number(keyframeIndex)+"/isValid");
            if (bindIndex == index){
                table->item(IS_VALID_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("KeyframeInfoUI::variableRenamed(): The data is nullptr!!");
    }
}
