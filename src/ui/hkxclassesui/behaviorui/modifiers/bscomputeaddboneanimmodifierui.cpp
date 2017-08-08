#include "bscomputeaddboneanimmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/bscomputeaddboneanimmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define ENABLE_ROW 1
#define BONE_INDEX_ROW 2
#define TRANSLATION_LS_OUT_ROW 3
#define ROTATION_LS_OUT_ROW 4
#define SCALE_LS_OUT_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSComputeAddBoneAnimModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSComputeAddBoneAnimModifierUI::BSComputeAddBoneAnimModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      boneIndex(new ComboBox),
      translationLSOut(new QuadVariableWidget),
      rotationLSOut(new QuadVariableWidget),
      scaleLSOut(new QuadVariableWidget)
{
    setTitle("BSComputeAddBoneAnimModifier");
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
    table->setItem(BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("boneIndex"));
    table->setItem(BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BONE_INDEX_ROW, VALUE_COLUMN, boneIndex);
    table->setItem(TRANSLATION_LS_OUT_ROW, NAME_COLUMN, new TableWidgetItem("translationLSOut"));
    table->setItem(TRANSLATION_LS_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TRANSLATION_LS_OUT_ROW, VALUE_COLUMN, translationLSOut);
    table->setItem(ROTATION_LS_OUT_ROW, NAME_COLUMN, new TableWidgetItem("rotationLSOut"));
    table->setItem(ROTATION_LS_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ROTATION_LS_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROTATION_LS_OUT_ROW, VALUE_COLUMN, rotationLSOut);
    table->setItem(SCALE_LS_OUT_ROW, NAME_COLUMN, new TableWidgetItem("scaleLSOut"));
    table->setItem(SCALE_LS_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SCALE_LS_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SCALE_LS_OUT_ROW, VALUE_COLUMN, scaleLSOut);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSComputeAddBoneAnimModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)), Qt::UniqueConnection);
    connect(translationLSOut, SIGNAL(editingFinished()), this, SLOT(setTranslationLSOut()), Qt::UniqueConnection);
    connect(rotationLSOut, SIGNAL(editingFinished()), this, SLOT(setRotationLSOut()), Qt::UniqueConnection);
    connect(scaleLSOut, SIGNAL(editingFinished()), this, SLOT(setScaleLSOut()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSComputeAddBoneAnimModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)));
    disconnect(translationLSOut, SIGNAL(editingFinished()), this, SLOT(setTranslationLSOut()));
    disconnect(rotationLSOut, SIGNAL(editingFinished()), this, SLOT(setRotationLSOut()));
    disconnect(scaleLSOut, SIGNAL(editingFinished()), this, SLOT(setScaleLSOut()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSComputeAddBoneAnimModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSComputeAddBoneAnimModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_COMPUTE_ADD_BONE_ANIM_MODIFIER){
            QStringList boneNames;
            boneNames.append("None");
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<BSComputeAddBoneAnimModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            if (boneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                boneIndex->insertItems(0, boneNames);
            }
            boneIndex->setCurrentIndex(bsData->boneIndex + 1);
            translationLSOut->setValue(bsData->translationLSOut);
            rotationLSOut->setValue(bsData->rotationLSOut);
            scaleLSOut->setValue(bsData->scaleLSOut);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(BONE_INDEX_ROW, BINDING_COLUMN, varBind, "boneIndex");
                loadBinding(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN, varBind, "translationLSOut");
                loadBinding(ROTATION_LS_OUT_ROW, BINDING_COLUMN, varBind, "rotationLSOut");
                loadBinding(SCALE_LS_OUT_ROW, BINDING_COLUMN, varBind, "scaleLSOut");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROTATION_LS_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SCALE_LS_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSComputeAddBoneAnimModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setName(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::setBoneIndex(int index){
    if (bsData){
        bsData->boneIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setboneIndex(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::setTranslationLSOut(){
    if (bsData){
        if (bsData->translationLSOut != translationLSOut->value()){
            bsData->translationLSOut = translationLSOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::settranslationLSOut(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::setRotationLSOut(){
    if (bsData){
        if (bsData->rotationLSOut != rotationLSOut->value()){
            bsData->rotationLSOut = rotationLSOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setrotationLSOut(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::setScaleLSOut(){
    if (bsData){
        if (bsData->scaleLSOut != scaleLSOut->value()){
            bsData->scaleLSOut = scaleLSOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setscaleLSOut(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::viewSelected(int row, int column){
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
            case BONE_INDEX_ROW:
                if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "boneIndex");
                break;
            case TRANSLATION_LS_OUT_ROW:
                if (table->item(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "translationLSOut");
                break;
            case ROTATION_LS_OUT_ROW:
                if (table->item(ROTATION_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotationLSOut");
                break;
            case SCALE_LS_OUT_ROW:
                if (table->item(SCALE_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "scaleLSOut");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSComputeAddBoneAnimModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("boneIndex");
            if (bindIndex == index){
                table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("translationLSOut");
            if (bindIndex == index){
                table->item(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rotationLSOut");
            if (bindIndex == index){
                table->item(ROTATION_LS_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("scaleLSOut");
            if (bindIndex == index){
                table->item(SCALE_LS_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSComputeAddBoneAnimModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSComputeAddBoneAnimModifierUI::setBindingVariable(int index, const QString &name){
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
        case BONE_INDEX_ROW:
            if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "boneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case TRANSLATION_LS_OUT_ROW:
            if (table->item(TRANSLATION_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "translationLSOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ROTATION_LS_OUT_ROW:
            if (table->item(ROTATION_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotationLSOut", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case SCALE_LS_OUT_ROW:
            if (table->item(SCALE_LS_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "scaleLSOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSComputeAddBoneAnimModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSComputeAddBoneAnimModifierUI::loadBinding(): The data is NULL!!"));
    }
}
